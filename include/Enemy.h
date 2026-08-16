#pragma once
#include "types.h"
#include "move.h"
#include "ItemType.h"

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>

using namespace std;

namespace monbase {

    // 技能池条目：精灵在某等级可习得的技能
    struct MovePoolEntry {
        int learnLevel;   // 习得此技能的等级
        Move move;        // 技能本身
    };

    // 进化链节点
    struct EvolutionData {
        int evolveLevel;           // 进化所需等级
        string evolveName;         // 进化后的名称
        Stats evolveBaseStats;     // 进化后的基础种族值
        Element evolveType1;       // 进化后的第一属性
        Element evolveType2 = Element::None;  // 进化后的第二属性
    };

    // 精灵基类
    class Codemon {
    protected:
        // 基本信息
        string name;                    // 精灵名称
        int codemonId = 0;              // 图鉴编号
        Element type1;                  // 第一属性
        Element type2;                  // 第二属性
        int level;                      // 当前等级
        int experience;                 // 当前累积经验值

        // 属性系统
        Stats baseStats;                // 基础种族值
        Stats currentStats;             // 当前实际属性
        StatStages stages;              // 能力升降阶段
        int currentHP;                  // 当前 HP
        int currentEnergy;              // 当前能量
        float shieldAmount = 0.0f;      // 护盾减伤比例

        // 装备系统
        std::map<EquipSlot, ItemID> equippedItems;  // 已装备物品

        // 属性加成系统
        struct PermanentBonuses {
            int attack = 0;
            int defense = 0;
            int magicAttack = 0;
            int magicDefense = 0;
            int speed = 0;
        } permBonuses;

        // 临时加成，休息后重置
        struct TemporaryBonuses {
            int attack = 0;
            int defense = 0;
            int magicAttack = 0;
            int magicDefense = 0;
            int speed = 0;
        } tempBonuses;

        // 费用与伤害修正
        int nextCostReduction = 0;       // 下次技能费用减少量
        int allCostIncrease = 0;         // 所有技能费用的额外增加
        float damageMultiplier = 1.0f;   // 伤害倍率修正

        // 常量定义
        static constexpr int kMaxEnergy = 10;       // 最大能量值
        static constexpr int kMaxStages = 99;       // 能力阶段上限
        static constexpr int kElementCount = 16;    // 元素种类总数

        // 技能成长跟踪

        int elementUseCounts[kElementCount] = { 0 };          // 每种元素属性技能的使用次数
        unordered_map<string, int> moveUseCount;              // 每个技能名称的使用次数
        unordered_map<string, int> moveCostReduce;            // 每个技能名称的费用缩减累计
        unordered_map<string, int> movePowerBonus;            // 每个技能名称的威力加成累计

        // 进化系统
        vector<EvolutionData> evolutionChain;  // 进化链
        int evolutionStage = 0;                // 当前进化阶段索引

        // 异常状态系统
        unordered_map<StatusType, int> statusStacks;  // 异常状态及其层数

        // 技能系统
        vector<MovePoolEntry> speciesMovePool;  // 种族技能池
        vector<Move> unlockedMoves;             // 已解锁的所有技能
        vector<int> equippedIndices;            // 已装备的技能在 unlockedMoves 中的索引


        // 计算有效属性值
        int getEffectiveStat(int base, int s, bool isSpeed) const {
            if (isSpeed) return max(1, base + s * 10);
            float multiplier = 1.0f + s * 0.1f;
            return max(1, static_cast<int>(base * multiplier));
        }

        // 根据效果类型获取阶段指针
        int* stagePtr(MoveEffect ef) {
            if (ef == MoveEffect::BuffAttack || ef == MoveEffect::DebuffAttack)
                return &stages.attack;
            if (ef == MoveEffect::BuffDefense || ef == MoveEffect::DebuffDefense)
                return &stages.defense;
            if (ef == MoveEffect::BuffMagicAttack || ef == MoveEffect::DebuffMagicAttack)
                return &stages.magicAttack;
            if (ef == MoveEffect::BuffMagicDefense || ef == MoveEffect::DebuffMagicDefense)
                return &stages.magicDefense;
            if (ef == MoveEffect::BuffSpeed || ef == MoveEffect::DebuffSpeed)
                return &stages.speed;
            return nullptr;
        }

    public:
        // 构造函数
        Codemon(string n, Element t1, Stats base, int lvl = 1)
            : name(move(n)), type1(t1), type2(Element::None),
            baseStats(base), level(lvl), experience(xpForLevel(lvl)), stages{} {
            calculateActualStats();
            currentHP = currentStats.hp;
            currentEnergy = kMaxEnergy;
        }

        virtual ~Codemon() = default;                         // 虚析构
        virtual unique_ptr<Codemon> clone() const = 0;        // 纯虚克隆方法

        // 图鉴编号
        int getCodemonId() const { return codemonId; }
        void setCodemonId(int id) { codemonId = id; }

        // 公开设置器
        void setHP(int hp) { currentHP = min(hp, currentStats.hp); }       // 设置HP
        void setEnergy(int energy) { currentEnergy = min(energy, kMaxEnergy); } // 设置能量

        // 伤害倍率管理
        float getDamageMultiplier() const { return damageMultiplier; }
        void setDamageMultiplier(float mult) { damageMultiplier = mult; }
        void resetDamageMultiplier() { damageMultiplier = 1.0f; }

        // 下次费用减免
        int getNextCostReduction() const { return nextCostReduction; }
        void setNextCostReduction(int n) { nextCostReduction = max(0, n); }

        // 全局费用增加
        int getAllCostIncrease() const { return allCostIncrease; }
        void addAllCostIncrease(int amount) { allCostIncrease += amount; 

        // 执行附加效果
        void applyExtraEffect(MoveEffect ef, int p, Codemon* target = nullptr) {
            if (ef == MoveEffect::HealHP) {
                heal(p);                                   // 固定值治疗
            }
            else if (ef == MoveEffect::HealHPPercent) {
                heal(currentStats.hp * p / 100);           // 百分比治疗
            }
            else if (ef == MoveEffect::RestoreEnergy) {
                currentEnergy = min(currentEnergy + p, kMaxEnergy);  // 回复能量
            }
            else if (ef >= MoveEffect::BuffAttack && ef <= MoveEffect::BuffSpeed) {
                modifyStatStage(ef, p);                    // 提升自身能力阶段
            }
            else if (ef >= MoveEffect::DebuffAttack && ef <= MoveEffect::DebuffSpeed) {
                if (target) target->modifyStatStage(ef, -p);  // 降低敌方能力
                else        modifyStatStage(ef, -p);          // 无目标则降低自身
            }
            else if (ef == MoveEffect::Shield) {
                shieldAmount = p / 100.0f;                 // 设置护盾减伤比例
            }
            else if (ef == MoveEffect::Dispel) {
                if (target) target->clearStatStages();     // 驱散目标所有增益
            }
            else if (ef == MoveEffect::DispelOne) {
                if (target) target->clearOneBuff();        // 驱散目标一个增益
            }
            else if (ef == MoveEffect::ClearDebuffs) {
                clearNegativeStages();                     // 清除自身减益
            }
            else if (ef == MoveEffect::ReduceAllCost) {
                for (auto& m : unlockedMoves)
                    moveCostReduce[m.name] += p;           // 所有技能费用 -p
            }
            else if (ef == MoveEffect::BoostAllPower) {
                for (auto& m : unlockedMoves)
                    movePowerBonus[m.name] += p;           // 所有技能威力 +p
            }
            else if (ef == MoveEffect::ApplyPoison) {
                if (target) {
                    target->applyStatus(StatusType::Poison, p);           // 施加中毒
                    onAppliedStatus(target, StatusType::Poison, p);       // 触发特性钩子
                }
            }
            else if (ef == MoveEffect::ApplyBurn) {
                if (target) {
                    target->applyStatus(StatusType::Burn, p);             // 施加灼烧
                    onAppliedStatus(target, StatusType::Burn, p);
                }
            }
            else if (ef == MoveEffect::ApplyFreeze) {
                if (target) {
                    target->applyStatus(StatusType::Freeze, p);           // 施加冻结
                    onAppliedStatus(target, StatusType::Freeze, p);
                }
            }
            else if (ef == MoveEffect::IncreaseAllCost) {
                if (target) target->allCostIncrease += p;   // 敌方全技能费用 +p
            }
            else if (ef == MoveEffect::EnemyLoseEnergy) {
                if (target) {
                    int drained = min(target->currentEnergy, p);  // 吸取敌方能量
                    target->currentEnergy -= drained;
                }
            }
        }

        virtual void onMoveUsed(const Move&) {}                    // 技能使用后触发
        virtual void onRest() {}                                   // 休息时触发
        virtual void onTurnEnd() {}                                // 回合结束时触发
        virtual void onSwitchIn(int teamCounterCount) {}            // 入场时触发
        virtual void onSwitchOut(Codemon* nextAlly) {}             // 退场时触发
        virtual void onAppliedStatus(Codemon* target, StatusType type, int stacks) {}  // 施加异常状态后
        virtual int onModifyPower(const Move& move, const Codemon& target) const { return 0; }  // 修改技能威力
        virtual int onModifyCost(const Move& move, int cost) const { return cost; }     // 修改技能费用
        virtual void onCounterTrigger(Codemon* target) {}          // 应对成功时触发

        // 计算实际属性值
        void calculateActualStats() {
            auto calc = [&](int base, bool isHP) {
                if (isHP) return ((base * 2) * level / 100) + level + 10;
                return ((base * 2) * level / 100) + 5;
                };
            currentStats.hp = calc(baseStats.hp, true);
            currentStats.attack = calc(baseStats.attack, false) + permBonuses.attack + tempBonuses.attack;
            currentStats.defense = calc(baseStats.defense, false) + permBonuses.defense + tempBonuses.defense;
            currentStats.magicAttack = calc(baseStats.magicAttack, false) + permBonuses.magicAttack + tempBonuses.magicAttack;
            currentStats.magicDefense = calc(baseStats.magicDefense, false) + permBonuses.magicDefense + tempBonuses.magicDefense;
            currentStats.speed = calc(baseStats.speed, false) + permBonuses.speed + tempBonuses.speed;
        }

        // 应用临时增益
        void applyTemporaryBoost(MoveEffect effect, int amount) {
            switch (effect) {
            case MoveEffect::HealHPPercent:    heal(currentStats.hp * amount / 100); break;
            case MoveEffect::BuffAttack:       tempBonuses.attack += amount; break;
            case MoveEffect::BuffDefense:      tempBonuses.defense += amount; break;
            case MoveEffect::BuffMagicAttack:  tempBonuses.magicAttack += amount; break;
            case MoveEffect::BuffMagicDefense: tempBonuses.magicDefense += amount; break;
            case MoveEffect::BuffSpeed:        tempBonuses.speed += amount; break;
            case MoveEffect::ClearDebuffs:     clearNegativeStages(); break;
            case MoveEffect::RestoreEnergy:    currentEnergy = kMaxEnergy; break;
            default: break;
            }
            calculateActualStats();
        }

        // 清除所有临时加成
        void clearTemporaryBonuses() {
            tempBonuses = TemporaryBonuses{};
            calculateActualStats();
        }

        // 应用装备加成
        void applyEquipmentBoost(MoveEffect effect, int amount, ItemID id, EquipSlot slot) {
            switch (effect) {
            case MoveEffect::BuffAttack:       permBonuses.attack += amount; break;
            case MoveEffect::BuffDefense:      permBonuses.defense += amount; break;
            case MoveEffect::BuffMagicAttack:  permBonuses.magicAttack += amount; break;
            case MoveEffect::BuffMagicDefense: permBonuses.magicDefense += amount; break;
            case MoveEffect::BuffSpeed:        permBonuses.speed += amount; break;
            default: break;
            }
            equippedItems[slot] = id;
            calculateActualStats();
        }

        // 移除装备加成
        void removeEquipmentBoost(MoveEffect effect, int amount, EquipSlot slot) {
            switch (effect) {
            case MoveEffect::BuffAttack:       permBonuses.attack -= amount; break;
            case MoveEffect::BuffDefense:      permBonuses.defense -= amount; break;
            case MoveEffect::BuffMagicAttack:  permBonuses.magicAttack -= amount; break;
            case MoveEffect::BuffMagicDefense: permBonuses.magicDefense -= amount; break;
            case MoveEffect::BuffSpeed:        permBonuses.speed -= amount; break;
            default: break;
            }
            equippedItems.erase(slot);
            calculateActualStats();
        }

        // 有效属性获取
        int getEffectiveAttack() const { return getEffectiveStat(currentStats.attack, stages.attack, false); }
        int getEffectiveDefense() const { return getEffectiveStat(currentStats.defense, stages.defense, false); }
        int getEffectiveMagicAttack() const { return getEffectiveStat(currentStats.magicAttack, stages.magicAttack, false); }
        int getEffectiveMagicDefense() const { return getEffectiveStat(currentStats.magicDefense, stages.magicDefense, false); }
        int getEffectiveSpeed() const { return getEffectiveStat(currentStats.speed, stages.speed, true); }

        // 修改能力阶段
        void modifyStatStage(MoveEffect ef, int delta) {
            int* ptr = stagePtr(ef);
            if (!ptr) return;
            *ptr = clamp(*ptr + delta, -kMaxStages, kMaxStages);
        }

        // 获取某属性的阶段值
        int getStatStage(MoveEffect ef) const {
            if (ef == MoveEffect::BuffAttack || ef == MoveEffect::DebuffAttack)
                return stages.attack;
            if (ef == MoveEffect::BuffDefense || ef == MoveEffect::DebuffDefense)
                return stages.defense;
            if (ef == MoveEffect::BuffMagicAttack || ef == MoveEffect::DebuffMagicAttack)
                return stages.magicAttack;
            if (ef == MoveEffect::BuffMagicDefense || ef == MoveEffect::DebuffMagicDefense)
                return stages.magicDefense;
            if (ef == MoveEffect::BuffSpeed || ef == MoveEffect::DebuffSpeed)
                return stages.speed;
            return 0;
        }

        // 清除所有能力阶段
        void clearStatStages() { stages = StatStages{}; }

        // 清除所有负面的能力阶段
        void clearNegativeStages() {
            if (stages.attack < 0)      stages.attack = 0;
            if (stages.defense < 0)     stages.defense = 0;
            if (stages.magicAttack < 0) stages.magicAttack = 0;
            if (stages.magicDefense < 0)stages.magicDefense = 0;
            if (stages.speed < 0)       stages.speed = 0;
        }

        // 清除一个增益
        void clearOneBuff() {
            int* targets[] = { &stages.attack, &stages.defense, &stages.magicAttack, &stages.magicDefense, &stages.speed };
            int* highest = nullptr;
            for (auto* p : targets) {
                if (*p > 0 && (!highest || *p > *highest))
                    highest = p;
            }
            if (highest) *highest = 0;
        }

        // 进化系统

        // 添加一级进化
        void addEvolution(int lvl, string newName, Stats newStats,
            Element newType1, Element newType2 = Element::None) {
            evolutionChain.push_back({ lvl, move(newName), newStats, newType1, newType2 });
        }

        // 判断是否可以进化：有未完成的进化且等级达标
        bool canEvolve() const {
            return evolutionStage < static_cast<int>(evolutionChain.size())
                && level >= evolutionChain[evolutionStage].evolveLevel;
        }

        // 执行一次进化
        void evolve() {
            if (!canEvolve()) return;
            auto& evo = evolutionChain[evolutionStage];
            name = evo.evolveName;
            baseStats = evo.evolveBaseStats;
            type1 = evo.evolveType1;
            type2 = evo.evolveType2;
            calculateActualStats();
            currentHP = currentStats.hp;
            evolutionStage++;
        }

        // 连续进化直到不可再进化
        void checkEvolution() { while (canEvolve()) evolve(); }
        int getEvolutionStage() const { return evolutionStage; }
        int getMaxEvolutionStage() const { return static_cast<int>(evolutionChain.size()); }
        bool hasEvolution() const { return !evolutionChain.empty(); }

        // 技能池管理

        // 添加技能到种族池
        void addMoveToSpeciesPool(int lvl, Move m) {
            speciesMovePool.push_back({ lvl, move(m) });
            if (level >= lvl) {
                unlockedMoves.push_back(speciesMovePool.back().move);
                if (equippedIndices.size() < 4)
                    equippedIndices.push_back(static_cast<int>(unlockedMoves.size()) - 1);
            }
        }

        // 升一级
        void levelUp() {
            level++;
            calculateActualStats();
            for (auto& entry : speciesMovePool) {
                if (entry.learnLevel == level) {
                    unlockedMoves.push_back(entry.move);
                    if (equippedIndices.size() < 4)
                        equippedIndices.push_back(static_cast<int>(unlockedMoves.size()) - 1);
                }
            }
            checkEvolution();
        }

        // 经验值系统

        // 计算等级所需经验
        static int xpForLevel(int level) {
            if (level <= 1) return 0;
            return level * level * level;
        }

        // 增加经验
        void addExperience(int amount) {
            experience += amount;
            while (level < 100 && experience >= xpForLevel(level + 1)) {
                levelUp();
            }
        }

        // 距下一级还差多少经验
        int getXpToNextLevel() const {
            return xpForLevel(level + 1) - xpForLevel(level);
        }

        // 当前等级内已有多少经验
        int getXpProgress() const {
            return experience - xpForLevel(level);
        }

        // 技能装备管理

        // 设置装备技能索引列表
        void setEquippedIndices(const std::vector<int>& indices) {
            equippedIndices = indices;
        }

        // 添加一个已解锁技能到装备栏
        void addEquippedIndex(int index) {
            if (equippedIndices.size() < 4) {
                equippedIndices.push_back(index);
            }
        }

        // 将已解锁技能添加到装备栏
        void addMoveToUnlockedMoves(const Move& move) {
            unlockedMoves.push_back(move);
            if (equippedIndices.size() < 4) {
                equippedIndices.push_back(static_cast<int>(unlockedMoves.size()) - 1);
            }
        }

        // 交换装备技能
        bool swapEquippedMove(int eIdx, int uIdx) {
            if (eIdx < 0 || eIdx >= static_cast<int>(equippedIndices.size())) return false;
            if (uIdx < 0 || uIdx >= static_cast<int>(unlockedMoves.size())) return false;
            equippedIndices[eIdx] = uIdx;
            return true;
        }

        bool unequipMove(int slotIndex)
        {
            if (slotIndex < 0 || slotIndex >= static_cast<int>(equippedIndices.size()))
                return false;
            equippedIndices.erase(equippedIndices.begin() + slotIndex);
            return true;
        }

        // 添加已解锁技能但不自动添加到 equippedIndices
        void addUnlockedMoveWithoutEquip(const Move& move)
        {
            unlockedMoves.push_back(move);
        }

        // 清空已解锁技能和已装备索引
        void clearUnlockedMoves()
        {
            unlockedMoves.clear();
            equippedIndices.clear();
        }

        // 能量系统

        bool hasEnoughEnergy(int cost) const { return currentEnergy >= cost; }

        // 消耗能量
        virtual bool consumeEnergy(int amount) {
            if (currentEnergy < amount) return false;
            currentEnergy -= amount;
            return true;
        }

        void drainEnergy(int amount) {
            currentEnergy = std::max(0, currentEnergy - amount);
        }

        void restoreEnergy() { currentEnergy = kMaxEnergy; }
        int getMaxEnergy() const { return kMaxEnergy; }

        // 能量充能：+5点，返回true如果确实充了
        bool useEnergyCharge() {
            if (currentEnergy >= kMaxEnergy) return false;
            currentEnergy = min(currentEnergy + 5, kMaxEnergy);
            return true;
        }

        bool canUseEnergyCharge() const { return currentEnergy < kMaxEnergy; }

        // 计算技能实际费用
        int getEffectiveCost(const Move& mv) const {
            int cost = mv.energyCost;
            if (mv.costScalingBonus > 0)
                cost += elementUseCounts[int(mv.element)] * mv.costScalingBonus;
            auto it = moveCostReduce.find(mv.name);
            if (it != moveCostReduce.end())
                cost -= it->second;
            cost += allCostIncrease;
            cost = onModifyCost(mv, cost);
            return max(0, cost);
        }

        // 战斗系统

        // 获取装备槽的技能指针
        const Move* getEquippedMove(int slotIndex) const {
            if (slotIndex < 0 || slotIndex >= static_cast<int>(equippedIndices.size()))
                return nullptr;
            int moveIdx = equippedIndices[slotIndex];
            if (moveIdx < 0 || moveIdx >= static_cast<int>(unlockedMoves.size()))
                return nullptr;
            return &unlockedMoves[moveIdx];
        }

        // 计算伤害
        int calculateDamage(const Move& move, const Codemon& target) const {
            int power = move.power;

            // HP阈值加成：HP高于指定百分比时获得额外威力
            if (move.hpThresholdPercent > 0) {
                int hpPercent = currentHP * 100 / currentStats.hp;
                if (hpPercent > move.hpThresholdPercent)
                    power += move.hpThresholdBonus;
            }

            // 同元素使用次数加成
            if (move.scalingBonus > 0)
                power += elementUseCounts[int(move.element)] * move.scalingBonus;

            // 自使用成长/指数成长
            int selfUsed = 0;
            if (move.selfScalingBonus > 0 || move.exponentScaling > 0) {
                auto it = moveUseCount.find(move.name);
                selfUsed = (it != moveUseCount.end()) ? it->second : 0;
            }
            if (move.selfScalingBonus > 0)
                power += selfUsed * move.selfScalingBonus;
            if (move.exponentScaling > 0) {
                int otherUsed = elementUseCounts[int(move.element)] - selfUsed;
                power = move.power * (1 << otherUsed);  // 2^otherUsed 倍
            }

            // 费用→威力转化
            if (move.costToPowerBonus > 0) {
                auto it = moveCostReduce.find(move.name);
                if (it != moveCostReduce.end())
                    power += it->second * move.costToPowerBonus;
            }

            // 累计威力加成
            {
                auto it = movePowerBonus.find(move.name);
                if (it != movePowerBonus.end())
                    power += it->second;
            }

            // 子类特性加成
            power += onModifyPower(move, target);

            // 计算攻防比
            int atk = move.isPhysical ? getEffectiveAttack() : getEffectiveMagicAttack();
            int def = move.isPhysical ? target.getEffectiveDefense() : target.getEffectiveMagicDefense();
            if (def <= 0) def = 1;  // 防除0
            int raw = (power * atk) / def;
            return max(1, (raw * level / 10 + 2) / 3);
        }

        // 使用装备槽的技能
        MoveResult useEquippedMove(int slotIndex, Codemon* target = nullptr) {
            MoveResult result;
            const Move* move = getEquippedMove(slotIndex);
            if (!move) return result;
            result.move = move;

            // 计算费用并消耗能量
            int cost = getEffectiveCost(*move);
            cost = max(0, cost - nextCostReduction);
            nextCostReduction = 0;
            if (!consumeEnergy(cost)) return result;

            result.success = true;
            result.effect = move->effect;

            auto ef = move->effect;
            // 主效果执行
            if (ef == MoveEffect::Damage) {
                if (!target) { result.success = false; return result; }
                int dmg = calculateDamage(*move, *target);
                dmg = static_cast<int>(dmg * damageMultiplier);
                damageMultiplier = 1.0f;  // 应用后重置倍率
                target->takeDamage(dmg);
                result.value = dmg;
            }
            else if (ef == MoveEffect::HealHP) {
                int before = currentHP;
                heal(move->power);
                result.value = currentHP - before;
            }
            else if (ef == MoveEffect::HealHPPercent) {
                int before = currentHP;
                heal(currentStats.hp * move->power / 100);
                result.value = currentHP - before;
            }
            else if (ef == MoveEffect::RestoreEnergy) {
                int before = currentEnergy;
                currentEnergy = min(currentEnergy + move->power, kMaxEnergy);
                result.value = currentEnergy - before;
            }
            else if (ef >= MoveEffect::BuffAttack && ef <= MoveEffect::BuffSpeed) {
                modifyStatStage(move->effect, move->power);
                result.value = move->power;
            }
            else if (ef >= MoveEffect::DebuffAttack && ef <= MoveEffect::DebuffSpeed) {
                if (!target) { result.success = false; return result; }
                target->modifyStatStage(move->effect, -move->power);
                result.value = move->power;
            }
            else if (ef == MoveEffect::Shield) {
                shieldAmount = move->power / 100.0f;
                result.value = move->power;
            }
            else if (ef == MoveEffect::Dispel) {
                if (!target) { result.success = false; return result; }
                target->clearStatStages();
                result.value = 1;
            }
            else if (ef == MoveEffect::DispelOne) {
                if (!target) { result.success = false; return result; }
                target->clearOneBuff();
                result.value = 1;
            }
            else if (ef == MoveEffect::ClearDebuffs) {
                clearNegativeStages();
                clearStatuses();
                result.value = 1;
            }
            else if (ef == MoveEffect::ReduceAllCost) {
                for (auto& m : unlockedMoves)
                    moveCostReduce[m.name] += move->power;
                result.value = move->power;
            }
            else if (ef == MoveEffect::NextReduceCost) {
                nextCostReduction = move->power;
                result.value = move->power;
            }
            else if (ef == MoveEffect::BoostAllPower) {
                for (auto& m : unlockedMoves)
                    movePowerBonus[m.name] += move->power;
                result.value = move->power;
            }
            else if (ef == MoveEffect::ApplyFreeze) {
                if (!target) { result.success = false; return result; }
                target->applyStatus(StatusType::Freeze, move->power);
                onAppliedStatus(target, StatusType::Freeze, move->power);
                result.value = move->power;
            }
            else if (ef == MoveEffect::ApplyPoison) {
                if (!target) { result.success = false; return result; }
                target->applyStatus(StatusType::Poison, move->power);
                onAppliedStatus(target, StatusType::Poison, move->power);
                result.value = move->power;
            }
            else if (ef == MoveEffect::ApplyBurn) {
                if (!target) { result.success = false; return result; }
                target->applyStatus(StatusType::Burn, move->power);
                onAppliedStatus(target, StatusType::Burn, move->power);
                result.value = move->power;
            }
            else if (ef == MoveEffect::EnemyLoseEnergy) {
                if (!target) { result.success = false; return result; }
                int drained = min(target->currentEnergy, move->power);
                target->currentEnergy -= drained;
                result.value = drained;
            }

            // 自费用缩减：每次使用此技能后永久降低其费用
            if (move->costSelfReduce > 0)
                moveCostReduce[move->name] += move->costSelfReduce;

            // 连锁副效果执行
            MoveEffect extras[] = { move->secondaryEffect, move->tertiaryEffect, move->quaternaryEffect };
            int extraPowers[] = { move->secondaryPower, move->tertiaryPower, move->quaternaryPower };
            for (int i = 0; i < 3; i++) {
                if (extras[i] != MoveEffect::None)
                    applyExtraEffect(extras[i], extraPowers[i], target);
            }

            // 特性钩子
            onMoveUsed(*move);

            // 更新使用计数器
            elementUseCounts[int(move->element)]++;
            moveUseCount[move->name]++;

            return result;
        }

        // 回合结束处理

        // 处理回合结束
        void processTurnEndEffects() {
            for (auto& m : unlockedMoves) {
                if (m.turnEndPowerBonus > 0)
                    movePowerBonus[m.name] += m.turnEndPowerBonus;
            }
            processStatusEffects();
            onTurnEnd();
        }

        // 应对系统

        // 应用应对效果
        void applyCounterEffect(const Move& move, Codemon* target) {
            if (!move.isCounter) return;
            if (move.counterEffect != MoveEffect::None) {
                if (move.counterEffect == MoveEffect::ReduceSelfCost) {
                    moveCostReduce[move.name] += move.counterPower;
                }
                else if (move.counterEffect == MoveEffect::Damage) {
                    if (target) target->takeDamage(move.counterPower);
                }
                else {
                    applyExtraEffect(move.counterEffect, move.counterPower, target);
                }
            }
            onCounterTrigger(target);
        }

        // HP管理

        // 受到伤害
        virtual void takeDamage(int dmg) {
            int reduced = static_cast<int>(dmg * (1.0f - shieldAmount));
            currentHP = max(0, currentHP - reduced);
        }

        // 治疗
        void heal(int amount) {
            currentHP = min(currentStats.hp, currentHP + amount);
        }

        bool isFainted() const { return currentHP <= 0; }

        // 休息

        // 完全恢复
        void rest() {
            currentHP = currentStats.hp;
            currentEnergy = kMaxEnergy;
            clearStatStages();
            clearShield();
            clearElementUseCount();
            clearStatuses();
            nextCostReduction = 0;
            allCostIncrease = 0;
            damageMultiplier = 1.0f;
            onRest();
        }

        // 护盾系统

        void setShield(float percent) { shieldAmount = percent; }
        void clearShield() { shieldAmount = 0.0f; }
        float getShield() const { return shieldAmount; }

        // 战斗计数器管理

        // 清除所有战斗计数器
        void clearElementUseCount() {
            for (int& c : elementUseCounts) c = 0;
            moveUseCount.clear();
            moveCostReduce.clear();
            movePowerBonus.clear();
        }

        int getElementUseCount(Element e) const { return elementUseCounts[int(e)]; }

        // 异常状态系统

        // 施加异常状态
        void applyStatus(StatusType type, int stacks) {
            // 元素免疫检测
            if (type == StatusType::Poison && (type1 == Element::Poison || type2 == Element::Poison))
                return;
            if (type == StatusType::Burn && (type1 == Element::Fire || type2 == Element::Fire))
                return;
            if (type == StatusType::Freeze && (type1 == Element::Ice || type2 == Element::Ice))
                return;
            statusStacks[type] += stacks;
        }

        bool hasStatus(StatusType type) const {
            auto it = statusStacks.find(type);
            return it != statusStacks.end() && it->second > 0;
        }

        int getStatusStacks(StatusType type) const {
            auto it = statusStacks.find(type);
            return it != statusStacks.end() ? it->second : 0;
        }

        void clearStatuses() {
            statusStacks.clear();
        }

        // 结算异常状态伤害
        void processStatusEffects() {
            for (auto& [type, stacks] : statusStacks) {
                if (stacks <= 0) continue;
                if (type == StatusType::Poison) {
                    int dmg = currentStats.hp * (2 * stacks) / 100;
                    currentHP = max(0, currentHP - dmg);
                }
                else if (type == StatusType::Burn) {
                    int dmg = currentStats.hp * (3 * stacks) / 100;
                    currentHP = max(0, currentHP - dmg);
                }
                else if (type == StatusType::Freeze) {
                    int threshold = currentStats.hp * (2 * stacks) / 100;
                    if (currentHP < threshold)
                        currentHP = 0;
                }
            }
        }

        // 属性访问器
        const string& getName() const { return name; }
        int getHP() const { return currentHP; }
        int getMaxHP() const { return currentStats.hp; }
        int getEnergy() const { return currentEnergy; }
        int getLevel() const { return level; }
        int getExperience() const { return experience; }
        Element getType1() const { return type1; }
        Element getType2() const { return type2; }
        const Stats& getCurrentStats() const { return currentStats; }
        const Stats& getBaseStats() const { return baseStats; }
        const StatStages& getStatStages() const { return stages; }
        const vector<Move>& getUnlockedMoves() const { return unlockedMoves; }
        const vector<MovePoolEntry>& getSpeciesMovePool() const { return speciesMovePool; }

        // 获取已装备技能的指针列表
        vector<const Move*> getEquippedMoves() const {
            vector<const Move*> result;
            for (int idx : equippedIndices) {
                if (idx >= 0 && idx < static_cast<int>(unlockedMoves.size()))
                    result.push_back(&unlockedMoves[idx]);
            }
            return result;
        }

        const std::vector<int>& getEquippedIndices() const { return equippedIndices; }
        int getEquippedCount() const { return static_cast<int>(equippedIndices.size()); }
        const auto& getEquippedItems() const { return equippedItems; }

        bool isSlotEquipped(EquipSlot slot) const {
            return equippedItems.find(slot) != equippedItems.end();
        }

        ItemID getEquippedItemInSlot(EquipSlot slot) const {
            auto it = equippedItems.find(slot);
            if (it != equippedItems.end())
                return it->second;
            return ItemID::BasicHealthPotion;
        }
    };

} 
