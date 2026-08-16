#include "../include/Player.h"
#include "../include/Item.h"
#include "../include/ItemDB.h"
#include "../include/codemon.h"
#include "../include/species.h"
#include "../include/Bag.h"
#include "../include/Room.h"
#include "../include/Map.h"
#include "../include/MapManager.h"
#include "../include/move.h"
#include "../include/moves.h"
#include "../include/Obj.h"
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>

#define WRITE_VAR(var) ofs.write(reinterpret_cast<const char *>(&var), sizeof(var))
#define READ_VAR(var) ifs.read(reinterpret_cast<char *>(&var), sizeof(var))

Player::Player(const std::string &playerName) : name(playerName), gold(100), exp(0) {}

// 仓库管理
void Player::enterStage(const std::string &stageName)
{
    if (reachedStage.find(stageName) == reachedStage.end())
    {
        reachedStage.insert(stageName);
    }
    currentStage = stageName;
}

//添加精灵至仓库
void Player::acquireCodemon(std::unique_ptr<monbase::Codemon> &&codemon)
{
    if (!codemon)
        return;
    //新获得的精灵恢复满状态
    codemon->rest();
    storage.push_back(std::move(codemon));
}

//获取仓库中的精灵
monbase::Codemon *Player::getStorageCodemon(int index)
{
    if (index >= 0 && index < static_cast<int>(storage.size()))
    {
        return storage[index].get();
    }
    return nullptr;
}

//获取仓库大小
size_t Player::getStorageSize() const { return storage.size(); }

//从仓库中选一只上场
bool Player::withdrawToTeam(int storageIndex)
{
    //检查出战队伍是否已满
    if (team.size() >= MAX_TEAM_SIZE)
    {
        std::cout << "出战队伍已满，无法上场！" << std::endl;
        return false;
    }

    //检查索引合法性
    if (storageIndex < 0 || storageIndex >= static_cast<int>(storage.size()))
    {
        return false;
    }

    //移动所有权
    auto &src_ptr = storage[storageIndex];
    src_ptr->rest(); //上场前恢复满状态
    team.push_back(std::move(src_ptr));
    storage.erase(storage.begin() + storageIndex);

    std::cout << team.back()->getName() << " 上场！" << std::endl;
    return true;
}

//将精灵撤退回仓库
bool Player::depositToStorage(int teamIndex)
{
    //检查出战队伍是否为空
    if (team.empty())
        return false;

    //检查索引合法性
    if (teamIndex < 0 || teamIndex >= static_cast<int>(team.size()))
    {
        return false;
    }

    //移动所有权
    auto &src_ptr = team[teamIndex];
    storage.push_back(std::move(src_ptr)); 
    team.erase(team.begin() + teamIndex);  

    std::cout << "精灵撤回仓库。" << std::endl;
    return true;
}

//获取出战精灵
monbase::Codemon *Player::getTeamCodemon(int index)
{
    if (index >= 0 && index < static_cast<int>(team.size()))
    {
        return team[index].get();
    }
    return nullptr;
}

//获取当前默认出战精灵
monbase::Codemon *Player::getActiveCodemon()
{
    return team.empty() ? nullptr : team[0].get();
}

//获取队伍大小
size_t Player::getTeamSize() const { return team.size(); }
bool Player::isTeamFull() const { return team.size() >= MAX_TEAM_SIZE; }

//精灵技能管理
const std::vector<monbase::Move> *Player::getStorageCodemonUnlockedMoves(int storageIndex) const
{
    if (storageIndex < 0 || storageIndex >= static_cast<int>(storage.size()))
    {
        return nullptr;
    }
    return &storage[storageIndex]->getUnlockedMoves();
}

const std::vector<int> *Player::getStorageCodemonEquippedIndices(int storageIndex) const
{
    if (storageIndex < 0 || storageIndex >= static_cast<int>(storage.size()))
    {
        return nullptr;
    }
    return &storage[storageIndex]->getEquippedIndices();
}

std::vector<const monbase::Move *> Player::getStorageCodemonEquippedMoves(int storageIndex) const
{
    std::vector<const monbase::Move *> result;
    if (storageIndex < 0 || storageIndex >= static_cast<int>(storage.size()))
    {
        return result;
    }
    return storage[storageIndex]->getEquippedMoves();
}

bool Player::swapStorageCodemonMove(int storageIndex, int eIdx, int uIdx)
{
    if (storageIndex < 0 || storageIndex >= static_cast<int>(storage.size()))
    {
        std::cout << "无效的仓库索引！" << std::endl;
        return false;
    }

    auto &codemon = storage[storageIndex];
    if (!codemon)
    {
        std::cout << "精灵不存在！" << std::endl;
        return false;
    }

    bool success = codemon->swapEquippedMove(eIdx, uIdx);
    if (success)
    {
        std::cout << codemon->getName() << " 技能切换成功！" << std::endl;
    }
    else
    {
        std::cout << codemon->getName() << " 技能切换失败！" << std::endl;
    }
    return success;
}

int Player::getStorageCodemonEquippedCount(int storageIndex) const
{
    if (storageIndex < 0 || storageIndex >= static_cast<int>(storage.size()))
    {
        return 0;
    }
    return storage[storageIndex]->getEquippedCount();
}

bool Player::isStorageCodemonMovesFull(int storageIndex) const
{
    if (storageIndex < 0 || storageIndex >= static_cast<int>(storage.size()))
    {
        return false;
    }
    return storage[storageIndex]->getEquippedCount() >= 4;
}

//经济系统
bool Player::spendGold(int amount)
{
    if (gold >= amount)
    {
        gold -= amount;
        return true;
    }
    return false;
}
void Player::earnGold(int amount) { gold += amount; }
int Player::getGold() const { return gold; }

//经验
void Player::addExp(int e) { exp += e; }
int Player::getExp() const { return exp; }

//钥匙系统
void Player::addKey(std::unique_ptr<Item> key)
{
    if (key && key->getType() == ItemType::KEY)
    {
        keyStorage.push_back(std::move(key));
    }
}

void Player::SetMapManager(MapManager *mm)
{
    mapManager = mm;
}

MapManager *Player::GetMapManager() const
{
    return mapManager;
}

void Player::useKey()
{
    //获取MapManager
    MapManager *mapMgr = GetMapManager();
    if (!mapMgr)
    {
        std::cout << "地图管理器未连接！" << std::endl;
        return;
    }

    //获取当前地图
    Map *currentMap = mapMgr->GetCurrentMap();
    if (!currentMap)
    {
        std::cout << "当前没有加载的地图！" << std::endl;
        return;
    }

    //获取当前房间
    Room *currentRoom = currentMap->GetCurrentRoom();
    if (!currentRoom)
    {
        std::cout << "当前房间不存在！" << std::endl;
        return;
    }

    std::cout << "尝试使用钥匙开门..." << std::endl;

    //遍历钥匙仓库，尝试开门
    for (auto it = keyStorage.begin(); it != keyStorage.end();)
    {
        if (*it)
        {
            std::string keyName = (*it)->getName();
            bool removed = currentRoom->RemoveBlocksByKey(keyName);

            if (removed)
            {
                std::cout << "障碍物已移除！" << std::endl;
                it = keyStorage.erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            ++it;
        }
    }
    std::cout << "钥匙使用完毕。" << std::endl;
}

bool Player::HasKey(const std::string &keyName) const
{
    for (const auto &key : keyStorage)
    {
        if (key == nullptr)
            continue;

        if (key->getName() == keyName)
        {
            return true;
        }
    }
    return false;
}

void Player::RemoveKey(const std::string &keyName)
{
    for (auto it = keyStorage.begin(); it != keyStorage.end();)
    {
        if (*it == nullptr)
        {
            ++it;
            continue;
        }

        if ((*it)->getName() == keyName)
        {
            it = keyStorage.erase(it);

            std::cout << "已移除钥匙: " << keyName << std::endl;
            return;
        }
        else
        {
            ++it;
        }
    }
    std::cout << "未找到钥匙: " << keyName << std::endl;
}

//战斗系统
bool Player::IsInBattle() const { return state == State::InBattle; }
bool Player::IsDead() const { return state == State::Dead; }

//宝箱奖励
void Player::addChestReward(ChestRewardData &&reward)
{
    //经验
    if (reward.exp > 0)
    {
        addExp(reward.exp);
    }
    //金币
    if (reward.gold > 0)
    {
        earnGold(reward.gold);
    }
    //道具
    for (auto &item : reward.items)
    {
        if (item)
        {
            if (item->getType() == ItemType::KEY)
            {
                keyStorage.push_back(std::move(item));
            }
            else
            {
                bag.addItem(std::move(item));
            }
        }
    }
}

//清空
void Player::clearPlayer()
{
    gold = 0;
    exp = 0;
    position = sf::Vector2f(100.f, 100.f);
    currentMapIndex = 0;
    currentRoomIndex = 0;
    state = State::Idle;
    canMove = true;
    team.clear();
    storage.clear();
    keyStorage.clear();
    bag = Bag();
}

//存档
static void WriteString(std::ofstream &ofs, const std::string &str)
{
    size_t len = str.size();
    WRITE_VAR(len);
    if (len > 0)
        ofs.write(str.c_str(), len);
}

static std::string ReadString(std::ifstream &ifs)
{
    size_t len;
    READ_VAR(len);
    std::string str(len, ' ');
    if (len > 0)
        ifs.read(&str[0], len);
    return str;
}

const monbase::Move *Player::findMoveByName(const std::string &name)
{
    using namespace monbase::moves;
    //映射所有技能名字到指针
    static const std::map<std::string, const monbase::Move *> moveMap = {
        {"抓挠", &Scratch}, {"扫尾", &TailSweep}, {"猛烈撞击", &PowerTackle}, {"火苗", &Ember}, {"火焰切割", &FlameSlash}, {"吹火", &FireBreath}, {"火云车", &FireChariot}, {"山火", &Wildfire}, {"筛管奔流", &SieveTubeRush}, {"仙人掌刺击", &CactusStrike}, {"棘突", &ThornSpike}, {"藤绞", &VineWring}, {"叶绿光束", &ChloroBeam}, {"光能聚集", &LightGather}, {"甩水", &WaterFlick}, {"气泡", &Bubble}, {"水炮", &WaterCannon}, {"涌泉", &Spring}, {"天洪", &Flood}, {"水波术", &WaterWave}, {"拍击", &Slap}, {"防御", &Defend}, {"休息回复", &RestRecover}, {"徒长", &GrowthSpurt}, {"丰饶", &Abundance}, {"力量增效", &PowerBoost}, {"晒太阳", &Sunbathe}, {"怒火", &Rage}, {"魔法增效", &MagicBoost}, {"洗礼", &Purification}, {"水泡盾", &WaterShield}, {"气波", &AirWave}, {"泡沫", &Foam}, {"蓄水", &StoreWater}, {"防御反击", &DefendCounter}, {"化劲", &PowerFocus}, {"水幕冲击", &WaterScreenStrike}, {"听桥", &ListenBridge}, {"一拳", &OnePunch}, {"斩断", &Sever}, {"水刃", &WaterBlade}, {"鸣叫", &Chirp}, {"飞羽", &FlyingFeather}, {"潮涌", &TideSurge}, {"风矢", &WindArrow}, {"翼击", &WingStrike}, {"龙卷风", &Tornado}, {"冰晶坠", &IceCrystalFall}, {"暴风雪", &BlizzardIce}, {"破罐破摔", &ShatterPot}, {"碎冰冰", &CrushedIce}, {"风吹雪", &WindSnow}, {"当头棒喝", &BluntStrike}, {"冰墙", &IceWall}, {"生日蛋糕", &BirthdayCake}, {"冬至", &WinterSolstice}, {"冰点", &FreezingPoint}, {"霜降", &FrostFall}, {"甜心续航", &SweetEndurance}, {"雾气环绕", &FogSurround}, {"反击拳", &CounterPunch}, {"先发制人", &PreemptiveStrike}, {"截拳", &JeetKuneDo}, {"影袭", &ShadowStrike}, {"回旋踢", &RoundhouseKick}, {"飞踢", &FlyingKick}, {"突袭", &SneakAttack}, {"许愿星", &WishingStar}, {"提气", &SpiritBoost}, {"主场优势", &HomeFieldAdvantage}, {"应激反应", &StressResponse}, {"光球", &LightOrb}, {"天光", &SkyLight}, {"虹光冲击", &RainbowImpact}, {"光之矛", &LightSpear}, {"过曝", &Overexposure}, {"雷暴", &Thunderstorm}, {"导电撞击", &ConductiveTackle}, {"超导", &Superconduct}, {"离子火花", &IonSpark}, {"强制重启", &ForceRestart}, {"漫反射", &DiffuseReflection}, {"恐吓", &Scare}, {"午夜噪音", &MidnightNoise}, {"幽灵爆发", &GhostBurst}, {"背袭", &BackStrike}, {"勾魂", &SoulHook}, {"恶作剧", &Mischief}, {"操控", &Control}, {"热身运动", &WarmUp}, {"魔爪", &DarkClaw}, {"蝙蝠", &BatStrike}, {"暗突袭", &DarkAmbush}, {"鸣沙陷阱", &SandTrap}, {"岩土暴击", &RockCrit}, {"砂石冲撞", &SandRush}, {"地震", &Earthquake}, {"钧势", &BalanceStance}, {"力量吞噬", &PowerDevour}, {"泥浆铠甲", &MudArmor}, {"诡刺", &GhostStab}, {"坟场搏击", &GraveyardFight}, {"龙吼", &DragonRoar}, {"幻象", &Illusion}, {"隼鳞", &HawkScale}, {"龙之利爪", &DragonClaw}, {"吹炎", &FlameBreath}, {"虚化", &Vanish}, {"架势", &Stance}, {"龙威", &DragonAwe}, {"电弧", &ArcStrike}, {"交叉闪电", &CrossLightning}, {"水花四溅", &WaterSplash}, {"双联脉冲", &DualPulse}, {"水光冲击", &HydroImpact}, {"集中", &Focus}, {"电磁偏转", &EMDeflect}, {"电离爆破", &IonBlast}, {"腐蚀酸液", &CorrosiveAcid}, {"瘴气喷射", &MiasmaShot}, {"激流", &Torrent}, {"湮灭", &Annihilation}, {"毒液渗透", &VenomPenetration}, {"鸩毒", &PoisonVenom}, {"不可接触", &Untouchable}, {"以毒攻毒", &VenomCure}, {"疫病吐息", &PlagueBreath}, {"音爆", &SonicBoom}, {"潮汐", &Tide}, {"水环", &WaterRing}, {"叠势", &StackingMomentum}, {"打鼾", &Snore}, {"破防", &BreakDefense}, {"打湿", &Wet}, {"预备势", &ReadyStance}, {"鼓劲", &Cheer}, {"清洗", &Wash}, {"啄击", &Peck}, {"疾风刺", &GustStab}, {"见招拆招", &Parry}, {"扇风", &FanWind}, {"羽刃", &FeatherBlade}, {"乘风", &RideWind}, {"羽化加速", &FeatherBoost}, {"三连破", &TripleBreak}, {"快速移动", &QuickMove}, {"炎息", &FlameBreathLight}, {"灼伤", &BurnStrike}, {"闪燃", &FlashBurn}, {"持续高温", &HeatWave}, {"焚烧烙印", &BurnBrand}, {"充分燃烧", &FullBurn}, {"热身", &WarmUpFire}};
    auto it = moveMap.find(name);
    return it != moveMap.end() ? it->second : nullptr;
}

void Player::SaveCodemon(std::ofstream &ofs, const monbase::Codemon *codemon)
{
    if (!codemon)
        return;

    struct CodemonAccessor : public monbase::Codemon
    {
        static const auto &getSpeciesPool(const monbase::Codemon *c)
        {
            return static_cast<const CodemonAccessor *>(c)->speciesMovePool;
        }
    };

    //ID
    WriteString(ofs, codemon->getName());
    int codemonId = codemon->getCodemonId();
    WRITE_VAR(codemonId);

    //等级&经验
    int level = codemon->getLevel();
    int exp = codemon->getExperience();
    WRITE_VAR(level);
    WRITE_VAR(exp);

    //基础属性
    monbase::Stats baseStats = codemon->getBaseStats();
    WRITE_VAR(baseStats.hp);
    WRITE_VAR(baseStats.attack);
    WRITE_VAR(baseStats.defense);
    WRITE_VAR(baseStats.magicAttack);
    WRITE_VAR(baseStats.magicDefense);
    WRITE_VAR(baseStats.speed);

    //当前状态
    int curHP = codemon->getHP();
    int curEnergy = codemon->getEnergy();
    WRITE_VAR(curHP);
    WRITE_VAR(curEnergy);

    //基础元素
    monbase::Element type1 = codemon->getType1();
    monbase::Element type2 = codemon->getType2();
    WRITE_VAR(type1);
    WRITE_VAR(type2);

    //进化阶段
    int evoStage = codemon->getEvolutionStage();
    WRITE_VAR(evoStage);

    //已解锁技能
    const auto &unlockedMoves = codemon->getUnlockedMoves();
    size_t moveCount = unlockedMoves.size();
    WRITE_VAR(moveCount);
    for (const auto &move : unlockedMoves)
    {
        WriteString(ofs, move.name);
    }

    //已装备技能保存索引
    const auto &equippedIndices = codemon->getEquippedIndices();
    size_t idxCount = equippedIndices.size();
    WRITE_VAR(idxCount);
    for (int idx : equippedIndices)
    {
        WRITE_VAR(idx);
    }

    //装备栏
    const auto &equippedItems = codemon->getEquippedItems();
    size_t eqSize = equippedItems.size();
    WRITE_VAR(eqSize);
    for (const auto &pair : equippedItems)
    {
        int slot = static_cast<int>(pair.first);
        int itemId = static_cast<int>(pair.second);
        WRITE_VAR(slot);
        WRITE_VAR(itemId);
    }

    //物种技能池
    const auto &speciesPool = CodemonAccessor::getSpeciesPool(codemon);
    size_t poolSize = speciesPool.size();
    WRITE_VAR(poolSize);
    for (const auto &entry : speciesPool)
    {
        int learnLvl = entry.learnLevel;
        WRITE_VAR(learnLvl);
        WriteString(ofs, entry.move.name);
    }
}

std::unique_ptr<monbase::Codemon> Player::LoadCodemon(std::ifstream &ifs)
{
    //ID
    std::string name = ReadString(ifs);
    int codemonId;
    READ_VAR(codemonId);

    //使用通用的保存/加载实例类以还原存档数据
    class SavedCodemon : public monbase::Codemon
    {
    public:
        SavedCodemon(const std::string &n, monbase::Element t1, const monbase::Stats &base, int id)
            : monbase::Codemon(n, t1, base, 1)
        {
            this->codemonId = id;
        }
        virtual std::unique_ptr<monbase::Codemon> clone() const override
        {
            return std::make_unique<SavedCodemon>(*this);
        }
        //用于读档后恢复物种技能池
        void addToSpeciesPoolOnly(int lvl, const monbase::Move &move)
        {
            speciesMovePool.push_back({lvl, move});
        }
        //从存档恢复基础种族值
        void setBaseStats(const monbase::Stats &s)
        {
            baseStats = s;
            calculateActualStats();
        }
        //从存档恢复属性类型
        void setTypes(monbase::Element t1, monbase::Element t2)
        {
            type1 = t1;
            type2 = t2;
        }
        //从存档恢复总经验值并重算等级
        void setTotalExperience(int exp)
        {
            experience = exp;
            level = 1;
            while (level < 100 && experience >= xpForLevel(level + 1))
            {
                ++level;
            }
            calculateActualStats();
        }
        //从存档恢复进化阶段
        void setEvolutionStage(int stage)
        {
            evolutionStage = stage;
        }
    };

    std::unique_ptr<monbase::Codemon> codemon = std::make_unique<SavedCodemon>(name, monbase::Element::None, monbase::Stats{}, codemonId);
    auto *savedPtr = static_cast<SavedCodemon *>(codemon.get());

    //等级&经验
    int level;
    int exp;
    READ_VAR(level);
    READ_VAR(exp);

    //基础属性与元素
    monbase::Stats dummyStats;
    READ_VAR(dummyStats.hp);
    READ_VAR(dummyStats.attack);
    READ_VAR(dummyStats.defense);
    READ_VAR(dummyStats.magicAttack);
    READ_VAR(dummyStats.magicDefense);
    READ_VAR(dummyStats.speed);

    //当前状态
    int curHP;
    int curEnergy;
    READ_VAR(curHP);
    READ_VAR(curEnergy);

    monbase::Element dummyType1, dummyType2;
    READ_VAR(dummyType1);
    READ_VAR(dummyType2);

    //清空构造时的默认技能与索引，按存档严格恢复
    codemon->clearUnlockedMoves();
    //恢复基础属性与元素类型
    savedPtr->setBaseStats(dummyStats);
    savedPtr->setTypes(dummyType1, dummyType2);

    //将存档中的总经验值直接设置到实例上，重算等级与属性
    if (exp > 0)
        savedPtr->setTotalExperience(exp);

    //恢复当前 HP / 能量
    codemon->setHP(curHP);
    codemon->setEnergy(curEnergy);

    //进化阶段
    int evoStage;
    READ_VAR(evoStage);

    //已解锁技能
    size_t moveCount;
    READ_VAR(moveCount);
    std::vector<std::string> savedMoveNames;
    savedMoveNames.reserve(moveCount);
    for (size_t i = 0; i < moveCount; ++i)
    {
        savedMoveNames.push_back(ReadString(ifs));
    }

    //恢复已解锁技能
    for (size_t i = 0; i < moveCount; ++i)
    {
        const monbase::Move *move = findMoveByName(savedMoveNames[i]);
        if (move && !move->name.empty())
        {
            codemon->addUnlockedMoveWithoutEquip(*move);
        }
        else
        {
            std::cout << "[Warning] LoadCodemon: Invalid skill '" << savedMoveNames[i] << "' skipped." << std::endl;
        }
    }

    //装备的技能索引
    size_t idxCount;
    READ_VAR(idxCount);
    for (size_t i = 0; i < idxCount; ++i)
    {
        int origIdx;
        READ_VAR(origIdx);

        if (origIdx < 0 || origIdx >= static_cast<int>(savedMoveNames.size()))
        {
            std::cout << "[Warning] LoadCodemon: Skill index " << origIdx << " out of range." << std::endl;
            continue;
        }

        const std::string &targetName = savedMoveNames[origIdx];
        const auto &unlockedMoves = codemon->getUnlockedMoves();
        bool found = false;
        for (int j = 0; j < static_cast<int>(unlockedMoves.size()); ++j)
        {
            if (unlockedMoves[j].name == targetName)
            {
                codemon->addEquippedIndex(j);
                found = true;
                break;
            }
        }
        if (!found)
        {
            std::cout << "[Warning] LoadCodemon: Equipped skill '" << targetName << "' not found." << std::endl;
        }
    }

    //装备栏
    size_t eqCount;
    READ_VAR(eqCount);
    for (size_t i = 0; i < eqCount; ++i)
    {
        int slotInt;
        int itemIdInt;
        READ_VAR(slotInt);
        READ_VAR(itemIdInt);
        EquipSlot slot = static_cast<EquipSlot>(slotInt);
        ItemID itemId = static_cast<ItemID>(itemIdInt);
        auto item = ItemDB::getItem(itemId);
        if (item)
            item->equip(codemon.get());
    }

    savedPtr->setEvolutionStage(evoStage);

    //物种技能池恢复
    size_t poolSize;
    READ_VAR(poolSize);
    for (size_t i = 0; i < poolSize; ++i)
    {
        int learnLvl;
        READ_VAR(learnLvl);
        std::string poolMoveName = ReadString(ifs);
        const monbase::Move *poolMove = findMoveByName(poolMoveName);
        if (poolMove)
        {
            savedPtr->addToSpeciesPoolOnly(learnLvl, *poolMove);
        }
    }

    return codemon;
}

void Player::SaveBag(std::ofstream &ofs)
{
    const auto &inv = bag.getInventory();
    size_t invSize = inv.size();
    WRITE_VAR(invSize);
    for (const auto &pair : inv)
    {
        int itemId = static_cast<int>(pair.first);
        int count = pair.second.second;
        WRITE_VAR(itemId);
        WRITE_VAR(count);
    }
}

void Player::LoadBag(std::ifstream &ifs)
{
    size_t invSize;
    READ_VAR(invSize);
    for (size_t i = 0; i < invSize; ++i)
    {
        int itemIdInt;
        int count;
        READ_VAR(itemIdInt);
        READ_VAR(count);
        ItemID itemId = static_cast<ItemID>(itemIdInt);
        auto item = ItemDB::getItem(itemId);
        if (item)
            bag.addItem(std::move(item), count);
    }
}

void Player::SaveKeys(std::ofstream &ofs)
{
    size_t keyCount = keyStorage.size();
    WRITE_VAR(keyCount);
    for (const auto &key : keyStorage)
    {
        if (key)
        {
            int itemId = static_cast<int>(key->getID());
            WRITE_VAR(itemId);
        }
    }
}

void Player::LoadKeys(std::ifstream &ifs)
{
    size_t keyCount;
    READ_VAR(keyCount);
    keyStorage.clear();
    for (size_t i = 0; i < keyCount; ++i)
    {
        int itemIdInt;
        READ_VAR(itemIdInt);
        ItemID itemId = static_cast<ItemID>(itemIdInt);
        auto key = ItemDB::getItem(itemId);
        if (key && key->getType() == ItemType::KEY)
        {
            keyStorage.push_back(std::move(key));
        }
    }
}

bool Player::SaveToFile(const std::string &filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open())
    {
        std::cerr << "无法打开存档：" << filename << std::endl;
        return false;
    }

    try
    {
        //玩家基础信息
        WriteString(ofs, name);
        WRITE_VAR(gold);
        WRITE_VAR(exp);
        WriteString(ofs, currentStage);

        //地图进度
        size_t mapCount = reachedStage.size();
        WRITE_VAR(mapCount);
        for (const auto &stage : reachedStage)
        {
            WriteString(ofs, stage);
        }

        //玩家位置
        float posX = position.x;
        float posY = position.y;
        WRITE_VAR(posX);
        WRITE_VAR(posY);

        //地图和房间引索
        WRITE_VAR(currentMapIndex);
        WRITE_VAR(currentRoomIndex);

        //枚举状态
        int playerState = static_cast<int>(state);
        WRITE_VAR(playerState);

        //移动许可
        WRITE_VAR(canMove);

        //背包
        SaveBag(ofs);

        //精灵仓库
        size_t storageSize = storage.size();
        WRITE_VAR(storageSize);
        for (const auto &codemon : storage)
        {
            SaveCodemon(ofs, codemon.get());
        }

        //出战队伍
        size_t teamSize = team.size();
        WRITE_VAR(teamSize);
        for (const auto &codemon : team)
        {
            SaveCodemon(ofs, codemon.get());
        }

        //钥匙仓库
        SaveKeys(ofs);
    }
    catch (const std::exception &e)
    {
        std::cerr << "存档写入失败：" << e.what() << std::endl;
        ofs.close();
        return false;
    }

    ofs.close();
    return true;
}

bool Player::LoadFromFile(const std::string &filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs.is_open())
    {
        std::cerr << "无法打开文件：" << filename << std::endl;
        return false;
    }

    try
    {
        clearPlayer();

        //基础信息
        name = ReadString(ifs);
        READ_VAR(gold);
        READ_VAR(exp);
        currentStage = ReadString(ifs);

        //地图进度
        size_t mapCount;
        READ_VAR(mapCount);
        reachedStage.clear();
        for (size_t i = 0; i < mapCount; ++i)
        {
            std::string stage = ReadString(ifs);
            reachedStage.insert(stage);
        }

        //玩家位置
        float posX;
        float posY;
        READ_VAR(posX);
        READ_VAR(posY);
        position.x = posX;
        position.y = posY;

        //地图和房间引索
        READ_VAR(currentMapIndex);
        READ_VAR(currentRoomIndex);

        if (mapManager)
        {
            mapManager->LoadMap(currentMapIndex);
            if (auto map = mapManager->GetCurrentMap())
            {
                map->SetCurrentRoom(currentRoomIndex);
            }
        }

        //枚举状态
        int playerStateInt;
        READ_VAR(playerStateInt);
        state = static_cast<State>(playerStateInt);

        //移动许可
        READ_VAR(canMove);

        //背包
        bag = Bag();
        LoadBag(ifs);

        //精灵仓库
        size_t storageSize;
        READ_VAR(storageSize);
        storage.clear();
        for (size_t i = 0; i < storageSize; ++i)
        {
            auto codemon = LoadCodemon(ifs);
            if (codemon)
                storage.push_back(std::move(codemon));
        }

        //出战队伍
        size_t teamSize;
        READ_VAR(teamSize);
        team.clear();
        for (size_t i = 0; i < teamSize; ++i)
        {
            auto codemon = LoadCodemon(ifs);
            if (codemon)
                team.push_back(std::move(codemon));
        }

        //钥匙
        keyStorage.clear();
        LoadKeys(ifs);
    }
    catch (const std::exception &e)
    {
        std::cerr << "存档读取失败：" << e.what() << std::endl;
        ifs.close();
        return false;
    }

    ifs.close();
    return true;
}