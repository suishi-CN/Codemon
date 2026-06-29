#pragma once
#include "move.h"

namespace monbase::moves
{

    // Normal physical attacks
    inline const Move Scratch("抓挠", Element::Normal, MoveCategory::Attack,
                              MoveEffect::Damage, 35, 0, true);
    inline const Move TailSweep("扫尾", Element::Normal, MoveCategory::Attack,
                                MoveEffect::Damage, 90, 2, true);
    inline const Move PowerTackle("猛烈撞击", Element::Normal, MoveCategory::Attack,
                                  MoveEffect::Damage, 65, 1, true);

    // Fire
    inline const Move Ember("火苗", Element::Fire, MoveCategory::Attack,
                            MoveEffect::Damage, 30, 0, true,
                            MoveEffect::RestoreEnergy, 1);
    inline const Move FlameSlash("火焰切割", Element::Fire, MoveCategory::Attack,
                                 MoveEffect::Damage, 100, 3, true);
    inline const Move FireBreath = []
    {
        Move m("吹火", Element::Fire, MoveCategory::Attack,
               MoveEffect::Damage, 50, 1, true);
        m.selfScalingBonus = 20;
        return m;
    }();
    inline const Move FireChariot("火云车", Element::Fire, MoveCategory::Attack,
                                  MoveEffect::Damage, 140, 5, true);
    inline const Move Wildfire = []
    {
        Move m("山火", Element::Fire, MoveCategory::Attack,
               MoveEffect::Damage, 15, 3, true);
        m.exponentScaling = 1;
        return m;
    }();

    // Grass
    inline const Move SieveTubeRush = []
    {
        Move m("筛管奔流", Element::Grass, MoveCategory::Attack,
               MoveEffect::Damage, 80, 3, true);
        m.hpThresholdPercent = 80;
        m.hpThresholdBonus = 7;
        return m;
    }();
    inline const Move CactusStrike("仙人掌刺击", Element::Grass, MoveCategory::Attack,
                                   MoveEffect::Damage, 150, 6, true);
    inline const Move ThornSpike("棘突", Element::Grass, MoveCategory::Attack,
                                 MoveEffect::Damage, 100, 3, false);
    inline const Move VineWring("藤绞", Element::Grass, MoveCategory::Attack,
                                MoveEffect::Damage, 80, 4, true,
                                MoveEffect::RestoreEnergy, 5);
    inline const Move ChloroBeam("叶绿光束", Element::Grass, MoveCategory::Attack,
                                 MoveEffect::Damage, 120, 4, false);
    inline const Move LightGather = []
    {
        Move m("光能聚集", Element::Grass, MoveCategory::Attack,
               MoveEffect::Damage, 100, 7, false);
        m.scalingBonus = 60;
        return m;
    }();

    // Water
    inline const Move WaterFlick("甩水", Element::Water, MoveCategory::Attack,
                                 MoveEffect::Damage, 30, 0, false,
                                 MoveEffect::RestoreEnergy, 1);
    inline const Move Bubble("气泡", Element::Water, MoveCategory::Attack,
                             MoveEffect::Damage, 100, 3, false);
    inline const Move WaterCannon = []
    {
        Move m("水炮", Element::Water, MoveCategory::Attack,
               MoveEffect::Damage, 110, 4, false);
        m.costSelfReduce = 1;
        return m;
    }();
    inline const Move Spring = []
    {
        Move m("涌泉", Element::Water, MoveCategory::Attack,
               MoveEffect::Damage, 60, 6, false);
        m.costToPowerBonus = 10;
        return m;
    }();
    inline const Move Flood = []
    {
        Move m("天洪", Element::Water, MoveCategory::Attack,
               MoveEffect::Damage, 150, 4, false);
        m.scalingBonus = 30;
        m.costScalingBonus = 1;
        return m;
    }();
    inline const Move WaterWave = []
    {
        Move m("水波术", Element::Water, MoveCategory::Attack,
               MoveEffect::Damage, 90, 6, false);
        m.turnEndPowerBonus = 20;
        return m;
    }();

    // Normal magic attacks
    inline const Move Slap("拍击", Element::Normal, MoveCategory::Attack,
                           MoveEffect::Damage, 65, 1, false);

    // Defense
    inline const Move Defend = []
    {
        Move m("防御", Element::Normal, MoveCategory::Defense,
               MoveEffect::Shield, 70, 1);
        m.setCounter(MoveCategory::Attack, MoveEffect::None, 0);
        return m;
    }();

    // Status
    inline const Move RestRecover("休息回复", Element::Normal, MoveCategory::Status,
                                  MoveEffect::HealHPPercent, 30, 2);
    inline const Move GrowthSpurt("徒长", Element::Grass, MoveCategory::Status,
                                  MoveEffect::RestoreEnergy, 10, 2);
    inline const Move Abundance("丰饶", Element::Grass, MoveCategory::Status,
                                MoveEffect::BuffAttack, 14, 3, false,
                                MoveEffect::BuffMagicAttack, 14);
    inline const Move PowerBoost("力量增效", Element::Normal, MoveCategory::Status,
                                 MoveEffect::BuffAttack, 10, 1);
    inline const Move Sunbathe("晒太阳", Element::Normal, MoveCategory::Status,
                               MoveEffect::Dispel, 1, 1);
    inline const Move Rage = []
    {
        Move m("怒火", Element::Fire, MoveCategory::Status,
               MoveEffect::BuffAttack, 12, 1, false,
               MoveEffect::BuffMagicAttack, 12);
        m.tertiaryEffect = MoveEffect::DebuffDefense;
        m.tertiaryPower = 4;
        m.quaternaryEffect = MoveEffect::DebuffMagicDefense;
        m.quaternaryPower = 4;
        return m;
    }();
    inline const Move MagicBoost("魔法增效", Element::Normal, MoveCategory::Status,
                                 MoveEffect::BuffMagicAttack, 7, 0);
    inline const Move Purification("洗礼", Element::Water, MoveCategory::Status,
                                   MoveEffect::ClearDebuffs, 1, 1, false,
                                   MoveEffect::ReduceAllCost, 1);

    // 水泡盾 - 防御/水/2费, 永久减伤80%, 应对攻击→魔攻+70%
    inline const Move WaterShield = []
    {
        Move m("水泡盾", Element::Water, MoveCategory::Defense,
               MoveEffect::Shield, 80, 2);
        m.setCounter(MoveCategory::Attack, MoveEffect::BuffMagicAttack, 7);
        return m;
    }();

    // 气波 - 魔法/无/40威/0费
    inline const Move AirWave("气波", Element::Normal, MoveCategory::Attack,
                              MoveEffect::Damage, 40, 0, false);

    // 泡沫 - 物理/水/60威/1费
    inline const Move Foam("泡沫", Element::Water, MoveCategory::Attack,
                           MoveEffect::Damage, 60, 1, true);

    // 蓄水 - 状态/水/1费, 下次技能能耗-6
    inline const Move StoreWater("蓄水", Element::Water, MoveCategory::Status,
                                 MoveEffect::NextReduceCost, 6, 1);

    // 防御反击 - 防御/无/3费, 永久减伤80%, 应对攻击→全技能威力+40
    inline const Move DefendCounter = []
    {
        Move m("防御反击", Element::Normal, MoveCategory::Defense,
               MoveEffect::Shield, 80, 3);
        m.setCounter(MoveCategory::Attack, MoveEffect::BoostAllPower, 40);
        return m;
    }();

    // 化劲 - 状态/无/2费, 全技能威力+40
    inline const Move PowerFocus("化劲", Element::Normal, MoveCategory::Status,
                                 MoveEffect::BoostAllPower, 40, 2);

    // 水幕冲击 - 物理/水/100威/3费
    inline const Move WaterScreenStrike("水幕冲击", Element::Water, MoveCategory::Attack,
                                        MoveEffect::Damage, 100, 3, true);

    // 听桥 - 防御/无/4费, 永久减伤60%, 应对攻击→武系物伤（威力由战斗系统注入）
    inline const Move ListenBridge = []
    {
        Move m("听桥", Element::Normal, MoveCategory::Defense,
               MoveEffect::Shield, 60, 4);
        m.setCounter(MoveCategory::Attack, MoveEffect::Damage, 0);
        return m;
    }();

    // 一拳 - 物理/无/140威/5费
    inline const Move OnePunch("一拳", Element::Normal, MoveCategory::Attack,
                               MoveEffect::Damage, 140, 5, true);

    // 斩断 - 物理/无/70威/2费, 应对状态→打断（战斗系统处理）
    inline const Move Sever = []
    {
        Move m("斩断", Element::Normal, MoveCategory::Attack,
               MoveEffect::Damage, 70, 2, true);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 水刃 - 物理/水/115威/4费, 应对状态→本技能能耗永久-3
    inline const Move WaterBlade = []
    {
        Move m("水刃", Element::Water, MoveCategory::Attack,
               MoveEffect::Damage, 115, 4, true);
        m.setCounter(MoveCategory::Status, MoveEffect::ReduceSelfCost, 3);
        return m;
    }();

    // === 新技能（风系） ===

    // 鸣叫 - 魔法/风/40威/0费
    inline const Move Chirp("鸣叫", Element::Wind, MoveCategory::Attack,
                            MoveEffect::Damage, 40, 0, false);

    // 飞羽 - 状态/风/0费, 驱散敌方1种增益
    inline const Move FlyingFeather("飞羽", Element::Wind, MoveCategory::Status,
                                    MoveEffect::DispelOne, 1, 0);

    // 潮涌 - 物理/水/80威/2费
    inline const Move TideSurge("潮涌", Element::Water, MoveCategory::Attack,
                                MoveEffect::Damage, 80, 2, true);

    // 风矢 - 物理/风/80威/2费
    inline const Move WindArrow("风矢", Element::Wind, MoveCategory::Attack,
                                MoveEffect::Damage, 80, 2, true);

    // 翼击 - 魔法/风/50威/3费
    inline const Move WingStrike("翼击", Element::Wind, MoveCategory::Attack,
                                 MoveEffect::Damage, 50, 3, false);

    // 龙卷风 - 物理/风/70威/5费, 应对状态→威力1.5倍(战斗系统注入damageMultiplier)
    inline const Move Tornado = []
    {
        Move m("龙卷风", Element::Wind, MoveCategory::Attack,
               MoveEffect::Damage, 70, 5, true);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 冰晶坠 - 物理/冰/90威/4费, 敌方全技能能耗+1
    inline const Move IceCrystalFall = []
    {
        Move m("冰晶坠", Element::Ice, MoveCategory::Attack,
               MoveEffect::Damage, 90, 4, true);
        m.secondaryEffect = MoveEffect::IncreaseAllCost;
        m.secondaryPower = 1;
        return m;
    }();

    // 暴风雪 - 物理/冰/85威/3费, 敌方获得1层冻结
    inline const Move BlizzardIce("暴风雪", Element::Ice, MoveCategory::Attack,
                                  MoveEffect::Damage, 85, 3, true,
                                  MoveEffect::ApplyFreeze, 1);

    // 破罐破摔 - 魔法/无/80威/3费, 自身有减益时威力+60( species中实现)
    inline const Move ShatterPot("破罐破摔", Element::Normal, MoveCategory::Attack,
                                 MoveEffect::Damage, 80, 3, false);

    // 碎冰冰 - 魔法/冰/40威/3费, 敌方每1层冻结威力+20( species中实现)
    inline const Move CrushedIce("碎冰冰", Element::Ice, MoveCategory::Attack,
                                 MoveEffect::Damage, 40, 3, false);

    // 风吹雪 - 魔法/冰/30威/0费, 自身回复1能量
    inline const Move WindSnow("风吹雪", Element::Ice, MoveCategory::Attack,
                               MoveEffect::Damage, 30, 0, false,
                               MoveEffect::RestoreEnergy, 1);

    // 当头棒喝 - 物理/无/80威/3费, 应对状态威力+100(战斗系统处理)
    inline const Move BluntStrike = []
    {
        Move m("当头棒喝", Element::Normal, MoveCategory::Attack,
               MoveEffect::Damage, 80, 3, true);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 冰墙 - 防御/冰/2费, 永久减伤80%, 应对攻击→敌方获得2层冻结
    inline const Move IceWall = []
    {
        Move m("冰墙", Element::Ice, MoveCategory::Defense,
               MoveEffect::Shield, 80, 2);
        m.setCounter(MoveCategory::Attack, MoveEffect::ApplyFreeze, 2);
        return m;
    }();

    // 生日蛋糕 - 状态/无/4费, 驱散自身减益, 自身增益翻倍( species中实现)
    inline const Move BirthdayCake("生日蛋糕", Element::Normal, MoveCategory::Status,
                                   MoveEffect::ClearDebuffs, 1, 4);

    // 冬至 - 状态/冰/7费, 开启暴风雪天气8回合(天气系统待实现)
    inline const Move WinterSolstice("冬至", Element::Ice, MoveCategory::Status,
                                     MoveEffect::None, 0, 7);

    // 冰点 - 状态/冰/2费, 敌方获得5层冻结, 应对防御额外+5层
    inline const Move FreezingPoint = []
    {
        Move m("冰点", Element::Ice, MoveCategory::Status,
               MoveEffect::ApplyFreeze, 5, 2);
        m.setCounter(MoveCategory::Defense, MoveEffect::ApplyFreeze, 5);
        return m;
    }();

    // 霜降 - 状态/冰/1费, 敌方获得4层冻结
    inline const Move FrostFall("霜降", Element::Ice, MoveCategory::Status,
                                MoveEffect::ApplyFreeze, 4, 1);

    // 甜心续航 - 状态/无/3费, 回复40%生命
    inline const Move SweetEndurance("甜心续航", Element::Normal, MoveCategory::Status,
                                     MoveEffect::HealHPPercent, 40, 3);

    // 雾气环绕 - 状态/冰/1费, 回复能量=敌方技能总能耗一半(战斗系统注入具体数值)
    inline const Move FogSurround("雾气环绕", Element::Ice, MoveCategory::Status,
                                  MoveEffect::RestoreEnergy, 9, 1);


    // 反击拳 - 物理/武/50威/2费
    inline const Move CounterPunch("反击拳", Element::Fighting, MoveCategory::Attack,
                                   MoveEffect::Damage, 50, 2, true);

    // 先发制人 - 物理/普通/55威/2费, 必定先手
    inline const Move PreemptiveStrike("先发制人", Element::Normal, MoveCategory::Attack,
                                       MoveEffect::Damage, 55, 2, true);

    // 截拳 - 物理/武/85威/3费, 应对状态→回复能耗
    inline const Move JeetKuneDo = []
    {
        Move m("截拳", Element::Fighting, MoveCategory::Attack,
               MoveEffect::Damage, 85, 3, true);
        m.setCounter(MoveCategory::Status, MoveEffect::RestoreEnergy, 3);
        return m;
    }();

    // 影袭 - 物理/武/100威/3费
    inline const Move ShadowStrike("影袭", Element::Fighting, MoveCategory::Attack,
                                   MoveEffect::Damage, 100, 3, true);

    // 回旋踢 - 物理/武/80威/3费, 应对状态→威力翻倍(战斗系统处理)
    inline const Move RoundhouseKick = []
    {
        Move m("回旋踢", Element::Fighting, MoveCategory::Attack,
               MoveEffect::Damage, 80, 3, true);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 飞踢 - 物理/普通/110威/3费
    inline const Move FlyingKick("飞踢", Element::Normal, MoveCategory::Attack,
                                 MoveEffect::Damage, 110, 3, true);

    // 突袭 - 魔法/普通/70威/2费, 应对状态→威力3倍(战斗系统处理)
    inline const Move SneakAttack = []
    {
        Move m("突袭", Element::Normal, MoveCategory::Attack,
               MoveEffect::Damage, 70, 2, false);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 许愿星 - 魔法/普通/110威/3费
    inline const Move WishingStar("许愿星", Element::Normal, MoveCategory::Attack,
                                  MoveEffect::Damage, 110, 3, false);

    // 提气 - 状态/武/4费, 全技能威力+40, 应对防御额外+50
    inline const Move SpiritBoost = []
    {
        Move m("提气", Element::Fighting, MoveCategory::Status,
               MoveEffect::BoostAllPower, 40, 4);
        m.setCounter(MoveCategory::Defense, MoveEffect::BoostAllPower, 50);
        return m;
    }();

    // 主场优势 - 状态/普通/3费, 下一次攻击伤害变为三倍( species中实现)
    inline const Move HomeFieldAdvantage("主场优势", Element::Normal, MoveCategory::Status,
                                         MoveEffect::None, 0, 3);

    // 应激反应 - 状态/普通/2费, 回复25%生命；应对防御改为回复50%
    inline const Move StressResponse = []
    {
        Move m("应激反应", Element::Normal, MoveCategory::Status,
               MoveEffect::HealHPPercent, 25, 2);
        m.setCounter(MoveCategory::Defense, MoveEffect::HealHPPercent, 25);
        return m;
    }();


    // 光球 - 魔法/光/80威/2费
    inline const Move LightOrb("光球", Element::Electric, MoveCategory::Attack,
                               MoveEffect::Damage, 80, 2, false);

    // 天光 - 魔法/光/95威/3费
    inline const Move SkyLight("天光", Element::Electric, MoveCategory::Attack,
                               MoveEffect::Damage, 95, 3, false);

    // 虹光冲击 - 魔法/光/100威/3费
    inline const Move RainbowImpact("虹光冲击", Element::Electric, MoveCategory::Attack,
                                    MoveEffect::Damage, 100, 3, false);

    // 光之矛 - 物理/光/90威/3费
    inline const Move LightSpear("光之矛", Element::Electric, MoveCategory::Attack,
                                 MoveEffect::Damage, 90, 3, true);

    // 过曝 - 魔法/光/60威/3费, 每使用其他系别技能1次永久+30(species中实现)
    inline const Move Overexposure("过曝", Element::Electric, MoveCategory::Attack,
                                   MoveEffect::Damage, 60, 3, false);

    // 雷暴 - 魔法/电/55威/1费, 迸发→获得所有迸发效果(species中实现)
    inline const Move Thunderstorm("雷暴", Element::Electric, MoveCategory::Attack,
                                   MoveEffect::Damage, 55, 1, false);

    // 导电撞击 - 物理/电/40威/0费
    inline const Move ConductiveTackle("导电撞击", Element::Electric, MoveCategory::Attack,
                                       MoveEffect::Damage, 40, 0, true);

    // 超导 - 魔法/电/95威/3费, 迸发→能耗-1( species中实现)
    inline const Move Superconduct("超导", Element::Electric, MoveCategory::Attack,
                                   MoveEffect::Damage, 95, 3, false);

    // 离子火花 - 物理/电/80威/2费
    inline const Move IonSpark("离子火花", Element::Electric, MoveCategory::Attack,
                               MoveEffect::Damage, 80, 2, true);

    // 强制重启 - 魔法/电/90威/3费, 造成魔伤后重置敌方buff
    inline const Move ForceRestart("强制重启", Element::Electric, MoveCategory::Attack,
                                   MoveEffect::Damage, 90, 3, false,
                                   MoveEffect::Dispel, 1);

    // 漫反射 - 状态/光/1费, 每种系别至多1个技能威力+35(species中实现)
    inline const Move DiffuseReflection("漫反射", Element::Electric, MoveCategory::Status,
                                        MoveEffect::None, 0, 1);


    // 恐吓 - 魔法/幽/80威/2费
    inline const Move Scare("恐吓", Element::Ghost, MoveCategory::Attack,
                            MoveEffect::Damage, 80, 2, false);

    // 午夜噪音 - 魔法/幽/120威/4费
    inline const Move MidnightNoise("午夜噪音", Element::Ghost, MoveCategory::Attack,
                                    MoveEffect::Damage, 120, 4, false);

    // 幽灵爆发 - 魔法/幽/140威/5费
    inline const Move GhostBurst("幽灵爆发", Element::Ghost, MoveCategory::Attack,
                                 MoveEffect::Damage, 140, 5, false);

    // 背袭 - 魔法/幽/40威/2费, 敌方能量=0时20倍伤害(species中实现)
    inline const Move BackStrike("背袭", Element::Ghost, MoveCategory::Attack,
                                 MoveEffect::Damage, 40, 2, false);

    // 勾魂 - 状态/幽/1费, 敌方失去3能量,自身获得3能量
    inline const Move SoulHook("勾魂", Element::Ghost, MoveCategory::Status,
                               MoveEffect::EnemyLoseEnergy, 3, 1, true,
                               MoveEffect::RestoreEnergy, 3);

    // 恶作剧 - 状态/幽/1费, 敌方失去3能量, 应对防御→再失去3能量
    inline const Move Mischief = []
    {
        Move m("恶作剧", Element::Ghost, MoveCategory::Status,
               MoveEffect::EnemyLoseEnergy, 3, 1);
        m.setCounter(MoveCategory::Defense, MoveEffect::EnemyLoseEnergy, 3);
        return m;
    }();

    // 操控 - 状态/普通/1费, 敌方全技能能耗+7(持续3回合由战斗系统管理)
    inline const Move Control("操控", Element::Normal, MoveCategory::Status,
                              MoveEffect::None, 0, 1, true,
                              MoveEffect::IncreaseAllCost, 7);

    // 热身运动 - 状态/普通/2费, 攻击伤害+100%持续3回合(species中实现)
    inline const Move WarmUp("热身运动", Element::Normal, MoveCategory::Status,
                             MoveEffect::None, 0, 2);


    // 魔爪 - 物理/恶/30威/0费, 自身回复1能量
    inline const Move DarkClaw("魔爪", Element::Dark, MoveCategory::Attack,
                               MoveEffect::Damage, 30, 0, true,
                               MoveEffect::RestoreEnergy, 1);

    // 蝙蝠 - 物理/恶/65威/2费, 回复造成伤害100%(战斗系统处理)
    inline const Move BatStrike("蝙蝠", Element::Dark, MoveCategory::Attack,
                                MoveEffect::Damage, 65, 2, true);

    // 暗突袭 - 物理/恶/70威/4费, 回复50%伤害, 应对状态→威力×2(战斗系统处理)
    inline const Move DarkAmbush = []
    {
        Move m("暗突袭", Element::Dark, MoveCategory::Attack,
               MoveEffect::Damage, 70, 4, true);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 鸣沙陷阱 - 物理/地/60威/4费, 基于物防比较变威力(species中实现)
    inline const Move SandTrap("鸣沙陷阱", Element::Ground, MoveCategory::Attack,
                               MoveEffect::Damage, 60, 4, true);

    // 岩土暴击 - 物理/地/140威/8费, 每被攻击1次能耗永久-1
    inline const Move RockCrit = []
    {
        Move m("岩土暴击", Element::Ground, MoveCategory::Attack,
               MoveEffect::Damage, 140, 8, true);
        m.setCostReduce(1);
        return m;
    }();

    // 砂石冲撞 - 物理/地/80威/3费, 应对状态→自身物防+100%
    inline const Move SandRush = []
    {
        Move m("砂石冲撞", Element::Ground, MoveCategory::Attack,
               MoveEffect::Damage, 80, 3, true);
        m.setCounter(MoveCategory::Status, MoveEffect::BuffDefense, 10);
        return m;
    }();

    // 地震 - 物理/地/190威/10费
    inline const Move Earthquake("地震", Element::Ground, MoveCategory::Attack,
                                 MoveEffect::Damage, 190, 10, true);

    // 钧势 - 状态/地/3费, 自身物防+140%, 速度+20
    inline const Move BalanceStance("钧势", Element::Ground, MoveCategory::Status,
                                    MoveEffect::BuffDefense, 14, 3, false,
                                    MoveEffect::BuffSpeed, 2);

    // 力量吞噬 - 状态/恶/4费, 敌方全技能威力-20, 自身全技能威力+20
    inline const Move PowerDevour("力量吞噬", Element::Dark, MoveCategory::Status,
                                  MoveEffect::BoostAllPower, 20, 4, false,
                                  MoveEffect::None, 0); // 敌方-20由species处理

    // 泥浆铠甲 - 状态/地/2费, 自身物攻物防+60%, 应对防御→增益翻倍
    inline const Move MudArmor = []
    {
        Move m("泥浆铠甲", Element::Ground, MoveCategory::Status,
               MoveEffect::BuffAttack, 6, 2, false,
               MoveEffect::BuffDefense, 6);
        m.setCounter(MoveCategory::Defense, MoveEffect::None, 0);
        return m;
    }();


    // 诡刺 - 物理/幽/40威/0费
    inline const Move GhostStab("诡刺", Element::Ghost, MoveCategory::Attack,
                                MoveEffect::Damage, 40, 0, true);

    // 坟场搏击 - 物理/幽/180威/4费, 敌方每有1能量威力-10%(species中实现)
    inline const Move GraveyardFight("坟场搏击", Element::Ghost, MoveCategory::Attack,
                                     MoveEffect::Damage, 180, 4, true);

    // 龙吼 - 物理/龙/60威/1费
    inline const Move DragonRoar("龙吼", Element::Dragon, MoveCategory::Attack,
                                 MoveEffect::Damage, 60, 1, true);

    // 幻象 - 物理/幽/60威/1费
    inline const Move Illusion("幻象", Element::Ghost, MoveCategory::Attack,
                               MoveEffect::Damage, 60, 1, true);

    // 隼鳞 - 物理/龙/140威/5费
    inline const Move HawkScale("隼鳞", Element::Dragon, MoveCategory::Attack,
                                MoveEffect::Damage, 140, 5, true);

    // 龙之利爪 - 物理/龙/130威/5费, 吸血50%(战斗系统处理)
    inline const Move DragonClaw("龙之利爪", Element::Dragon, MoveCategory::Attack,
                                 MoveEffect::Damage, 130, 5, true);

    // 吹炎 - 物理/龙/170威/5费, 应对状态→威力×2(战斗系统处理)
    inline const Move FlameBreath = []
    {
        Move m("吹炎", Element::Dragon, MoveCategory::Attack,
               MoveEffect::Damage, 170, 5, true);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 虚化 - 防御/幽/2费, 永久减伤80%, 应对攻击→自身魔防+70%
    inline const Move Vanish = []
    {
        Move m("虚化", Element::Ghost, MoveCategory::Defense,
               MoveEffect::Shield, 80, 2);
        m.setCounter(MoveCategory::Attack, MoveEffect::BuffMagicDefense, 7);
        return m;
    }();

    // 架势 - 状态/龙/2费, 自身回复20%生命, 下次技能费用-2
    inline const Move Stance("架势", Element::Dragon, MoveCategory::Status,
                             MoveEffect::HealHPPercent, 20, 2, false,
                             MoveEffect::NextReduceCost, 2);

    // 龙威 - 状态/龙/3费, 敌方双攻-50%
    inline const Move DragonAwe("龙威", Element::Dragon, MoveCategory::Status,
                                MoveEffect::DebuffAttack, 5, 3, false,
                                MoveEffect::DebuffMagicAttack, 5);


    // 电弧 - 物理/电/80威/3费, 迸发→威力+40(战斗系统处理迸发)
    inline const Move ArcStrike("电弧", Element::Electric, MoveCategory::Attack,
                                MoveEffect::Damage, 80, 3, true);

    // 交叉闪电 - 物理/电/100威/3费
    inline const Move CrossLightning("交叉闪电", Element::Electric, MoveCategory::Attack,
                                     MoveEffect::Damage, 100, 3, true);

    // 水花四溅 - 魔法/水/80威/3费
    inline const Move WaterSplash("水花四溅", Element::Water, MoveCategory::Attack,
                                  MoveEffect::Damage, 80, 3, false);

    // 双联脉冲 - 魔法/电/50威/4费, 迸发→威力翻倍(战斗系统处理迸发)
    inline const Move DualPulse("双联脉冲", Element::Electric, MoveCategory::Attack,
                                MoveEffect::Damage, 50, 4, false);

    // 水光冲击 - 魔法/水/140威/5费
    inline const Move HydroImpact("水光冲击", Element::Water, MoveCategory::Attack,
                                  MoveEffect::Damage, 140, 5, false);

    // 集中 - 防御/电/2费, 永久减伤80%, 应对攻击→清除自身减益,下次攻击触发迸发
    inline const Move Focus = []
    {
        Move m("集中", Element::Electric, MoveCategory::Defense,
               MoveEffect::Shield, 80, 2);
        m.setCounter(MoveCategory::Attack, MoveEffect::ClearDebuffs, 1);
        return m;
    }();

    // 电磁偏转 - 防御/电/3费, 永久减伤70%, 应对攻击→下回合技能效果和费用翻倍(战斗系统处理)
    inline const Move EMDeflect = []
    {
        Move m("电磁偏转", Element::Electric, MoveCategory::Defense,
               MoveEffect::Shield, 70, 3);
        m.setCounter(MoveCategory::Attack, MoveEffect::None, 0);
        return m;
    }();

    // 电离爆破 - 状态/电/3费, 敌方速度-40
    inline const Move IonBlast("电离爆破", Element::Electric, MoveCategory::Status,
                               MoveEffect::DebuffSpeed, 4, 3);


    // 腐蚀酸液 - 魔法/毒/35威/2费, 敌方获得2层中毒
    inline const Move CorrosiveAcid("腐蚀酸液", Element::Poison, MoveCategory::Attack,
                                    MoveEffect::Damage, 35, 2, false,
                                    MoveEffect::ApplyPoison, 2);

    // 瘴气喷射 - 魔法/毒/100威/3费
    inline const Move MiasmaShot("瘴气喷射", Element::Poison, MoveCategory::Attack,
                                 MoveEffect::Damage, 100, 3, false);

    // 激流 - 魔法/水/120威/4费
    inline const Move Torrent("激流", Element::Water, MoveCategory::Attack,
                              MoveEffect::Damage, 120, 4, false);

    // 湮灭 - 魔法/普通/155威/5费
    inline const Move Annihilation("湮灭", Element::Normal, MoveCategory::Attack,
                                   MoveEffect::Damage, 155, 5, false);

    // 毒液渗透 - 魔法/毒/120威/5费, 敌方每有1层中毒能耗-1, 敌方+1中毒(species中实现)
    inline const Move VenomPenetration("毒液渗透", Element::Poison, MoveCategory::Attack,
                                       MoveEffect::Damage, 120, 5, false);

    // 鸩毒 - 魔法/毒/75威/3费, 每层中毒+10, 应对状态→+20( species中实现)
    inline const Move PoisonVenom = []
    {
        Move m("鸩毒", Element::Poison, MoveCategory::Attack,
               MoveEffect::Damage, 75, 3, false);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 不可接触 - 防御/毒/1费, 永久减伤50%+每层中毒+10%, 应对攻击
    inline const Move Untouchable = []
    {
        Move m("不可接触", Element::Poison, MoveCategory::Defense,
               MoveEffect::Shield, 50, 1);
        m.setCounter(MoveCategory::Attack, MoveEffect::None, 0);
        return m;
    }();

    // 以毒攻毒 - 状态/毒/1费, 敌方每1层中毒自身魔攻+30%(species中实现)
    inline const Move VenomCure("以毒攻毒", Element::Poison, MoveCategory::Status,
                                MoveEffect::None, 0, 1);

    // 疫病吐息 - 状态/毒/3费, 敌方获得5层中毒
    inline const Move PlagueBreath("疫病吐息", Element::Poison, MoveCategory::Status,
                                   MoveEffect::ApplyPoison, 5, 3);

    // ======== 深蓝鲸（水） ========

    // 音爆 - 魔法/普通/130威/4费
    inline const Move SonicBoom("音爆", Element::Normal, MoveCategory::Attack,
                                MoveEffect::Damage, 130, 4, false);

    // 潮汐 - 防御/水/4费, 永久减伤60%, 应对攻击→己方全技能能耗-1( species中实现)
    inline const Move Tide = []
    {
        Move m("潮汐", Element::Water, MoveCategory::Defense,
               MoveEffect::Shield, 60, 4);
        m.setCounter(MoveCategory::Attack, MoveEffect::None, 0);
        return m;
    }();

    // 水环 - 防御/水/2费, 永久减伤60%, 应对攻击→自身全技能能耗-2( species中实现)
    inline const Move WaterRing = []
    {
        Move m("水环", Element::Water, MoveCategory::Defense,
               MoveEffect::Shield, 60, 2);
        m.setCounter(MoveCategory::Attack, MoveEffect::None, 0);
        return m;
    }();

    // 叠势 - 魔法/武/50威/3费, 每成功应对1次连击数永久+50( species中实现)
    inline const Move StackingMomentum("叠势", Element::Fighting, MoveCategory::Attack,
                                       MoveEffect::Damage, 50, 3, false);

    // 打鼾 - 魔法/普通/165威/6费
    inline const Move Snore("打鼾", Element::Normal, MoveCategory::Attack,
                            MoveEffect::Damage, 165, 6, false);

    // 破防 - 状态/武/2费, 敌方双防-70%, 应对防御→被应对技能能耗永久+2
    inline const Move BreakDefense = []
    {
        Move m("破防", Element::Fighting, MoveCategory::Status,
               MoveEffect::DebuffDefense, 7, 2, false,
               MoveEffect::DebuffMagicDefense, 7);
        m.setCounter(MoveCategory::Defense, MoveEffect::None, 0);
        return m;
    }();

    // 打湿 - 状态/水/4费, 己方全技能能耗-1( species中实现)
    inline const Move Wet("打湿", Element::Water, MoveCategory::Status,
                          MoveEffect::None, 0, 4);

    // 预备势 - 状态/武/1费, 自身物攻+80%, 应对防御→敌方物防-80%
    inline const Move ReadyStance = []
    {
        Move m("预备势", Element::Fighting, MoveCategory::Status,
               MoveEffect::BuffAttack, 8, 1);
        m.setCounter(MoveCategory::Defense, MoveEffect::DebuffDefense, 8);
        return m;
    }();


    // 鞭打 - 物理/萌/40威/0费
    inline const Move Whip("鞭打", Element::Cute, MoveCategory::Attack,
                           MoveEffect::Damage, 40, 0, true);

    // 超级糖果 - 物理/萌/100威/3费
    inline const Move SuperCandy("超级糖果", Element::Cute, MoveCategory::Attack,
                                 MoveEffect::Damage, 100, 3, true);

    // 爆米花爆破 - 物理/萌/140威/5费
    inline const Move PopcornBlast("爆米花爆破", Element::Cute, MoveCategory::Attack,
                                   MoveEffect::Damage, 140, 5, true);

    // 撒娇 - 魔法/萌/60威/3费, 威力永久+20
    inline const Move CuteCharm("撒娇", Element::Cute, MoveCategory::Attack,
                                MoveEffect::Damage, 60, 3, false);

    // 鼓劲 - 状态/普通/3费, 自身魔防+170%
    inline const Move Cheer("鼓劲", Element::Normal, MoveCategory::Status,
                            MoveEffect::BuffMagicDefense, 17, 3);

    // 清洗 - 状态/水/3费, 驱散自身减益, 每有1层减益能耗-1( species中实现)
    inline const Move Wash("清洗", Element::Water, MoveCategory::Status,
                           MoveEffect::ClearDebuffs, 1, 3);


    // 啄击 - 物理/翼/30威/0费
    inline const Move Peck("啄击", Element::Flying, MoveCategory::Attack,
                           MoveEffect::Damage, 30, 0, true);

    // 疾风刺 - 物理/翼/50威/2费
    inline const Move GustStab("疾风刺", Element::Flying, MoveCategory::Attack,
                               MoveEffect::Damage, 50, 2, true);

    // 见招拆招 - 物理/普通/65威/2费, 上回合用状态技能则+55( species中实现)
    inline const Move Parry("见招拆招", Element::Normal, MoveCategory::Attack,
                            MoveEffect::Damage, 65, 2, true);

    // 扇风 - 物理/翼/75威/3费, 先手则+50%(战斗系统处理)
    inline const Move FanWind("扇风", Element::Flying, MoveCategory::Attack,
                              MoveEffect::Damage, 75, 3, true);

    // 羽刃 - 物理/翼/75威/2费, 应对状态→伤害永久+30( species中实现)
    inline const Move FeatherBlade = []
    {
        Move m("羽刃", Element::Flying, MoveCategory::Attack,
               MoveEffect::Damage, 75, 2, true);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 乘风 - 状态/翼/2费, 自身速度+120
    inline const Move RideWind("乘风", Element::Flying, MoveCategory::Status,
                               MoveEffect::BuffSpeed, 12, 2);

    // 羽化加速 - 状态/翼/2费, 自身全技能威力+30
    inline const Move FeatherBoost("羽化加速", Element::Flying, MoveCategory::Status,
                                   MoveEffect::BoostAllPower, 30, 2);

    // 三连破 - 状态/普通/1费, 自身物攻+90%
    inline const Move TripleBreak("三连破", Element::Normal, MoveCategory::Status,
                                  MoveEffect::BuffAttack, 9, 1);

    // 快速移动 - 状态/普通/1费, 自身速度+80, 应对防御→速度+160
    inline const Move QuickMove = []
    {
        Move m("快速移动", Element::Normal, MoveCategory::Status,
               MoveEffect::BuffSpeed, 8, 1);
        m.setCounter(MoveCategory::Defense, MoveEffect::BuffSpeed, 8);
        return m;
    }();


    // 炎息 - 物理/火/40威/0费
    inline const Move FlameBreathLight("炎息", Element::Fire, MoveCategory::Attack,
                                       MoveEffect::Damage, 40, 0, true);

    // 灼伤 - 物理/火/120威/4费
    inline const Move BurnStrike("灼伤", Element::Fire, MoveCategory::Attack,
                                 MoveEffect::Damage, 120, 4, true);

    // 闪燃 - 物理/火/40威/1费, 应对状态→威力×4(战斗系统处理)
    inline const Move FlashBurn = []
    {
        Move m("闪燃", Element::Fire, MoveCategory::Attack,
               MoveEffect::Damage, 40, 1, true);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 持续高温 - 魔法/火/70威/2费, 应对状态→下次攻击翻倍( species中实现)
    inline const Move HeatWave = []
    {
        Move m("持续高温", Element::Fire, MoveCategory::Attack,
               MoveEffect::Damage, 70, 2, false);
        m.setCounter(MoveCategory::Status, MoveEffect::None, 0);
        return m;
    }();

    // 焚烧烙印 - 状态/火/3费, 清除双方所有状态, 每驱散1层敌方+5灼烧( species中实现)
    inline const Move BurnBrand("焚烧烙印", Element::Fire, MoveCategory::Status,
                                MoveEffect::None, 0, 3);

    // 怒火 - 状态/火/1费, 自身双攻+120%, 双防-40%
    inline const Move Fury = []
    {
        Move m("怒火", Element::Fire, MoveCategory::Status,
               MoveEffect::BuffAttack, 12, 1, false,
               MoveEffect::BuffMagicAttack, 12);
        m.tertiaryEffect = MoveEffect::DebuffDefense;
        m.tertiaryPower = 4;
        m.quaternaryEffect = MoveEffect::DebuffMagicDefense;
        m.quaternaryPower = 4;
        return m;
    }();

    // 充分燃烧 - 状态/火/3费, 敌方灼烧翻倍并触发1次灼烧伤害
    inline const Move FullBurn("充分燃烧", Element::Fire, MoveCategory::Status,
                               MoveEffect::None, 0, 3);

    // 热身 - 状态/火/1费, 下次攻击×2, 应对防御→×4
    inline const Move WarmUpFire("热身", Element::Fire, MoveCategory::Status,
                                 MoveEffect::None, 0, 1);

} 
