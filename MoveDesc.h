#pragma once
#include "types.h"
#include <string>

namespace monbase
{

    struct MoveResult
    {
        bool success = false;
        MoveEffect effect = MoveEffect::Damage;
        int value = 0;
        const class Move *move = nullptr;
    };

    class Move
    {
    public:
        std::string name;
        std::string description; // 技能介绍文本
        Element element = Element::None;
        MoveCategory category = MoveCategory::Attack;
        MoveEffect effect = MoveEffect::Damage;
        int power = 0;
        int energyCost = 0;
        bool isPhysical = true;

        MoveEffect secondaryEffect = MoveEffect::None;
        int secondaryPower = 0;
        MoveEffect tertiaryEffect = MoveEffect::None;
        int tertiaryPower = 0;
        MoveEffect quaternaryEffect = MoveEffect::None;
        int quaternaryPower = 0;

        // 成长参数
        int hpThresholdPercent = 0;
        int hpThresholdBonus = 0;
        int scalingBonus = 0;      // 每用1次同元素技能+这么多
        int selfScalingBonus = 0;  // 每用1次本技能+这么多
        int exponentScaling = 0;   // power *= 2^(同元素使用次数)
        int costSelfReduce = 0;    // 每次使用减费
        int costToPowerBonus = 0;  // 每减1费加这么多威力
        int costScalingBonus = 0;  // 每用1次同元素技能加费
        int turnEndPowerBonus = 0; // 每回合结束加威力

        Move() = default;

        Move(std::string n, Element e, MoveCategory cat, MoveEffect ef,
             int p, int cost, bool physical = true,
             MoveEffect secEf = MoveEffect::None, int secP = 0)
            : name(std::move(n)), element(e), category(cat), effect(ef),
              power(p), energyCost(cost), isPhysical(physical),
              secondaryEffect(secEf), secondaryPower(secP) {}

        Move &setScaling(int n)
        {
            scalingBonus = n;
            return *this;
        }
        Move &setSelfScaling(int n)
        {
            selfScalingBonus = n;
            return *this;
        }
        Move &setExpScaling(int n)
        {
            exponentScaling = n;
            return *this;
        }
        Move &setHpThreshold(int pct, int bonus)
        {
            hpThresholdPercent = pct;
            hpThresholdBonus = bonus;
            return *this;
        }
        Move &setCostReduce(int n)
        {
            costSelfReduce = n;
            return *this;
        }
        Move &setCostPower(int n)
        {
            costToPowerBonus = n;
            return *this;
        }
        Move &setCostScale(int n)
        {
            costScalingBonus = n;
            return *this;
        }
        Move &setTurnPower(int n)
        {
            turnEndPowerBonus = n;
            return *this;
        }
        Move &setDescription(std::string desc)
        {
            description = std::move(desc);
            return *this;
        }
        Move &setTertiary(MoveEffect ef, int p)
        {
            tertiaryEffect = ef;
            tertiaryPower = p;
            return *this;
        }
        Move &setQuaternary(MoveEffect ef, int p)
        {
            quaternaryEffect = ef;
            quaternaryPower = p;
            return *this;
        }

        // 应对：指定目标类别，匹配则触发额外效果
        bool isCounter = false;
        MoveCategory counterTarget = MoveCategory::Attack;
        MoveEffect counterEffect = MoveEffect::None;
        int counterPower = 0;

        Move &setCounter(MoveCategory target, MoveEffect ef, int p)
        {
            isCounter = true;
            counterTarget = target;
            counterEffect = ef;
            counterPower = p;
            return *this;
        }
    };

    struct CounterResult
    {
        bool triggers = false;
        MoveEffect extraEffect = MoveEffect::None;
        int extraPower = 0;
    };

    // 判断应对是否触发
    inline CounterResult checkCounter(const Move &ourMove, MoveCategory opponentCategory)
    {
        CounterResult result;
        if (ourMove.isCounter && ourMove.counterTarget == opponentCategory)
        {
            result.triggers = true;
            result.extraEffect = ourMove.counterEffect;
            result.extraPower = ourMove.counterPower;
        }
        return result;
    }

} 
