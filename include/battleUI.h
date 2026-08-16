// BattleSystem.h
//前言
//使用unordered_map来存储属性克制关系，因为它提供了平均常数时间复杂度的查找，非常适合我们需要频繁查询属性克制关系的场景
//我们的属性克制有很多很多种组合，如果用二维数组或者二维vector会比较麻烦，unordered_map可以让我们直接通过属性枚举值来快速获取克制关系和伤害倍率
//使用SFML的Clock来管理动画和回合时间限制
//我们还用了unique_ptr来管理精灵对象的内存，确保在不再需要时自动释放，避免内存泄漏,这样可以少思考内存的东西，不是偷懒
//引入enum class来定义战斗状态和战斗结果
//对变量和函数的命名我们尽量保证清晰和一致，使用驼峰命名法来区分变量和函数，函数名通常是动词开头，变量名则更倾向于描述性名词
#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/System.hpp>
#include "../include/codemon.h"
#include "../include/Player.h"
using namespace std;
using namespace monbase;//这个是codemon里面我们自己做的命名空间
using namespace sf;//这个是SFML的命名空间，主要是为了使用Clock类
//这里是战斗系统的5个状态：玩家回合、敌人回合、动画播放中、等待切换精灵、战斗结束
enum class BattleState 
{
    PLAYER_TURN,
    ENEMY_TURN,
	ANIMATING,//这里提一嘴，虽然说是动画播放时间，但其实我们没有具体的动画画面，只是当作一个动画时间过渡，因为实在是找不到那么多播放动画的资源了，可惜
    WAITING_FOR_SWITCH,
    BATTLE_END
};
//这里是战斗系统的3种结果：无结果、玩家胜利、玩家失败
enum class BattleResult 
{
    NONE,
    PLAYER_WIN,
    PLAYER_LOSE,
};
//前向声明
class battleUI;
//基础定义类
class BattleSystemBase 
{
protected:
    Player* player;//玩家的指针，指向玩家对象
    BattleState currentState;//定义当前战斗状态
    BattleResult battleResult;//定义战斗结果
public:
    BattleSystemBase(Player* player) : player(player), currentState(BattleState::PLAYER_TURN), battleResult(BattleResult::NONE) {}
    virtual ~BattleSystemBase() = default;
    //基础接口
    virtual BattleState getState() const = 0;//这个是获取当前战斗状态的函数，返回一个 BattleState 枚举值，供外部调用判断当前状态使用
    virtual BattleResult getBattleResult() const = 0;//返回战斗结果的函数，返回一个 BattleResult 枚举值，供外部调用判断战斗结果使用
    virtual Player* getPlayer() const = 0; //这个是获取玩家对象的函数，返回一个指向玩家对象的指针，供外部调用获取玩家信息使用
};

//UI相关类
class BattleSystemUI 
{
protected:
    Clock animationClock;//这个是动画播放的时间定义
    float animationDuration;//这个是动画播放时间的具体数字
    battleUI* ui;//UI指针
    vector<string> battleLog;//这个是战斗日志记录用的，就是记录都用了啥技能，切换上场啥精灵而已
public:
    BattleSystemUI() : animationDuration(1.0f), ui(nullptr) {}
    virtual ~BattleSystemUI() = default;
    // UI相关接口
    virtual void setUI(battleUI* battleUI) = 0;
    virtual const vector<string>& getBattleLog() const = 0;
    virtual void clearBattleLog() = 0;
};

//对战实现类
class BattleSystemCombat 
{
protected:
    //队伍管理
    vector<Codemon*> playerTeam;//定义玩家队伍，使用unique_ptr管理精灵对象，确保在不再需要时自动释放。
    vector<Codemon*> enemyTeam;//定义敌人队伍，使用unique_ptr管理精灵对象，确保在不再需要时自动释放。
    Codemon* activecodemon;//当前玩家精灵的指针
    Codemon* activeEnemy;//当前敌人精灵的指针

    //时间管理
    Clock playerTurnClock;//这个是玩家回合时间限制的定义
    float turnTimeLimit = 30.0f;//这个是玩家回合时间限制的具体数字，单位是秒

    // 属性克制系统
    unordered_map<Element, unordered_map<Element, float>> typeChart;//这个是属性克制表
    // 防守反击系统
    bool playerIsDefending = false;//记录玩家是否防御
    bool playerIsAttacking = false;//记录玩家是否在攻击
    const Move* playerCounterMove = nullptr;//玩家使用的防御反击技能
    bool enemyIsDefending = false;//记录敌人是否处于防御反击状态
    bool enemyIsAttacking = false;//记录敌人是否在攻击
    const Move* enemyCounterMove = nullptr;//敌人使用的防御反击技能
    int enemySelectedMoveIndex = -1;//存储敌人选择的技能索引

    //私有战斗逻辑
	virtual void calculateDamage(Codemon* attacker, Codemon* defender, const Move& move) = 0;//这里是计算伤害的函数，接受攻击者、被攻击者和技能作为参数，负责去计算伤害值，并考虑各种因素（属性克制、状态效果等）
	virtual void applyStatusEffects(Codemon* target, const Move& move) = 0;//这里是应用状态效果的函数，接受目标和技能作为参数，负责根据技能的效果来修改目标的状态（中毒、灼烧、冻伤）
	virtual void checkFaint() = 0;//这里是检查精灵是否濒死的函数，负责检测当前出战的玩家和敌人精灵是否HP降到0或以下，并根据情况切换精灵或结束战斗
	virtual void switchToNextAlive(Codemon*& current, vector<Codemon*>& team) = 0;//这里是切换到下一只存活精灵的函数，接受当前精灵指针和队伍向量作为参数
	virtual void initTypeChart() = 0;//这里是初始化属性克制表的函数，负责填充 typeChart 这个 unordered_map，定义不同属性之间的克制关系和伤害倍率
	virtual float calculateTypeMultiplier(Element skillElement, const vector<Element>& targetElements) = 0;//这个是计算属性克制倍率的函数，接受技能属性和目标属性列表作为参数，返回一个float类型的值，表示最终的伤害倍率，供计算伤害时使用
	virtual void enemyAI_selectOnly() = 0;//这里是敌人AI选择技能的函数，负责在敌人回合时根据当前战斗情况来选择行动（攻击、技能、道具、切换精灵等），但不执行相应的逻辑，供外部调用来分步骤执行敌人行动使用
	virtual void enemyAI_executeSelected() = 0;//这里是敌人AI执行已选择技能的函数，负责在敌人回合时执行之前选择的行动（攻击、技能、道具、切换精灵等），并执行相应的逻辑，供外部调用来分步骤执行敌人行动使用
	virtual void playerSwitchCodemon(int index) = 0;//这里是玩家切换精灵的函数，接受一个整数类型的index参数，表示玩家选择切换到队伍中哪个索引的精灵，负责执行切换逻辑
public:
    BattleSystemCombat() = default;
    virtual ~BattleSystemCombat() = default;
    //对战相关接口
	virtual void update() = 0;//这个是更新函数，负责根据当前战斗状态来更新游戏逻辑
	virtual void executePlayerAction(int actionType, int targetIndex = 0) = 0;//这个是执行玩家行动的函数，接受一个整数类型的actionType参数，表示玩家选择的行动类型（攻击、技能、道具、切换精灵等），以及一个可选的targetIndex参数，表示目标索引（攻击或技能时指定敌人索引，切换精灵时指定队伍索引），负责根据玩家选择来执行相应的逻辑
	virtual float getRemainingTime() const = 0;//这个是获取玩家回合剩余时间的函数，返回一个 float 类型的值，表示玩家回合还剩多少秒，供外部调用显示倒计时使用
	virtual Codemon* getPlayerCodemon(int index) = 0;//获取指定索引的玩家精灵
	virtual Codemon* getActiveCodemon() const = 0;//获取当前精灵,const保证函数不会改变量
	virtual Codemon* getActiveEnemy() const = 0;//获取敌人精灵,const保证不修改成员变量
	virtual int getPlayerTeamSize() const = 0;//获取玩家队伍大小,const保证函数不会修改变量
	virtual int getEnemyTeamSize() const = 0;//获取敌人队伍大小,const保证函数不会修改变量
	virtual Codemon* getEnemyCodemon(int index) = 0;//获取指定索引的敌人精灵
	virtual int getActiveEnemyIndex() const = 0;//获取当前敌人精灵的索引
};

//具体实现类
class BattleSystem : public BattleSystemBase, public BattleSystemUI, public BattleSystemCombat
{
	friend class battleUI; //让battleUI 成为友元类，以便访问私有成员
private:
    //战斗逻辑实现
    void calculateDamage(Codemon* attacker, Codemon* defender, const Move& move);
    void applyStatusEffects(Codemon* target, const Move& move);
    void checkFaint();
    void switchToNextAlive(Codemon*& current, vector<Codemon*>& team);
    void initTypeChart();
    float calculateTypeMultiplier(Element skillElement, const vector<Element>& targetElements);
    void enemyAI_selectOnly();
    void enemyAI_executeSelected();
    void playerSwitchCodemon(int index);
public:
    BattleSystem(Player* player, vector<unique_ptr<Codemon>>& enemyTeam);
    //实现所有函数
    BattleState getState() const;
    BattleResult getBattleResult() const;
    void setUI(battleUI* battleUI);
    const vector<string>& getBattleLog() const;
    void clearBattleLog();
    void update();
    void executePlayerAction(int actionType, int targetIndex = 0);
    float getRemainingTime() const;
    Codemon* getPlayerCodemon(int index);
    Codemon* getActiveCodemon() const;
    Codemon* getActiveEnemy() const;
    int getPlayerTeamSize() const;
    int getEnemyTeamSize() const;
    Codemon* getEnemyCodemon(int index);
    int getActiveEnemyIndex() const;
    Player* getPlayer() const;

};


