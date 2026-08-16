#pragma once
#include "codemon.h"
#include "moves.h"
#include <set>

class Jimi : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Jimi(int lvl = 1)
        : Codemon("吉米", monbase::Element::Grass,
                  {65, 66, 49, 66, 91, 33}, lvl)
    {
        using namespace monbase::moves;
        addEvolution(18, "耄耋",
                     {86, 87, 65, 87, 121, 44}, monbase::Element::Grass);
        addEvolution(36, "胖猫",
                     {108, 109, 81, 109, 151, 55}, monbase::Element::Grass);

        addMoveToSpeciesPool(1,  Scratch);
        addMoveToSpeciesPool(1,  RestRecover);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(1,  TailSweep);
        addMoveToSpeciesPool(5,  ThornSpike);
        addMoveToSpeciesPool(8,  GrowthSpurt);
        addMoveToSpeciesPool(12, SieveTubeRush);
        addMoveToSpeciesPool(16, VineWring);
        addMoveToSpeciesPool(20, Abundance);
        addMoveToSpeciesPool(25, ChloroBeam);
        addMoveToSpeciesPool(30, LightGather);
        addMoveToSpeciesPool(35, CactusStrike);

        checkEvolution();
        codemonId = 1;
        if (evolutionStage >= 1) codemonId = 2;
        if (evolutionStage >= 2) codemonId = 3;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Jimi>(*this);
    }
protected:
    void onMoveUsed(const monbase::Move& move) override {
        if (move.element == monbase::Element::Grass)
            heal(currentStats.hp * 10 / 100);
    }
};

class Huohua : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Huohua(int lvl = 1)
        : Codemon("火花", monbase::Element::Fire,
                  {70, 84, 56, 37, 43, 78}, lvl)
    {
        using namespace monbase::moves;
        addEvolution(18, "焰火", {93, 111, 75, 49, 58, 109}, monbase::Element::Fire);
        addEvolution(36, "火神", {117, 139, 94, 61, 72, 130}, monbase::Element::Fire);

        addMoveToSpeciesPool(1,  PowerTackle);
        addMoveToSpeciesPool(1,  Ember);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  Sunbathe);
        addMoveToSpeciesPool(8,  PowerBoost);
        addMoveToSpeciesPool(12, FlameSlash);
        addMoveToSpeciesPool(16, FireBreath);
        addMoveToSpeciesPool(22, Rage);
        addMoveToSpeciesPool(28, Wildfire);
        addMoveToSpeciesPool(34, FireChariot);

        checkEvolution();
        codemonId = 4;
        if (evolutionStage >= 1) codemonId = 5;
        if (evolutionStage >= 2) codemonId = 6;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Huohua>(*this);
    }

protected:
    void onMoveUsed(const monbase::Move& move) override {
        if (move.element == monbase::Element::Fire) {
            modifyStatStage(monbase::MoveEffect::BuffAttack, 2);
            modifyStatStage(monbase::MoveEffect::BuffMagicAttack, 2);
        }
    }
};

class Shuilanlan : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Shuilanlan(int lvl = 1)
        : Codemon("水蓝蓝", monbase::Element::Water,
                  {75, 35, 56, 76, 79, 51}, lvl)
    {
        using namespace monbase::moves;
        addEvolution(18, "波波拉", {100, 46, 75, 102, 106, 68}, monbase::Element::Water);
        addEvolution(36, "水灵", {125, 58, 94, 127, 132, 85}, monbase::Element::Water);

        addMoveToSpeciesPool(1,  Slap);
        addMoveToSpeciesPool(1,  WaterFlick);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  MagicBoost);
        addMoveToSpeciesPool(8,  Bubble);
        addMoveToSpeciesPool(12, Purification);
        addMoveToSpeciesPool(18, WaterCannon);
        addMoveToSpeciesPool(24, Spring);
        addMoveToSpeciesPool(30, WaterWave);
        addMoveToSpeciesPool(36, Flood);

        checkEvolution();
        codemonId = 7;
        if (evolutionStage >= 1) codemonId = 8;
        if (evolutionStage >= 2) codemonId = 9;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Shuilanlan>(*this);
    }
protected:
    void onMoveUsed(const monbase::Move& move) override {
        if (move.element == monbase::Element::Water) {
            for (auto& m : unlockedMoves)
                moveCostReduce[m.name] += 1;
        }
    }
};

class Haibao : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Haibao(int lvl = 1)
        : Codemon("海豹战士", monbase::Element::Fighting,
                  {72, 91, 89, 87, 62, 80}, lvl)
    {
        using namespace monbase::moves;
        type2 = monbase::Element::Water;

        addEvolution(20, "海豹船长",
                     {90, 113, 111, 109, 77, 100},
                     monbase::Element::Fighting, monbase::Element::Water);

        addMoveToSpeciesPool(1,  Foam);
        addMoveToSpeciesPool(1,  AirWave);
        addMoveToSpeciesPool(1,  StoreWater);
        addMoveToSpeciesPool(5,  DefendCounter);
        addMoveToSpeciesPool(8,  MagicBoost);
        addMoveToSpeciesPool(12, WaterShield);
        addMoveToSpeciesPool(16, WaterScreenStrike);
        addMoveToSpeciesPool(20, PowerFocus);
        addMoveToSpeciesPool(20, Bubble);
        addMoveToSpeciesPool(24, WaterBlade);
        addMoveToSpeciesPool(28, ListenBridge);
        addMoveToSpeciesPool(32, Sever);
        addMoveToSpeciesPool(36, PowerBoost);
        addMoveToSpeciesPool(40, OnePunch);

        checkEvolution();
        codemonId = 10;
        if (evolutionStage >= 1) codemonId = 11;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Haibao>(*this);
    }
protected:
    int teamCounters = 0;

    void onSwitchIn(int count) override {
        teamCounters = count;
    }

    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        if (move.category == monbase::MoveCategory::Attack && teamCounters > 0)
            return static_cast<int>(move.power * 0.2f * teamCounters);
        return monbase::Codemon::onModifyPower(move, target);
    }
};

// 乖乖鹄进化链

class Guaiguaihu : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Guaiguaihu(int lvl = 1)
        : Codemon("乖乖鹄", monbase::Element::Flying,
                  {75, 57, 83, 52, 58, 69}, lvl)
    {
        using namespace monbase::moves;
        type2 = monbase::Element::Water;

        addEvolution(18, "蓝珠天鹅",
                     {100, 76, 110, 69, 78, 92},
                     monbase::Element::Flying, monbase::Element::Water);
        addEvolution(36, "翠顶夫人",
                     {125, 95, 138, 86, 97, 115},
                     monbase::Element::Flying, monbase::Element::Water);

        addMoveToSpeciesPool(1,  Chirp);
        addMoveToSpeciesPool(1,  TideSurge);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  FlyingFeather);
        addMoveToSpeciesPool(8,  Foam);
        addMoveToSpeciesPool(12, StoreWater);
        addMoveToSpeciesPool(16, WaterShield);
        addMoveToSpeciesPool(18, WindArrow);
        addMoveToSpeciesPool(22, MagicBoost);
        addMoveToSpeciesPool(26, WaterScreenStrike);
        addMoveToSpeciesPool(30, WingStrike);
        addMoveToSpeciesPool(34, Bubble);
        addMoveToSpeciesPool(36, PowerFocus);
        addMoveToSpeciesPool(40, ListenBridge);
        addMoveToSpeciesPool(44, OnePunch);
        addMoveToSpeciesPool(48, Tornado);
        addMoveToSpeciesPool(50, PowerBoost);

        checkEvolution();
        codemonId = 12;
        if (evolutionStage >= 1) codemonId = 13;
        if (evolutionStage >= 2) codemonId = 14;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Guaiguaihu>(*this);
    }
protected:
    void onSwitchOut(monbase::Codemon* nextAlly) override {
        if (nextAlly) {
            nextAlly->modifyStatStage(monbase::MoveEffect::BuffAttack, 5);
            nextAlly->modifyStatStage(monbase::MoveEffect::BuffMagicAttack, 5);
        }
    }
};

// 大耳帽兜进化链

class DaerMaodou : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    DaerMaodou(int lvl = 1)
        : Codemon("大耳帽兜", monbase::Element::Ice,
                  {78, 62, 39, 59, 78, 54}, lvl)
    {
        using namespace monbase::moves;
        addEvolution(18, "帽兜娃娃",
                     {104, 83, 52, 78, 104, 72}, monbase::Element::Ice);
        addEvolution(36, "雪影娃娃",
                     {130, 103, 66, 98, 130, 90}, monbase::Element::Ice);

        addMoveToSpeciesPool(1,  IceCrystalFall);
        addMoveToSpeciesPool(1,  WindSnow);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(1,  SweetEndurance);
        addMoveToSpeciesPool(5,  FrostFall);
        addMoveToSpeciesPool(8,  CrushedIce);
        addMoveToSpeciesPool(12, IceWall);
        addMoveToSpeciesPool(16, BluntStrike);
        addMoveToSpeciesPool(18, BlizzardIce);
        addMoveToSpeciesPool(22, ShatterPot);
        addMoveToSpeciesPool(26, FogSurround);
        addMoveToSpeciesPool(30, BirthdayCake);
        addMoveToSpeciesPool(34, FreezingPoint);
        addMoveToSpeciesPool(36, WinterSolstice);

        checkEvolution();
        codemonId = 15;
        if (evolutionStage >= 1) codemonId = 16;
        if (evolutionStage >= 2) codemonId = 17;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<DaerMaodou>(*this);
    }
protected:
    // 特性：捉迷藏 — 使敌方获得冻结时，也会使其获得全技能能耗+1
    void onAppliedStatus(monbase::Codemon* target, monbase::StatusType type, int stacks) override {
        if (type == monbase::StatusType::Freeze && target)
            target->addAllCostIncrease(1);
    }

    // 破罐破摔：自身有减益时威力+60
    // 碎冰冰：敌方每1层冻结，威力+20
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        if (move.name == "破罐破摔") {
            const auto& s = getStatStages();
            if (s.attack < 0 || s.defense < 0 ||
                s.magicAttack < 0 || s.magicDefense < 0 || s.speed < 0)
                return 60;
        }
        if (move.name == "碎冰冰")
            return target.getStatusStacks(monbase::StatusType::Freeze) * 20;
        return 0;
    }

    // 生日蛋糕：驱散自身减益后，增益翻倍
    void onMoveUsed(const monbase::Move& move) override {
        if (move.name == "生日蛋糕") {
            if (stages.attack > 0) stages.attack = min(stages.attack * 2, kMaxStages);
            if (stages.defense > 0) stages.defense = min(stages.defense * 2, kMaxStages);
            if (stages.magicAttack > 0) stages.magicAttack = min(stages.magicAttack * 2, kMaxStages);
            if (stages.magicDefense > 0) stages.magicDefense = min(stages.magicDefense * 2, kMaxStages);
            if (stages.speed > 0) stages.speed = min(stages.speed * 2, kMaxStages);
        }
    }
};

// 画精灵进化链

class Hualing : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Hualing(int lvl = 1)
        : Codemon("画精灵", monbase::Element::Normal,
                  {61, 78, 56, 32, 72, 72}, lvl)
    {
        using namespace monbase::moves;
        type2 = monbase::Element::Fighting;

        addEvolution(18, "画像守护",
                     {81, 104, 75, 42, 96, 96},
                     monbase::Element::Normal, monbase::Element::Fighting);
        addEvolution(36, "画间沉铁兽",
                     {100, 160, 100, 67, 76, 105},
                     monbase::Element::Normal, monbase::Element::Fighting);

        addMoveToSpeciesPool(1,  TailSweep);
        addMoveToSpeciesPool(1,  CounterPunch);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  PowerBoost);
        addMoveToSpeciesPool(5,  MagicBoost);
        addMoveToSpeciesPool(8,  PreemptiveStrike);
        addMoveToSpeciesPool(12, JeetKuneDo);
        addMoveToSpeciesPool(16, ShadowStrike);
        addMoveToSpeciesPool(18, RoundhouseKick);
        addMoveToSpeciesPool(22, SpiritBoost);
        addMoveToSpeciesPool(26, StressResponse);
        addMoveToSpeciesPool(30, FlyingKick);
        addMoveToSpeciesPool(36, SneakAttack);
        addMoveToSpeciesPool(40, HomeFieldAdvantage);
        addMoveToSpeciesPool(44, WishingStar);

        checkEvolution();
        codemonId = 18;
        if (evolutionStage >= 1) codemonId = 19;
        if (evolutionStage >= 2) codemonId = 20;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Hualing>(*this);
    }
protected:
    // 特性：变形活画 — 攻击时，敌方每1层增益，本次技能威力+10%
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        if (move.category != monbase::MoveCategory::Attack) return 0;
        const auto& s = target.getStatStages();
        int buffLayers = 0;
        if (s.attack > 0) buffLayers += s.attack;
        if (s.defense > 0) buffLayers += s.defense;
        if (s.magicAttack > 0) buffLayers += s.magicAttack;
        if (s.magicDefense > 0) buffLayers += s.magicDefense;
        if (s.speed > 0) buffLayers += s.speed;
        return move.power * buffLayers * 10 / 100;
    }

    // 主场优势：下一次攻击伤害变为三倍
    void onMoveUsed(const monbase::Move& move) override {
        if (move.name == "主场优势")
            damageMultiplier = 3.0f;
    }
};

// 小星光进化链

class Xiaoxingguang : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Xiaoxingguang(int lvl = 1)
        : Codemon("小星光", monbase::Element::Electric,
                  {64, 76, 72, 85, 96, 100}, lvl)
    {
        using namespace monbase::moves;

        addEvolution(36, "星光狮",
                     {80, 95, 90, 107, 120, 125},
                     monbase::Element::Electric);

        addMoveToSpeciesPool(1,  LightOrb);
        addMoveToSpeciesPool(1,  ConductiveTackle);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  MagicBoost);
        addMoveToSpeciesPool(8,  IonSpark);
        addMoveToSpeciesPool(12, Overexposure);
        addMoveToSpeciesPool(16, Thunderstorm);
        addMoveToSpeciesPool(22, DiffuseReflection);
        addMoveToSpeciesPool(28, LightSpear);
        addMoveToSpeciesPool(32, Superconduct);
        addMoveToSpeciesPool(36, RainbowImpact);
        addMoveToSpeciesPool(40, SkyLight);
        addMoveToSpeciesPool(44, ForceRestart);

        checkEvolution();
        codemonId = 21;
        if (evolutionStage >= 1) codemonId = 22;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Xiaoxingguang>(*this);
    }
    static void resetBattleBurstEffects() { battleBurstEffects.clear(); }

protected:
    bool burstAvailable = true;
    int otherElementUseCount = 0;  // 过曝：非光系攻击技能使用次数

    // 全战斗迸发效果记录
    inline static std::vector<std::string> battleBurstEffects;

    void onSwitchIn(int teamCounterCount) override {
        burstAvailable = true;
    }
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        int bonus = 0;

        if (move.category != monbase::MoveCategory::Attack)
            return 0;

        // 电流刺激：迸发·威力+40
        if (burstAvailable)
            bonus += 40;

        // 过曝：每使用1次其他系别技能，本技能威力永久+30
        if (move.name == "过曝")
            bonus += otherElementUseCount * 30;

        // 雷暴迸发：每获得1种迸发效果，威力+10
        if (move.name == "雷暴" && burstAvailable)
            bonus += static_cast<int>(battleBurstEffects.size()) * 10;

        return bonus;
    }

    int onModifyCost(const monbase::Move& move, int cost) const override {
        if (!burstAvailable) return cost;

        if (move.name == "超导")
            return cost - 1;
        if (move.name == "雷暴")
            return cost + static_cast<int>(battleBurstEffects.size());

        return cost;
    }

    void onMoveUsed(const monbase::Move& move) override {
        // 迸发消耗与记录
        if (burstAvailable && move.category == monbase::MoveCategory::Attack) {
            // 记录电流刺激的迸发效果
            if (std::find(battleBurstEffects.begin(), battleBurstEffects.end(),
                          "current_stimulus") == battleBurstEffects.end())
                battleBurstEffects.push_back("current_stimulus");
            burstAvailable = false;
        }

        // 超导迸发记录
        if (move.name == "超导") {
            if (std::find(battleBurstEffects.begin(), battleBurstEffects.end(),
                          "superconduct") == battleBurstEffects.end())
                battleBurstEffects.push_back("superconduct");
        }

        // 过曝：记录非无属性攻击技能使用
        if (move.element != monbase::Element::None
            && move.category == monbase::MoveCategory::Attack)
            otherElementUseCount++;

        // 漫反射：每种系别至多1个技能威力+35
        if (move.name == "漫反射") {
            std::set<monbase::Element> elementsAdded;
            for (auto& m : unlockedMoves) {
                if (m.element != monbase::Element::None
                    && elementsAdded.insert(m.element).second)
                    movePowerBonus[m.name] += 35;
            }
        }
    }
};

// 梦游进化链

class Mengyou : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Mengyou(int lvl = 1)
        : Codemon("梦游", monbase::Element::Ghost,
                  {90, 52, 60, 125, 74, 92}, lvl)
    {
        using namespace monbase::moves;

        addEvolution(32, "梦悠悠",
                     {113, 65, 75, 157, 93, 115},
                     monbase::Element::Ghost);

        addMoveToSpeciesPool(1,  Slap);
        addMoveToSpeciesPool(1,  Scare);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  MagicBoost);
        addMoveToSpeciesPool(8,  Mischief);
        addMoveToSpeciesPool(12, MidnightNoise);
        addMoveToSpeciesPool(16, SoulHook);
        addMoveToSpeciesPool(22, WarmUp);
        addMoveToSpeciesPool(28, BackStrike);
        addMoveToSpeciesPool(32, Control);
        addMoveToSpeciesPool(36, GhostBurst);

        checkEvolution();
        codemonId = 23;
        if (evolutionStage >= 1) codemonId = 24;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Mengyou>(*this);
    }
protected:
    int damageBoostTurns = 0;

    // 特性：做噩梦 — 应对成功时，敌方精灵失去3能量
    void onCounterTrigger(monbase::Codemon* target) override {
        if (target)
            target->drainEnergy(3);
    }

    // 背袭：敌方能量=0时20倍伤害
    // 热身运动：伤害+100%
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        int bonus = 0;

        if (move.category != monbase::MoveCategory::Attack)
            return 0;

        if (damageBoostTurns > 0)
            bonus += move.power;

        if (move.name == "背袭" && target.getEnergy() == 0)
            bonus += move.power * 19;

        return bonus;
    }

    void onMoveUsed(const monbase::Move& move) override {
        if (move.name == "热身运动")
            damageBoostTurns = 3;
    }

    void onTurnEnd() override {
        if (damageBoostTurns > 0)
            damageBoostTurns--;
    }
};

// 阿米亚特 → 阿米樱 → 罗隐

class Amiyate : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    Amiyate(int lvl = 1)
        : Codemon("阿米亚特", monbase::Element::Ground,
                  {64, 95, 67, 47, 42, 45}, lvl)
    {
        addEvolution(30, "阿米樱", {86, 127, 89, 62, 56, 60}, monbase::Element::Ground);
        addEvolution(50, "罗隐", {107, 159, 112, 78, 70, 75},
                     monbase::Element::Ground, monbase::Element::Dark);

        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  DarkClaw);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  SandRush);
        addMoveToSpeciesPool(8,  PowerBoost);
        addMoveToSpeciesPool(12, BatStrike);
        addMoveToSpeciesPool(16, SandTrap);
        addMoveToSpeciesPool(22, MudArmor);
        addMoveToSpeciesPool(28, DarkAmbush);
        addMoveToSpeciesPool(30, BalanceStance);
        addMoveToSpeciesPool(34, PowerDevour);
        addMoveToSpeciesPool(40, RockCrit);
        addMoveToSpeciesPool(46, Earthquake);
        checkEvolution();
        codemonId = 25;
        if (evolutionStage >= 1) codemonId = 26;
        if (evolutionStage >= 2) codemonId = 27;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<Amiyate>(*this);
    }
protected:
    // 盛宴：能量不足时消耗5%最大生命代替1能量
    bool consumeEnergy(int amount) override {
        if (currentEnergy >= amount) {
            currentEnergy -= amount;
            return true;
        }
        int deficit = amount - currentEnergy;
        int hpCost = currentStats.hp * 5 * deficit / 100;
        if (currentHP <= hpCost) return false;
        currentHP -= hpCost;
        currentEnergy = 0;
        return true;
    }

    // 鸣沙陷阱：物防对比(在species的onModifyPower中实现)
    // 蝙蝠/暗突袭：吸血由战斗系统处理
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        if (move.name == "鸣沙陷阱") {
            int myDef = getEffectiveDefense();
            int enemyDef = target.getEffectiveDefense();
            if (myDef >= enemyDef + 120) return 180;    // 240 - 60
            if (myDef >= enemyDef) return 60;            // 120 - 60
        }
        return 0;
    }
};

// 大头骨龙 → 寂灭骨龙

class DatouGulong : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    DatouGulong(int lvl = 1)
        : Codemon("大头骨龙", monbase::Element::Dragon,
                  {96, 109, 84, 40, 65, 48}, lvl)
    {
        type2 = monbase::Element::Ghost;
        addEvolution(40, "寂灭骨龙", {120, 137, 104, 50, 81, 60},
                     monbase::Element::Dragon, monbase::Element::Ghost);

        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  GhostStab);
        addMoveToSpeciesPool(1,  DragonRoar);
        addMoveToSpeciesPool(1,  Illusion);
        addMoveToSpeciesPool(5,  Vanish);
        addMoveToSpeciesPool(8,  PreemptiveStrike);
        addMoveToSpeciesPool(12, Stance);
        addMoveToSpeciesPool(16, HawkScale);
        addMoveToSpeciesPool(22, DragonClaw);
        addMoveToSpeciesPool(28, DragonAwe);
        addMoveToSpeciesPool(34, GraveyardFight);
        addMoveToSpeciesPool(40, FlameBreath);
        checkEvolution();
        codemonId = 28;
        if (evolutionStage >= 1) codemonId = 29;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<DatouGulong>(*this);
    }
protected:
    bool immortalUsed = false;

    // 不朽：首次HP<50%时回满
    void takeDamage(int dmg) override {
        monbase::Codemon::takeDamage(dmg);
        if (!immortalUsed && currentHP <= currentStats.hp * 50 / 100 && currentHP > 0) {
            currentHP = currentStats.hp;
            currentEnergy = kMaxEnergy;
            immortalUsed = true;
        }
    }

    // 坟场搏击：敌方每有1能量威力-10%
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        if (move.name == "坟场搏击")
            return -(move.power * target.getEnergy() / 10);
        if (move.name == "吹炎") {
            // 应对状态→威力×2 由战斗系统处理，这里不处理
        }
        return 0;
    }
};

// 胆小鳗鱼 → 闪电鳗鱼

class DanxiaoManyu : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    DanxiaoManyu(int lvl = 1)
        : Codemon("胆小鳗鱼", monbase::Element::Electric,
                  {68, 92, 83, 92, 90, 92}, lvl)
    {
        type2 = monbase::Element::Water;
        addEvolution(36, "闪电鳗鱼", {85, 115, 104, 115, 112, 115},
                     monbase::Element::Electric, monbase::Element::Water);

        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  Bubble);
        addMoveToSpeciesPool(1,  ConductiveTackle);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  StoreWater);
        addMoveToSpeciesPool(8,  ArcStrike);
        addMoveToSpeciesPool(12, WaterSplash);
        addMoveToSpeciesPool(16, Thunderstorm);
        addMoveToSpeciesPool(22, Focus);
        addMoveToSpeciesPool(28, CrossLightning);
        addMoveToSpeciesPool(32, IonBlast);
        addMoveToSpeciesPool(36, DualPulse);
        addMoveToSpeciesPool(40, EMDeflect);
        addMoveToSpeciesPool(44, HydroImpact);
        addMoveToSpeciesPool(48, ForceRestart);
        checkEvolution();
        codemonId = 30;
        if (evolutionStage >= 1) codemonId = 31;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<DanxiaoManyu>(*this);
    }
protected:
    bool burstAvailable = false;
    inline static std::vector<std::string> battleBurstEffectsL;  // 迸发记录

    void onSwitchIn(int) override { burstAvailable = true; }

    static void resetBurstEffects() { battleBurstEffectsL.clear(); }

    // 生物电：电系技能迸发→能耗-2
    int onModifyCost(const monbase::Move& move, int cost) const override {
        if (burstAvailable && move.element == monbase::Element::Electric)
            return cost - 2;
        return cost;
    }

    // 电弧迸发：+40 威力；双联脉冲迸发：威力翻倍
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        if (!burstAvailable || move.category != monbase::MoveCategory::Attack) return 0;
        if (move.name == "双联脉冲") return move.power;  // ×2
        if (move.element == monbase::Element::Electric) return 40;
        return 0;
    }

    void onMoveUsed(const monbase::Move& move) override {
        if (burstAvailable && move.category == monbase::MoveCategory::Attack)
            burstAvailable = false;
    }
};

// 翡翠水母 → 琉璃水母

class FeicuiShuimu : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    FeicuiShuimu(int lvl = 1)
        : Codemon("翡翠水母", monbase::Element::Water,
                  {108, 30, 75, 68, 109, 76}, lvl)
    {
        type2 = monbase::Element::Poison;
        addEvolution(26, "琉璃水母", {134, 37, 93, 85, 136, 95},
                     monbase::Element::Water, monbase::Element::Poison);

        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  WaterFlick);
        addMoveToSpeciesPool(1,  CorrosiveAcid);
        addMoveToSpeciesPool(1,  Untouchable);
        addMoveToSpeciesPool(5,  MagicBoost);
        addMoveToSpeciesPool(8,  MiasmaShot);
        addMoveToSpeciesPool(12, Purification);
        addMoveToSpeciesPool(16, PlagueBreath);
        addMoveToSpeciesPool(22, VenomCure);
        addMoveToSpeciesPool(26, Torrent);
        addMoveToSpeciesPool(30, PoisonVenom);
        addMoveToSpeciesPool(36, VenomPenetration);
        addMoveToSpeciesPool(42, Annihilation);
        checkEvolution();
        codemonId = 32;
        if (evolutionStage >= 1) codemonId = 33;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<FeicuiShuimu>(*this);
    }
protected:
    // 扩散侵蚀：使用水系技能后，敌方获得中毒(层数=当前中毒×2)
    // 鸩毒：每层中毒+10(应对后+20)
    // 不可接触：每层中毒+10%永久减伤
    // 毒液渗透：敌方每层中毒能耗-1
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        if (move.name == "鸩毒") {
            return target.getStatusStacks(monbase::StatusType::Poison) * 10;
        }
        return 0;
    }

    //int onModifyCost(const monbase::Move& move, int cost) const override {
    //    if (move.name == "毒液渗透") {
    //        // 无法获取当前战斗的目标，需要战斗系统支持
    //    }
    //    return cost;
    //}
};

// 深蓝鲸

class ShenlanJing : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    ShenlanJing(int lvl = 1)
        : Codemon("深蓝鲸", monbase::Element::Water,
                  {129, 40, 119, 115, 135, 50}, lvl)
    {
        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  WaterFlick);
        addMoveToSpeciesPool(1,  Bubble);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  MagicBoost);
        addMoveToSpeciesPool(8,  WaterShield);
        addMoveToSpeciesPool(12, SonicBoom);
        addMoveToSpeciesPool(16, WaterCannon);
        addMoveToSpeciesPool(22, WaterWave);
        addMoveToSpeciesPool(28, WaterRing);
        addMoveToSpeciesPool(34, Torrent);
        addMoveToSpeciesPool(40, Tide);
        addMoveToSpeciesPool(46, Flood);
        checkEvolution();
        codemonId = 34;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<ShenlanJing>(*this);
    }
protected:
};

// 水滴蛇 → 水蛇锁 → 游蛇魔使

class ShuidiShe : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    ShuidiShe(int lvl = 1)
        : Codemon("水滴蛇", monbase::Element::Water,
                  {63, 66, 62, 62, 62, 63}, lvl)
    {
        type2 = monbase::Element::Fighting;
        addEvolution(16, "水蛇锁", {84, 88, 83, 83, 83, 84},
                     monbase::Element::Water, monbase::Element::Fighting);
        addEvolution(36, "游蛇魔使", {105, 110, 103, 104, 103, 105},
                     monbase::Element::Water, monbase::Element::Fighting);

        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  TideSurge);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(1,  Slap);
        addMoveToSpeciesPool(5,  PowerFocus);
        addMoveToSpeciesPool(8,  WaterScreenStrike);
        addMoveToSpeciesPool(12, ReadyStance);
        addMoveToSpeciesPool(16, ShadowStrike);
        addMoveToSpeciesPool(20, BreakDefense);
        addMoveToSpeciesPool(24, ListenBridge);
        addMoveToSpeciesPool(28, StackingMomentum);
        addMoveToSpeciesPool(32, SpiritBoost);
        addMoveToSpeciesPool(36, Wet);
        addMoveToSpeciesPool(40, Snore);
        addMoveToSpeciesPool(46, Spring);
        checkEvolution();
        codemonId = 35;
        if (evolutionStage >= 1) codemonId = 36;
        if (evolutionStage >= 2) codemonId = 37;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<ShuidiShe>(*this);
    }
protected:
    // 思维之盾：应对成功后，下次行动技能能耗-5
    void onCounterTrigger(monbase::Codemon* target) override {
        nextCostReduction += 5;
    }

    //// 叠势：每成功应对1次连击数永久+50(需战斗系统跟踪应对次数)
    //int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
    //    if (move.name == "叠势") {
    //        // 需战斗系统提供counterCount
    //    }
    //    return 0;
    //}
};

// 加油海葵 → 加油蟹

class JiayouHaikui : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    JiayouHaikui(int lvl = 1)
        : Codemon("加油海葵", monbase::Element::Water,
                  {93, 87, 102, 87, 102, 76}, lvl)
    {
        type2 = monbase::Element::Cute;
        addEvolution(28, "加油蟹", {116, 108, 128, 108, 128, 95},
                     monbase::Element::Water, monbase::Element::Cute);

        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  Whip);
        addMoveToSpeciesPool(1,  TideSurge);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  Cheer);
        addMoveToSpeciesPool(8,  SuperCandy);
        addMoveToSpeciesPool(12, Wash);
        addMoveToSpeciesPool(16, WaterScreenStrike);
        addMoveToSpeciesPool(22, CuteCharm);
        addMoveToSpeciesPool(28, HydroImpact);
        addMoveToSpeciesPool(32, Tide);
        addMoveToSpeciesPool(36, PopcornBlast);
        addMoveToSpeciesPool(40, BirthdayCake);
        checkEvolution();
        codemonId = 38;
        if (evolutionStage >= 1) codemonId = 39;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<JiayouHaikui>(*this);
    }
protected:
    int cheerStacks = 0;

    void onSwitchIn(int count) override {
        cheerStacks = 0;
    }

    // 拉拉队长：回合结束时敌方双攻-20%，自身双攻+20%
    void onTurnEnd() override {
        cheerStacks++;
        // 敌方双攻-20% (由战斗系统处理，需要敌方引用)
    }

    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        if (cheerStacks > 0 && move.category == monbase::MoveCategory::Attack)
            return static_cast<int>(move.power * 0.2f * cheerStacks);
        return monbase::Codemon::onModifyPower(move, target);
    }

    // 撒娇：威力永久+20
    void onMoveUsed(const monbase::Move& move) override {
        if (move.name == "撒娇")
            movePowerBonus["撒娇"] += 20;
    }

    // 清洗：每有1层减益能耗-1
    int onModifyCost(const monbase::Move& move, int cost) const override {
        if (move.name == "清洗") {
            const auto& s = getStatStages();
            int debuffCount = 0;
            if (s.attack < 0) debuffCount++;
            if (s.defense < 0) debuffCount++;
            if (s.magicAttack < 0) debuffCount++;
            if (s.magicDefense < 0) debuffCount++;
            if (s.speed < 0) debuffCount++;
            return cost - debuffCount;
        }
        return cost;
    }
};

// 小狮鹫 → 神圣狮鹫 → 皇家狮鹫

class XiaoShijiu : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    XiaoShijiu(int lvl = 1)
        : Codemon("小狮鹫", monbase::Element::Flying,
                  {64, 70, 76, 41, 39, 72}, lvl)
    {
        addEvolution(32, "神圣狮鹫", {85, 93, 102, 55, 52, 96}, monbase::Element::Flying);
        addEvolution(46, "皇家狮鹫", {107, 116, 127, 69, 65, 120}, monbase::Element::Flying);

        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  Peck);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(5,  FlyingFeather);
        addMoveToSpeciesPool(8,  GustStab);
        addMoveToSpeciesPool(12, TripleBreak);
        addMoveToSpeciesPool(16, Parry);
        addMoveToSpeciesPool(20, FeatherBlade);
        addMoveToSpeciesPool(24, QuickMove);
        addMoveToSpeciesPool(28, FanWind);
        addMoveToSpeciesPool(32, FeatherBoost);
        addMoveToSpeciesPool(36, RideWind);
        addMoveToSpeciesPool(40, Tornado);
        checkEvolution();
        codemonId = 40;
        if (evolutionStage >= 1) codemonId = 41;
        if (evolutionStage >= 2) codemonId = 42;
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<XiaoShijiu>(*this);
    }
protected:
    // 乘风连击：使用翼系技能后全技能威力+20
    void onMoveUsed(const monbase::Move& move) override {
        if (move.element == monbase::Element::Flying) {
            for (auto& m : unlockedMoves)
                movePowerBonus[m.name] += 20;
        }
        // 羽刃：应对状态→伤害永久+30(需战斗系统处理)
        // 见招拆招：上回合使用状态技能则+55(需战斗系统跟踪上回合)
    }

    // 扇风：先手+50%由战斗系统处理
};

// 可爱猿 → 炽热猿 → 火焰猿

class KeaiYuan : public monbase::Codemon {
public:
    using monbase::Codemon::Codemon;

    KeaiYuan(int lvl = 1)
        : Codemon("可爱猿", monbase::Element::Fire,
                  {100, 97, 86, 43, 67, 36}, lvl)
    {
        addEvolution(28, "炽热猿", {133, 130, 115, 57, 89, 48}, monbase::Element::Fire);
        addEvolution(40, "火焰猿", {167, 162, 143, 72, 111, 60}, monbase::Element::Fire);

        using namespace monbase::moves;
        addMoveToSpeciesPool(1,  FlameBreathLight);
        addMoveToSpeciesPool(1,  Defend);
        addMoveToSpeciesPool(1,  Ember);
        addMoveToSpeciesPool(5,  PowerBoost);
        addMoveToSpeciesPool(8,  PreemptiveStrike);
        addMoveToSpeciesPool(12, WarmUpFire);
        addMoveToSpeciesPool(16, Fury);
        addMoveToSpeciesPool(22, BurnStrike);
        addMoveToSpeciesPool(28, HeatWave);
        addMoveToSpeciesPool(34, FlashBurn);
        addMoveToSpeciesPool(40, FireChariot);
        addMoveToSpeciesPool(44, FullBurn);
        addMoveToSpeciesPool(48, BurnBrand);
        checkEvolution();
        codemonId = 43;
        if (evolutionStage >= 1) codemonId = 44;
        if (evolutionStage >= 2) codemonId = 45;
        currentEnergy = 5; // 散热：初始能量为5
    }
    unique_ptr<Codemon> clone() const override {
        return make_unique<KeaiYuan>(*this);
    }
protected:
    int onModifyPower(const monbase::Move& move, const monbase::Codemon& target) const override {
        return 0;
    }
};
