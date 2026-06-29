//这里是具体的battlesystem的实现
//可能会看到莫名其妙要加f，这是因为防止下面warning那一直警告说数据类型的转换
//一些地方用了auto便于代码的简洁性
#include "BattleSystem.h"
#include "../include/codemon.h"
#include "../include/ItemDB.h"
#include "../include/ItemType.h"
#include "battleUI.h"
#include<iostream>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <memory>
using namespace std;
using namespace monbase;
BattleSystem::BattleSystem(Player* player, vector<unique_ptr<Codemon>>& enemyTeam): BattleSystemBase(player) 
{
    //拷贝玩家的精灵，从Player的team向量转移到BattleSystem的playerTeam
    for (auto& codemon : player->team) 
    {
        playerTeam.push_back(codemon.get());
    }
    //确保玩家的精灵在进入战斗时处于初始状态
    for (auto *c : playerTeam) 
    {
        if (c) c->rest();
    }
    //拷贝敌人的精灵，从传入的enemyTeam向量转移到BattleSystem的enemyTeam
    for (auto& codemon : enemyTeam) 
    {
        this->enemyTeam.push_back(codemon.get());
    }
    activecodemon = playerTeam[0];
    activeEnemy = this->enemyTeam[0];
    battleLog.push_back(activecodemon->getName() + "登场，这局随便赢");
    battleLog.push_back(activeEnemy->getName() + "登场");
    initTypeChart();
    animationDuration = 1.0f;
    srand(int(time(nullptr)));
    playerTurnClock.restart();
}

//update函数是战斗系统的核心更新函数，根据当前的战斗状态来执行不同的逻辑，比如玩家回合时检查时间限制，敌人回合时执行AI，动画播放时等待动画结束，等待切换时检查切换时间，战斗结束时调用回调函数等
void BattleSystem::update() 
{
    switch (currentState)
    {
        case BattleState::PLAYER_TURN: //玩家的回合
        {
            float elapsed = playerTurnClock.getElapsedTime().asSeconds();//获取玩家回合已经过去的时间，单位是秒
            if (elapsed >= turnTimeLimit)
            {
                string log = activecodemon->getName() + " 使用 聚能 ";
                battleLog.push_back(log);
                currentState = BattleState::ENEMY_TURN;//到时间了，切换到敌人回合
                playerTurnClock.restart();//重置回合计时器
            }
            ui->updateSkillBtns(activecodemon->getEquippedMoves());//更新技能按钮的状态，可能有些技能因为能量不足不能用
            break;
        }

        case BattleState::ENEMY_TURN://敌人的回合
        {
            checkFaint();
            enemyAI_selectOnly();// 敌人选择技能
            enemyAI_executeSelected(); // 执行敌人技能
            currentState = BattleState::ANIMATING;
            break;
        }

        case BattleState::ANIMATING://播放动画的状态，这里我们没有具体的动画，所以就是一个过渡时间
        {
            if (animationClock.getElapsedTime().asSeconds() >= animationDuration)
            {
                checkFaint();//检查精灵是否濒死，如果有精灵死了就切换到等待切换状态，否则继续玩家回合
                if (currentState != BattleState::WAITING_FOR_SWITCH)
                {
                    currentState = BattleState::PLAYER_TURN;
                    playerTurnClock.restart();
                }
                //这里是重置防御的机制
                playerIsDefending = false;
                playerCounterMove = nullptr;
                enemyIsDefending = false;
                enemyCounterMove = nullptr;
            }
            break;
        }

        case BattleState::WAITING_FOR_SWITCH: //这个是玩家切换精灵的状态，当玩家的精灵死了必须要换精灵就在这个状态
        {
            float elapsed = playerTurnClock.getElapsedTime().asSeconds();
            if (elapsed >= turnTimeLimit)
            {
                switchToNextAlive(activecodemon, playerTeam);//这里的意思是玩家如果超过时间限制还没有切换精灵，就自动切换到下一只存活的精灵
                if (activecodemon)
                {
                    string log = activecodemon->getName() + " 使用 聚能";//这里时间到了自动使用聚能
                    battleLog.push_back(log);
                }
                currentState = BattleState::ENEMY_TURN;//转到敌人回合
                playerTurnClock.restart();//重置回合计时器
            }
            break;
        }

        case BattleState::BATTLE_END://这个是战斗结束的状态，当玩家赢了输了或者逃跑了就会进入这个状态
        {
            playerTurnClock.restart();//重置回合计时器
            for (int i = 0; i < playerTeam.size(); i++)
            {
                playerTeam[i]->rest();
            }
            break;
        }
    }
}

//这个函数是执行玩家行动的函数，根据玩家选择的行动类型和目标索引来执行相应的逻辑，比如使用技能、聚能、切换精灵等，并且根据当前状态来判断是否允许执行这些行动，以及在执行后更新战斗状态和UI显示等
void BattleSystem::executePlayerAction(int actionType, int targetIndex) //actionType是玩家选择的行动类型，targetIndex是目标索引，具体含义根据actionType不同而不同，比如攻击或技能时表示敌人索引，切换精灵时表示队伍索引等
{
	//精灵死亡等待切换时，仅允许切换精灵(切换精灵的actionType是10)，其他行动类型都不允许执行，直接处理这种特殊情况
    if (currentState == BattleState::WAITING_FOR_SWITCH) 
    {
        playerSwitchCodemon(targetIndex);
        battleLog.push_back( activecodemon->getName()+"登场");
		playerTurnClock.restart();//切换后重置回合计时器
        currentState = BattleState::PLAYER_TURN; //切换后让玩家继续操作，玩家可以进行一次操作

        //更新UI
		ui->updatePlayer(activecodemon);//更新玩家UI
        ui->updateEnemy(activeEnemy);  //更新敌人UI
		ui->updateSkillBtns(activecodemon->getEquippedMoves());//更新技能按钮
        ui->updateCodemonBtns();  // 更新精灵按钮
        return;
    }
	//下面是正常情况下玩家回合时的行动处理，根据actionType来判断玩家选择了什么行动，并执行相应的逻辑，同时根据技能类型和敌人状态来判断行动顺序，确保防御技能优先设置护盾等机制
    switch (actionType) 
    {
        case 0: 
        case 1:
        case 2: 
	    case 3: //case0，1，2，3都是技能，刚好4个技能，根据actionType来获取玩家选择的技能，并执行技能逻辑，同时处理防御技能的优先级和反击机制
        {
			    const Move* move = activecodemon->getEquippedMove(actionType);//记录当前玩家选择的技能，根据actionType来获取玩家选择的技能，actionType应该是0-3，分别对应四个技能槽位
                if (!activecodemon->hasEnoughEnergy(move->energyCost))
                {
                    battleLog.push_back("能量不够不能使用" + move->name );
                    return;
                }
				enemyAI_selectOnly();//执行敌人AI的技能选择逻辑，确保在玩家行动后敌人已经选择了技能，这样我们才能根据敌人选择的技能类型来判断行动顺序，确保防御技能优先设置护盾等机制
                playerIsDefending = (move->category == MoveCategory::Defense);
				playerIsAttacking = (move->category == MoveCategory::Attack);
                if (playerIsDefending && enemyIsAttacking)
                {
                    playerCounterMove = move;
                    MoveResult result = activecodemon->useEquippedMove(actionType, activeEnemy);//执行技能，传入技能槽位和目标敌人
                    if (result.success)
                    {
                        battleLog.push_back(activecodemon->getName() + " 使用 " + move->name);
                        applyStatusEffects(activeEnemy, *move);//应用状态效果，传入目标敌人和技能，负责根据技能的效果来修改目标的状态（中毒、灼烧、冻伤等）1111111111111111111111111111111111111111111111111
                        activecodemon->processTurnEndEffects();//处理回合结束效果
						enemyAI_executeSelected();//执行敌人AI已经选择的技能
                        //更新UI血条显示
                        ui->updatePlayer(activecodemon);
                        ui->updateEnemy(activeEnemy);
                        currentState = BattleState::ANIMATING;
                    }
                }
                else if(playerIsAttacking&& enemyIsDefending)
                {
                    enemyAI_executeSelected();//执行敌人AI已经选择的技能
                    MoveResult result = activecodemon->useEquippedMove(actionType, activeEnemy);//执行技能，传入技能槽位和目标敌人
                    if (result.success)
                    {
                        battleLog.push_back(activecodemon->getName() + " 使用 " + move->name);
						applyStatusEffects(activeEnemy, *move);//应用状态效果，传入目标敌人和技能，负责根据技能的效果来修改目标的状态（中毒、灼烧、冻伤等）  
                        activecodemon->processTurnEndEffects();//处理回合结束效果
                        //更新UI血条显示
                        ui->updatePlayer(activecodemon);
                        ui->updateEnemy(activeEnemy);
                    }
                    currentState = BattleState::ANIMATING;
                    animationClock.restart();
                }
                else 
                {
                    int playerSpeed = activecodemon->getEffectiveSpeed();
                    int enemySpeed = activeEnemy->getEffectiveSpeed();
                    if (enemySpeed > playerSpeed) 
                    {
                        enemyAI_executeSelected();//执行敌人AI已经选择的技能
                        if (activecodemon->getHP() > 0)
                        {
                            MoveResult result = activecodemon->useEquippedMove(actionType, activeEnemy);//执行技能，传入技能槽位和目标敌人
                            if (result.success)
                            {
                                battleLog.push_back(activecodemon->getName() + " 使用 " + move->name);
                                applyStatusEffects(activeEnemy, *move);//应用状态效果，传入目标敌人和技能，负责根据技能的效果来修改目标的状态（中毒、灼烧、冻伤等）1111111111111111111111111111111111111111111111111
                                activecodemon->processTurnEndEffects();//处理回合结束效果
                                //更新UI血条显示
                                ui->updatePlayer(activecodemon);
                                ui->updateEnemy(activeEnemy);

                            }
                        }
                        currentState = BattleState::ANIMATING;
                        animationClock.restart();
                    }
                    else 
                    {
                        MoveResult result = activecodemon->useEquippedMove(actionType, activeEnemy);//执行技能，传入技能槽位和目标敌人
                        if (result.success)
                        {
                            battleLog.push_back(activecodemon->getName() + " 使用 " + move->name);
                            applyStatusEffects(activeEnemy, *move);//应用状态效果，传入目标敌人和技能，负责根据技能的效果来修改目标的状态（中毒、灼烧、冻伤等）1111111111111111111111111111111111111111111111111
                            activecodemon->processTurnEndEffects();//处理回合结束效果
                            if (activeEnemy->getHP() > 0)
                            {
                                enemyAI_executeSelected();//执行敌人AI已经选择的技能
                            }
                            //更新UI血条显示
                            ui->updatePlayer(activecodemon);
                            ui->updateEnemy(activeEnemy);
                        }
                        currentState = BattleState::ANIMATING;
                        animationClock.restart();
                    }
                }
				playerTurnClock.restart();//重置玩家回合计时器
                ui->updatePlayer(activecodemon);//更新能量显示
				ui->updateSkillBtns(activecodemon->getEquippedMoves());//刷新技能按钮状态
                break;
        }
        case 4: //聚能操作
        { 
                enemyAI_selectOnly();
                int playerSpeed = activecodemon->getEffectiveSpeed();
                int enemySpeed = activeEnemy->getEffectiveSpeed();
                if (enemySpeed > playerSpeed)
                {
                    enemyAI_executeSelected();//执行敌人AI已经选择的技能
                    MoveResult result = activecodemon->useEquippedMove(actionType, activeEnemy);//执行技能，传入技能槽位和目标敌人
                    activecodemon->useEnergyCharge();
                    battleLog.push_back(activecodemon->getName() + " 使用 聚能 ");
                    //更新UI血条显示
                    ui->updatePlayer(activecodemon);
                    ui->updateEnemy(activeEnemy);

                    currentState = BattleState::ANIMATING;
                    animationClock.restart();
                }
                else
                {
                    activecodemon->useEnergyCharge();
                    battleLog.push_back(activecodemon->getName() + " 使用 聚能 ");
                    enemyAI_executeSelected();//执行敌人AI已经选择的技能
                    //更新UI血条显示
                     ui->updatePlayer(activecodemon);
                     ui->updateEnemy(activeEnemy);
                    
                    enemyAI_executeSelected();//执行敌人AI已经选择的技能
                    currentState = BattleState::ANIMATING;
                    animationClock.restart();
                }
                 playerTurnClock.restart();//重置玩家回合计时器
                 ui->updatePlayer(activecodemon);  // 更新能量显示
                 ui->updateSkillBtns(activecodemon->getEquippedMoves());//刷新技能按钮状态
            break;
        }
        case 5: //点击战斗栏目按钮的操作
        { 
            currentState = BattleState::PLAYER_TURN;
            ui->updatePlayer(activecodemon);  // 更新能量显示
            ui->updateSkillBtns(activecodemon->getEquippedMoves());// 启用技能按钮
            break;
        }
        case 6: //点击道具栏目按钮的操作
        {
            currentState = BattleState::PLAYER_TURN;
            ui->updatePlayer(activecodemon);  // 更新能量显示
            ui->updateItemBtns(); //启用道具按钮
            break;
        }
		case 7: 
        { 
            currentState = BattleState::PLAYER_TURN;
            ui->updatePlayer(activecodemon);  // 更新能量显示
            ui->updateCodemonBtns(); // 更新精灵按钮
            break;
        }
		case 8: //点击撤退按钮的操作
        { 
            battleLog.push_back("玩家选择撤退！");
            battleResult = BattleResult::PLAYER_LOSE;
            currentState = BattleState::BATTLE_END;
            break;
        }
		case 9: //使用道具的操作
        { 
            if (ui)
            {
                //我们是根据点击的道具按钮来确定道具的使用
                ItemID itemID = ItemID(targetIndex);
                bool success = player->bag.use(itemID, activecodemon);
                if (success) 
                {
                    auto item = ItemDB::getItem(itemID);
                    battleLog.push_back(player->name + " 使用 " + item->getName());
                    // 更新UI
                    ui->updatePlayer(activecodemon);
                    ui->updateEnemy(activeEnemy);
                    ui->updateItemBtns();  // 更新道具按钮
                    playerTurnClock.restart();
                    currentState = BattleState::ENEMY_TURN;
                }
            }
            break;
        }
        case 10: //切换精灵的操作
        {
            playerSwitchCodemon(targetIndex);
            battleLog.push_back("玩家切换精灵到" + activecodemon->getName());
            playerTurnClock.restart();
            currentState = BattleState::ENEMY_TURN; //主动切换：结束玩家操作，轮到敌人
            break;
        }
    }
}

//计算属性克制倍率的函数
float BattleSystem::calculateTypeMultiplier(Element skillElement, const vector<Element>& targetElements) 
{
    if (skillElement == Element::None) //首先检查技能属性是否为None，如果是则返回1.0f表示没有克制关系
    {
        return 1.0f;
    }

	float multiplier = 1.0f;//初始定义倍率为1.0f，表示默认没有克制关系
    for (Element elem : targetElements) 
    {
		auto it_skill = typeChart.find(skillElement);//这里想到vector中学到的方法直接找到技能属性在克制表中的位置
        if (it_skill != typeChart.end()) 
        {
            auto it_target = it_skill->second.find(elem);
            if (it_target != it_skill->second.end())
            {
                multiplier *= it_target->second;
            }
        }
    }

    if (multiplier == 4.0f)
    {
		return 3.0f;//这块进行伤害修改成3倍伤害，避免出现4倍伤害过于夸张的情况，这样可以让战斗更平衡一些
    }
    return multiplier;
}

//初始化属性克制表的函数，负责填充 typeChart 属性克制表，克制是2倍伤害，A克制B那么B对A的伤害就是0.5倍，其他一律是1倍伤害
void BattleSystem::initTypeChart()
{
	// Fire,火克制草和冰，但被水和地克制
    typeChart[Element::Fire][Element::Grass] = 2.0f;
    typeChart[Element::Fire][Element::Ice] = 2.0f;
    typeChart[Element::Fire][Element::Water] = 0.5f;
    typeChart[Element::Fire][Element::Ground] = 0.5f;

	// Water，水克制火和地，但被草和电克制
    typeChart[Element::Water][Element::Fire] = 2.0f;
    typeChart[Element::Water][Element::Ground] = 2.0f;
    typeChart[Element::Water][Element::Grass] = 0.5f;
    typeChart[Element::Water][Element::Electric] = 0.5f;

	// Grass，草克制水和地，但被火克制
    typeChart[Element::Grass][Element::Water] = 2.0f;
    typeChart[Element::Grass][Element::Ground] = 2.0f;
    typeChart[Element::Grass][Element::Fire] = 0.5f;

    // Electric，电克制水，但被地克制
    typeChart[Element::Electric][Element::Water] = 2.0f;
    typeChart[Element::Electric][Element::Ground] = 0.5f;

    // Ice，冰克制草和地，但被火克制
    typeChart[Element::Ice][Element::Grass] = 2.0f;
    typeChart[Element::Ice][Element::Ground] = 2.0f;
    typeChart[Element::Ice][Element::Poison] = 2.0f;
    typeChart[Element::Ice][Element::Fighting] = 0.5f;

	// Fighting，格斗克制冰和毒，但被飞行克制
    typeChart[Element::Fighting][Element::Ice] = 2.0f;
    typeChart[Element::Fighting][Element::Poison] = 0.5f;

	// Poison，毒克制草和地，但被格斗和冰克制
    typeChart[Element::Poison][Element::Grass] = 2.0f;
    typeChart[Element::Poison][Element::Ground] = 2.0f;
    typeChart[Element::Poison][Element::Fighting] = 2.0f;
    typeChart[Element::Poison][Element::Poison] = 2.0f;
    typeChart[Element::Poison][Element::Ice] = 0.5f;

	// Ground，地克制火、电和毒，但被水和草克制
    typeChart[Element::Ground][Element::Fire] = 2.0f;
    typeChart[Element::Ground][Element::Electric] = 2.0f;
    typeChart[Element::Ground][Element::Water] = 0.5f;
    typeChart[Element::Ground][Element::Grass] = 0.5f;
}

//计算伤害的函数
void BattleSystem::calculateDamage(Codemon* attacker, Codemon* defender, const Move& move) 
{
    if (defender == activecodemon && playerIsDefending && playerCounterMove)
    {
        activecodemon->applyCounterEffect(*playerCounterMove, attacker);
        playerIsDefending = false;
        playerCounterMove = nullptr;
        ui->updatePlayer(activecodemon);
        ui->updateEnemy(activeEnemy);
        return;
    }
    else if (defender == activeEnemy && enemyIsDefending && enemyCounterMove)
    {
        activeEnemy->applyCounterEffect(*enemyCounterMove, attacker);
        enemyIsDefending = false;
        enemyCounterMove = nullptr;
        ui->updatePlayer(activecodemon);
        ui->updateEnemy(activeEnemy);
        return;
    }
    //计算基础伤害，就是本来应该打出来的不考虑因素的伤害
    int baseDamage = attacker->calculateDamage(move, *defender);
    //计算属性克制倍数
    vector<Element> defenderElements = { defender->getType1() };
    if (defender->getType2() != Element::None) //还得看是不是双属性的精灵
    {
        defenderElements.push_back(defender->getType2());
    }
    float typeMultiplier = calculateTypeMultiplier(move.element, defenderElements);
    //计算总伤害，要是整数
	int totalDamage = int(baseDamage * typeMultiplier);//计算出来的浮点数伤害转换成整数
    totalDamage = max(1, totalDamage);//至少是1
    //受伤
    defender->takeDamage(totalDamage);
	//触发技能效果
    attacker->onMoveUsed(move);
    //日志记录
    string log = attacker->getName() + " 使用了【" + move.name + "】！";
    battleLog.push_back(log);

    //上状态
    applyStatusEffects(defender, move);

    //更新UI血条显示
    ui->updatePlayer(activecodemon);
    ui->updateEnemy(activeEnemy);
}

//给双方加特殊状态
void BattleSystem::applyStatusEffects(Codemon* target, const Move& move) 
{
    if (!target) return;

    switch (move.effect) 
    {
    case MoveEffect::ApplyPoison:
        target->applyStatus(StatusType::Poison, move.power);
        break;
    case MoveEffect::ApplyBurn:
        target->applyStatus(StatusType::Burn, move.power);
        break;
    case MoveEffect::ApplyFreeze:
        target->applyStatus(StatusType::Freeze, move.power);
        break;
    default:
        break;
    }
}

//这是检查精灵是否濒死的函数，在每次行动后都要调用这个函数来检查玩家和敌方的精灵是否有一方已经濒死了
//如果玩家的精灵濒死了就切换到等待切换状态，让玩家选择下一只出战精灵，如果敌方的精灵濒死了就自动切换到下一只存活的敌方精灵，如果双方都有精灵濒死了就判定战斗结果，看看是玩家赢了还是输了
void BattleSystem::checkFaint() 
{
    playerIsDefending = false;
    playerCounterMove = nullptr;
    enemyIsDefending = false;
    enemyCounterMove = nullptr;
    //检测玩家精灵是否濒死
    if (activecodemon && activecodemon->getHP() <= 0) 
    {
        string log = activecodemon->getName() + " 死了，等待替换 ";
        battleLog.push_back(log);
        //检查玩家队伍中是否还有存活的精灵
        bool anyAlive = false;
        for (auto& cod : playerTeam)
        {
            if (cod && cod->getHP() > 0)
            {
                anyAlive = true;
                break;
            }
        }               
        if (!anyAlive)
        {
            battleLog.push_back("玩家失败，可惜兄弟");
            battleResult = BattleResult::PLAYER_LOSE;
            currentState = BattleState::BATTLE_END;
            return;
        }
        else
        {
            currentState = BattleState::WAITING_FOR_SWITCH;
        }
        return;
    }
    if (activeEnemy && activeEnemy->getHP() <= 0) 
    {
        string log = activeEnemy->getName() + " 死了";
        battleLog.push_back(log);
        switchToNextAlive(activeEnemy, enemyTeam);
    }

    //判定战斗结果
    if (activecodemon == nullptr) 
    {
        battleLog.push_back("玩家失败，可惜兄弟");
        battleResult = BattleResult::PLAYER_LOSE;
        currentState = BattleState::BATTLE_END;
    }
    else if (activeEnemy == nullptr) 
    {
        battleLog.push_back("玩家胜利，拿下easy");
        battleResult = BattleResult::PLAYER_WIN;
        currentState = BattleState::BATTLE_END;
    }
}

//这是切换下一个精灵的函数，是自动的一个保底切换
void BattleSystem::switchToNextAlive(Codemon*& current, vector<Codemon*>& team) 
{
    if (team.empty()) 
    {
        current = nullptr;
        return;
    }
    for (auto& codemon : team) 
    {
        if (codemon && codemon->getHP() > 0)
        {
            current = codemon;
            // 更新UI血条显示
            if (ui) 
            {
                if (current == activecodemon) 
                {
                    ui->updatePlayer(activecodemon);
                }
                else if (current == activeEnemy) 
                {
                    ui->updateEnemy(activeEnemy);
                }
            }
            return;
        }
    }
    current = nullptr; //全灭的情况下
}

//外部接口函数，提供给UI调用来获取当前战斗状态
BattleState BattleSystem::getState() const 
{
    return currentState;
}

//外部接口函数，提供给UI调用来获取当前玩家精灵的剩余回合时间
float BattleSystem::getRemainingTime() const 
{
    if (currentState != BattleState::PLAYER_TURN &&currentState != BattleState::WAITING_FOR_SWITCH &&currentState != BattleState::BATTLE_END) 
    {
        return 0.0f;
	}//只有在玩家回合、等待切换和战斗结束状态下才有剩余时间，其他状态返回0
    //使用SFML时钟获取已过时间
    float elapsed = playerTurnClock.getElapsedTime().asSeconds();
    float remaining = turnTimeLimit - elapsed; //turnTimeLimit = 30.0f
    return remaining;
}

//玩家切换精灵的函数
void BattleSystem::playerSwitchCodemon(int index) 
{
    playerIsDefending = false;
    playerCounterMove = nullptr;
    Codemon* target = playerTeam[index];
    //清除下场精灵的所有临时状态
    if (activecodemon) 
    {
        activecodemon->clearStatStages();//清除属性等级
        activecodemon->clearStatuses();//清除状态
        activecodemon->clearShield();//清除护盾
        activecodemon->clearElementUseCount();//清除技能使用计数
    }
    //切换至新精灵
    activecodemon = target;
    //重置回合时钟
    playerTurnClock.restart();
    ui->updatePlayer(activecodemon);
    ui->updateEnemy(activeEnemy);
    ui->updateSkillBtns(activecodemon->getEquippedMoves());
    ui->updateCodemonBtns();
    string texturePath = "Resources/" + activecodemon->getName() + ".png";//111111111111111111111111111111111111111111111111111111111111
    ui->setPlayerSprite(texturePath);
}

//这个是设置UI指针的函数，提供给外部调用来关联UI对象，这样战斗系统就可以通过这个指针来更新UI显示了
void BattleSystem::setUI(battleUI* battleUI) 
{
    ui = battleUI;

}

//提供给UI调用来获取玩家当前精灵的函数，返回一个指向当前玩家精灵的指针
Codemon* BattleSystem::getPlayerCodemon(int index) 
{
    if (index >= 0 && index < int(playerTeam.size())) 
    {
        return playerTeam[index];
    }
    return nullptr;  //没有索引时候返回空
}

//敌人执行技能的函数
void BattleSystem::enemyAI_executeSelected() 
{
    const Move* selectedMove = activeEnemy->getEquippedMove(enemySelectedMoveIndex);//调用codemon里面的函数执行
    MoveResult result = activeEnemy->useEquippedMove(enemySelectedMoveIndex, activecodemon);
    if (result.success) 
    {
        string log = activeEnemy->getName() + " 使用 " + selectedMove->name;
        battleLog.push_back(log);
        applyStatusEffects(activecodemon, *selectedMove);
        activeEnemy->processTurnEndEffects();
        ui->updatePlayer(activecodemon);
        ui->updateEnemy(activeEnemy);
    }
}

//敌人选择技能的函数
//这里的敌人AI选择技能的逻辑是根据当前敌人的HP百分比来调整攻击、状态和防御技能的权重
//然后按权重随机选择一个技能类型，优先选择这个类型的技能，如果没有可用的技能再选择第二个类型的技能，如果还是没有就自动使用聚能
//我这里用了随机数实现随机选择技能
void BattleSystem::enemyAI_selectOnly() 
{
    int maxHP = activeEnemy->getMaxHP();
    int hpPercent =(activeEnemy->getHP() * 100) / maxHP ;
    float attackWeight = 0.2f;
    float statusWeight = 0.3f;
    float defenseWeight = 0.5f;
    if (hpPercent >= 50) 
    {
        attackWeight = 0.7f;
        statusWeight = 0.2f;
        defenseWeight = 0.1f;
    }
    else if (hpPercent >= 30 && hpPercent < 50)
    {
        attackWeight = 0.4f;
        statusWeight = 0.4f;
        defenseWeight = 0.2f;
    }
    //按权重选第一个技能类型
    float randNum =(1.0*rand()) / (1.0*RAND_MAX);//这里是生成一个0到1之间的随机数，来根据权重选择技能类型
    MoveCategory firstCategory;
    if (randNum <= attackWeight) 
    {
        firstCategory = MoveCategory::Attack;
    }
    else if (randNum < (attackWeight + statusWeight)&&randNum>attackWeight) 
    {
        firstCategory = MoveCategory::Status;
    }
    else 
    {
        firstCategory = MoveCategory::Defense;
    }
	//记录第一个技能类型的选择结果，下面是根据这个类型来优先选择技能的逻辑
    int selectedSlot = -1;
    int equippedCount = activeEnemy->getEquippedCount();
    for (int i = 0; i < equippedCount; i++) 
    {
        const Move* move = activeEnemy->getEquippedMove(i);
        int effCost = activeEnemy->getEffectiveCost(*move);
        if (move->category == firstCategory && activeEnemy->hasEnoughEnergy(effCost)) 
        {
            selectedSlot = i;
            break;
        }
    }
	//如果没有第一个技能类型的可用技能，再选第二个技能类型
    if (selectedSlot == -1) 
    {
        MoveCategory secondCategory;
        if (firstCategory == MoveCategory::Attack) 
        {
            secondCategory = MoveCategory::Status;
        }
        else if (firstCategory == MoveCategory::Status)
        {
            secondCategory = MoveCategory::Attack;
        }
        else 
        {
            secondCategory = MoveCategory::Status;
        }
        for (int i = 0; i < equippedCount; i++) 
        {
            const Move* move = activeEnemy->getEquippedMove(i);
            int effCost = activeEnemy->getEffectiveCost(*move);
            if (move->category == secondCategory && activeEnemy->hasEnoughEnergy(effCost))
            {
                selectedSlot = i;
                break;
            }
        }
    }
    //两个技能都没有的情况下就聚能
    if (selectedSlot == -1) 
    {
        activeEnemy->useEnergyCharge();
        string log = activeEnemy->getName() + " 使用 聚能 ";
        battleLog.push_back(log);
        enemySelectedMoveIndex = -1;
        enemyIsDefending = false;
        return;
    }
    // 保存选择的技能
    enemySelectedMoveIndex = selectedSlot;
    const Move* selectedMove = activeEnemy->getEquippedMove(selectedSlot);
    if (selectedMove->category == MoveCategory::Defense)
    {
        enemyIsDefending = 1;
        enemyCounterMove = selectedMove;
    }
    if (selectedMove->category == MoveCategory::Attack)
    {
        enemyIsAttacking = 1;
    }
}

//获取当前精灵,const保证函数不会改变量
Codemon* BattleSystem::getActiveCodemon() const 
{
    return activecodemon;
}

//获取敌人精灵,const保证不修改成员变量
Codemon* BattleSystem::getActiveEnemy() const
{
    return activeEnemy;
}

//获取玩家队伍大小,const保证函数不会修改变量
int BattleSystem::getPlayerTeamSize() const
{
    return playerTeam.size();
}

//获取指定索引的敌人精灵
Codemon* BattleSystem::getEnemyCodemon(int index)
{
    if (index >= 0 && index < int(enemyTeam.size()))
    {
        return enemyTeam[index];
    }
    return nullptr;  //没有索引时候返回空
}

//获取敌人队伍大小,const保证函数不会修改变量
int BattleSystem::getEnemyTeamSize() const
{
    return enemyTeam.size();
}

//获取当前敌人索引,const保证函数不会修改变量
int BattleSystem::getActiveEnemyIndex() const
{
    for (int i = 0; i < enemyTeam.size(); i++) 
    {
        if (enemyTeam[i] == activeEnemy) 
        {
            return i;
        }
    }
    return -1; //没有找到
}

//获取战斗结果,const保证函数不会修改变量
BattleResult BattleSystem::getBattleResult() const
{
    return battleResult;
}

//这个是获取战斗日志的函数
const vector<string>& BattleSystem::getBattleLog() const
{
    return battleLog;
}

//这个是清空战斗日志的函数
void BattleSystem::clearBattleLog()
{
    battleLog.clear();
}

//这个是获取玩家对象的函数，返回一个指向玩家对象的指针，供外部调用获取玩家信息使用
Player* BattleSystem::getPlayer() const
{
    return player;
}