#include "../include/ItemDB.h"
#include <algorithm>

// 定义静态成员
std::map<ItemID, std::unique_ptr<Item>> ItemDB::database;
std::unordered_map<ItemID, std::string> ItemDB::texturePaths;

// 实现初始化
void ItemDB::initialize()
{
    // 如果已经初始化过，先清空
    database.clear();
    
    //消耗品
    ItemDB::registerTexturePath(ItemID::BasicHealthPotion, "Resources/Items/BasicHealthPotion.png");
    ItemDB::registerTexturePath(ItemID::NormalHealthPotion, "Resources/Items/NormalHealthPotion.png");
    ItemDB::registerTexturePath(ItemID::AdvancedHealthPotion, "Resources/Items/AdvancedHealthPotion.png");

    ItemDB::registerTexturePath(ItemID::BasicAttackPotion, "Resources/Items/BasicAttackPotion.png");
    ItemDB::registerTexturePath(ItemID::NormalAttackPotion, "Resources/Items/NormalAttackPotion.png");
    ItemDB::registerTexturePath(ItemID::AdvancedAttackPotion, "Resources/Items/AdvancedAttackPotion.png");

    ItemDB::registerTexturePath(ItemID::BasicMagicAttackPotion, "Resources/Items/BasicMagicAttackPotion.png");
    ItemDB::registerTexturePath(ItemID::NormalMagicAttackPotion, "Resources/Items/NormalMagicAttackPotion.png");
    ItemDB::registerTexturePath(ItemID::AdvancedMagicAttackPotion, "Resources/Items/AdvancedMagicAttackPotion.png");

    ItemDB::registerTexturePath(ItemID::BasicDefencePotion, "Resources/Items/BasicDefencePotion.png");
    ItemDB::registerTexturePath(ItemID::NormalDefencePotion, "Resources/Items/NormalDefencePotion.png");
    ItemDB::registerTexturePath(ItemID::AdvancedDefencePotion, "Resources/Items/AdvancedDefencePotion.png");

    ItemDB::registerTexturePath(ItemID::BasicMagicDefencePotion, "Resources/Items/BasicMagicDefencePotion.png");
    ItemDB::registerTexturePath(ItemID::NormalMagicDefencePotion, "Resources/Items/NormalMagicDefencePotion.png");
    ItemDB::registerTexturePath(ItemID::AdvancedMagicDefencePotion, "Resources/Items/AdvancedMagicDefencePotion.png");

    ItemDB::registerTexturePath(ItemID::EnergyPotion, "Resources/Items/EnergyPotion.png");

    ItemDB::registerTexturePath(ItemID::PurificationPotion, "Resources/Items/PurificationPotion.png");

    ItemDB::registerTexturePath(ItemID::BloodragePotion, "Resources/Items/BloodragePotion.png");
    ItemDB::registerTexturePath(ItemID::StonehidePotion, "Resources/Items/StonehidePotion.png");
    ItemDB::registerTexturePath(ItemID::SacredElixir, "Resources/Items/SacredElixir.png");
    ItemDB::registerTexturePath(ItemID::ThiefPotion, "Resources/Items/ThiefPotion.png");
    ItemDB::registerTexturePath(ItemID::ArcanePotion, "Resources/Items/ArcanePotion.png");
    ItemDB::registerTexturePath(ItemID::GiantPotion, "Resources/Items/GiantPotion.png");
    ItemDB::registerTexturePath(ItemID::VoidPotion, "Resources/Items/VoidPotion.png");

    //装备
    ItemDB::registerTexturePath(ItemID::CopperSword, "Resources/Items/CopperSword.png");
    ItemDB::registerTexturePath(ItemID::IronSword, "Resources/Items/IronSword.png");
    ItemDB::registerTexturePath(ItemID::SteelSword, "Resources/Items/SteelSword.png");
    ItemDB::registerTexturePath(ItemID::DiamondSword, "Resources/Items/DiamondSword.png");
    ItemDB::registerTexturePath(ItemID::DragonBoneSword, "Resources/Items/DragonBoneSword.png");
    ItemDB::registerTexturePath(ItemID::CopperWand, "Resources/Items/CopperWand.png");
    ItemDB::registerTexturePath(ItemID::IronWand, "Resources/Items/IronWand.png");
    ItemDB::registerTexturePath(ItemID::CrystalWand, "Resources/Items/CrystalWand.png");
    ItemDB::registerTexturePath(ItemID::DiamondWand, "Resources/Items/DiamondWand.png");
    ItemDB::registerTexturePath(ItemID::DragonBoneWand, "Resources/Items/DragonBoneWand.png");

    ItemDB::registerTexturePath(ItemID::LeatherArmour, "Resources/Items/LeatherArmour.png");
    ItemDB::registerTexturePath(ItemID::CopperArmour, "Resources/Items/CopperArmour.png");
    ItemDB::registerTexturePath(ItemID::IronArmour, "Resources/Items/IronArmour.png");
    ItemDB::registerTexturePath(ItemID::SteelArmour, "Resources/Items/SteelArmour.png");
    ItemDB::registerTexturePath(ItemID::CrystalArmour, "Resources/Items/CrystalArmour.png");
    ItemDB::registerTexturePath(ItemID::DiamondArmour, "Resources/Items/DiamondArmour.png");
    ItemDB::registerTexturePath(ItemID::DragonScaleArmour, "Resources/Items/DragonScaleArmour.png");

    ItemDB::registerTexturePath(ItemID::IronNecklace, "Resources/Items/IronNecklace.png");
    ItemDB::registerTexturePath(ItemID::GoldenNecklace, "Resources/Items/GoldenNecklace.png");
    ItemDB::registerTexturePath(ItemID::RubyNecklace, "Resources/Items/RubyNecklace.png");
    ItemDB::registerTexturePath(ItemID::SapphireNecklace, "Resources/Items/SapphireNecklace.png");
    ItemDB::registerTexturePath(ItemID::EmeraldNecklace, "Resources/Items/EmeraldNecklace.png");
    ItemDB::registerTexturePath(ItemID::DiamondNecklace, "Resources/Items/DiamondNecklace.png");
    ItemDB::registerTexturePath(ItemID::DragonBoneNecklace, "Resources/Items/DragonBoneNecklace.png");

    ItemDB::registerTexturePath(ItemID::IronRing, "Resources/Items/IronRing.png");
    ItemDB::registerTexturePath(ItemID::QuartzRing, "Resources/Items/QuartzRing.png");
    ItemDB::registerTexturePath(ItemID::GoldenRing, "Resources/Items/GoldenRing.png");
    ItemDB::registerTexturePath(ItemID::RubyRing, "Resources/Items/RubyRing.png");
    ItemDB::registerTexturePath(ItemID::SapphireRing, "Resources/Items/SapphireRing.png");
    ItemDB::registerTexturePath(ItemID::EmeraldRing, "Resources/Items/EmeraldRing.png");
    ItemDB::registerTexturePath(ItemID::DiamondRing, "Resources/Items/DiamondRing.png");
    ItemDB::registerTexturePath(ItemID::DragonBoneRing, "Resources/Items/DragonBoneRing.png");

    //钥匙
    ItemDB::registerTexturePath(ItemID::KEY01, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY02, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY11, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY12, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY21, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY22, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY31, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY32, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY41, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY42, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY51, "Resources/Items/Key.png");
    ItemDB::registerTexturePath(ItemID::KEY52, "Resources/Items/Key.png");

    // 定义道具模板列表
    std::vector<ItemTemplate> templates = {
        //消耗品
        {
            ItemID::BasicHealthPotion,
            "初级治疗药水",
            "恢复10%血量",
            100,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::HealHPPercent, 10}
            }
        },
        {
            ItemID::NormalHealthPotion,
            "中级治疗药水",
            "恢复20%血量",
            200,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::HealHPPercent, 20}
            }
        },
        {
            ItemID::AdvancedHealthPotion,
            "高级治疗药水",
            "恢复30%血量",
            300,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::HealHPPercent, 30}
            }
        },
        {
            ItemID::BasicAttackPotion,
            "初级物攻药水",
            "增加20点物理攻击",
            100,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffAttack, 20}
            }
        },
        {
            ItemID::NormalAttackPotion,
            "中级物攻药水",
            "增加40点物理攻击",
            200,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffAttack, 40}
            }
        },
        {
            ItemID::AdvancedAttackPotion,
            "高级物攻药水",
            "增加60点物理攻击",
            300,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffAttack, 60}
            }
        },
        {
            ItemID::BasicMagicAttackPotion,
            "初级魔攻药水",
            "增加20点魔法攻击",
            100,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffMagicAttack, 20}
            }
        },
        {
            ItemID::NormalMagicAttackPotion,
            "中级魔攻药水",
            "增加40点魔法攻击",
            200,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffMagicAttack, 40}
            }
        },
        {
            ItemID::AdvancedMagicAttackPotion,
            "高级魔攻药水",
            "增加60点魔法攻击",
            300,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffMagicAttack, 60}
            }
        },
        {
            ItemID::BasicDefencePotion,
            "初级物防药水",
            "增加20点物理防御",
            100,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffDefense, 20}
            }
        },
        {
            ItemID::NormalDefencePotion,
            "中级物防药水",
            "增加40点物理防御",
            200,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffDefense, 40}
            }
        },
        {
            ItemID::AdvancedDefencePotion,
            "高级物防药水",
            "增加60点物理防御",
            300,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffDefense, 60}
            }
        },
        {
            ItemID::BasicMagicDefencePotion,
            "初级魔防药水",
            "增加20点魔法防御",
            100,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffMagicDefense, 20}
            }
        },
        {
            ItemID::NormalMagicDefencePotion,
            "中级魔防药水",
            "增加40点魔法防御",
            200,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffMagicDefense, 40}
            }
        },
        {
            ItemID::AdvancedMagicDefencePotion,
            "高级魔防药水",
            "增加60点魔法防御",
            300,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffMagicDefense, 60}
            }
        },
        {
            ItemID::EnergyPotion,
            "能量药水",
            "恢复全部能量",
            200,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::RestoreEnergy, 10}
            }
        },
        {
            ItemID::PurificationPotion,
            "净化药水",
            "清除所有负面效果",
            250,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::ClearDebuffs, 0}
            }
        },
        {
            ItemID::BloodragePotion,
            "血怒药水",
            "增加120点物理攻击，增加120点魔法攻击，减少30%当前生命值",
            450,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffAttack, 120},
                {monbase::MoveEffect::BuffMagicAttack, 120},
                {monbase::MoveEffect::HealHPPercent, -30}
            }
        },
        {
            ItemID::StonehidePotion,
            "石肤药水",
            "增加80点物理防御，增加80点魔法防御",
            450,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffDefense, 80},
                {monbase::MoveEffect::BuffMagicDefense, 80}
            }
        },
        {
            ItemID::SacredElixir,
            "神圣药水",
            "清除所有负面效果，恢复满所有能量，恢复25%生命值",
            450,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::ClearDebuffs, 0},
                {monbase::MoveEffect::RestoreEnergy, 10},
                {monbase::MoveEffect::HealHPPercent, 25}
            }
        },
        {
            ItemID::ThiefPotion,
            "盗贼药水",
            "增加100点物理攻击，增加20点速度",
            450,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffAttack, 100},
                {monbase::MoveEffect::BuffSpeed, 20}
            }
        },
        {
            ItemID::ArcanePotion,
            "奥术药水",
            "增加100点魔法攻击，恢复满所有能量",
            450,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffMagicAttack, 100},
                {monbase::MoveEffect::RestoreEnergy, 10}
            }
        },
        {
            ItemID::GiantPotion,
            "巨人药水",
            "增加80点物理攻击，增加80点物理防御",
            450,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffAttack, 80},
                {monbase::MoveEffect::BuffDefense, 80}
            }
        },
        {
            ItemID::VoidPotion,
            "虚空药水",
            "增加100点魔法攻击，增加20点速度",
            450,
            ItemType::CONSUMABLE,
            EquipSlot::NONE,
            {
                {monbase::MoveEffect::BuffMagicAttack, 100},
                {monbase::MoveEffect::BuffSpeed, 20}
            }
        },

        //装备
        //武器
        {
            ItemID::CopperSword,
            "铜剑",
            "增加20点物理攻击",
            200,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffAttack, 20}
            }
        },
        {
            ItemID::IronSword,
            "铁剑",
            "增加40点物理攻击",
            400,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffAttack, 40}
            }
        },
        {
            ItemID::SteelSword,
            "钢剑",
            "增加60点物理攻击",
            600,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffAttack, 60}
            }
        },
        {
            ItemID::DiamondSword,
            "钻石剑",
            "增加80点物理攻击",
            1000,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffAttack, 80}
            }
        },
        {
            ItemID::DragonBoneSword,
            "龙骨剑",
            "增加100点物理攻击",
            1500,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffAttack, 100}
            }
        },
        {
            ItemID::CopperWand,
            "铜法杖",
            "增加20点魔法攻击",
            200,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffMagicAttack, 20}
            }
        },
        {
            ItemID::IronWand,
            "铁法杖",
            "增加40点魔法攻击",
            400,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffMagicAttack, 40}
            }
        },
        {
            ItemID::CrystalWand,
            "水晶法杖",
            "增加60点魔法攻击",
            600,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffMagicAttack, 60}
            }
        },
        {
            ItemID::DiamondWand,
            "钻石法杖",
            "增加80点魔法攻击",
            1000,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffMagicAttack, 80}
            }
        },
        {
            ItemID::DragonBoneWand,
            "龙骨法杖",
            "增加100点魔法攻击",
            1500,
            ItemType::EQUIPMENT,
            EquipSlot::WEAPON,
            {
                {monbase::MoveEffect::BuffMagicAttack, 100}
            }
        },
        //护甲
        {
            ItemID::LeatherArmour,
            "皮质护甲",
            "增加10点物理防御，增加10点魔法防御",
            100,
            ItemType::EQUIPMENT,
            EquipSlot::ARMOR,
            {
                {monbase::MoveEffect::BuffDefense, 10},
                {monbase::MoveEffect::BuffMagicDefense, 10}
            }
        },
        {
            ItemID::CopperArmour,
            "铜护甲",
            "增加20点物理防御，增加20点魔法防御",
            200,
            ItemType::EQUIPMENT,
            EquipSlot::ARMOR,
            {
                {monbase::MoveEffect::BuffDefense, 20},
                {monbase::MoveEffect::BuffMagicDefense, 20}
            }
        },
        {
            ItemID::IronArmour,
            "铁护甲",
            "增加40点物理防御，增加40点魔法防御",
            400,
            ItemType::EQUIPMENT,
            EquipSlot::ARMOR,
            {
                {monbase::MoveEffect::BuffDefense, 40},
                {monbase::MoveEffect::BuffMagicDefense, 40}
            }
        },
        {
            ItemID::SteelArmour,
            "钢护甲",
            "增加60点物理防御，增加40点魔法防御",
            600,
            ItemType::EQUIPMENT,
            EquipSlot::ARMOR,
            {
                {monbase::MoveEffect::BuffDefense, 60},
                {monbase::MoveEffect::BuffMagicDefense, 40}
            }
        },
        {
            ItemID::CrystalArmour,
            "水晶护甲",
            "增加40点物理防御，增加60点魔法防御",
            600,
            ItemType::EQUIPMENT,
            EquipSlot::ARMOR,
            {
                {monbase::MoveEffect::BuffDefense, 40},
                {monbase::MoveEffect::BuffMagicDefense, 60}
            }
        },
        {
            ItemID::DiamondArmour,
            "钻石护甲",
            "增加80点物理防御，增加80点魔法防御",
            1000,
            ItemType::EQUIPMENT,
            EquipSlot::ARMOR,
            {
                {monbase::MoveEffect::BuffDefense, 80},
                {monbase::MoveEffect::BuffMagicDefense, 80}
            }
        },
        {
            ItemID::DragonScaleArmour,
            "龙鳞护甲",
            "增加100点物理防御，增加100点魔法防御，增加10点速度",
            1500,
            ItemType::EQUIPMENT,
            EquipSlot::ARMOR,
            {
                {monbase::MoveEffect::BuffDefense, 100},
                {monbase::MoveEffect::BuffMagicDefense, 100},
                {monbase::MoveEffect::BuffSpeed, 10}
            }
        },
        //项链
        {
            ItemID::IronNecklace,
            "铁项链",
            "增加10点物理攻击，增加10点物理防御",
            200,
            ItemType::EQUIPMENT,
            EquipSlot::NECKLACE,
            {
                {monbase::MoveEffect::BuffAttack, 10},
                {monbase::MoveEffect::BuffDefense, 10}
            }
        },
        {
            ItemID::GoldenNecklace,
            "金项链",
            "增加10点魔法攻击，增加10点魔法防御",
            200,
            ItemType::EQUIPMENT,
            EquipSlot::NECKLACE,
            {
                {monbase::MoveEffect::BuffMagicAttack, 10},
                {monbase::MoveEffect::BuffMagicDefense, 10}
            }
        },
        {
            ItemID::RubyNecklace,
            "红宝石项链",
            "增加20点物理攻击，增加20点物理防御",
            500,
            ItemType::EQUIPMENT,
            EquipSlot::NECKLACE,
            {
                {monbase::MoveEffect::BuffAttack, 20},
                {monbase::MoveEffect::BuffDefense, 20}
            }
        },
        {
            ItemID::SapphireNecklace,
            "蓝宝石项链",
            "增加20点魔法攻击，增加20点魔法防御",
            500,
            ItemType::EQUIPMENT,
            EquipSlot::NECKLACE,
            {
                {monbase::MoveEffect::BuffMagicAttack, 20},
                {monbase::MoveEffect::BuffMagicDefense, 20}
            }
        },
        {
            ItemID::EmeraldNecklace,
            "绿宝石项链",
            "增加20点物理防御，增加20点魔法防御",
            500,
            ItemType::EQUIPMENT,
            EquipSlot::NECKLACE,
            {
                {monbase::MoveEffect::BuffDefense, 20},
                {monbase::MoveEffect::BuffMagicDefense, 20}
            }
        },
        {
            ItemID::DiamondNecklace,
            "钻石项链",
            "增加20点物攻，增加30点物防，增加20点魔攻，增加30点魔防",
            800,
            ItemType::EQUIPMENT,
            EquipSlot::NECKLACE,
            {
                {monbase::MoveEffect::BuffAttack, 20},
                {monbase::MoveEffect::BuffDefense, 30},
                {monbase::MoveEffect::BuffMagicAttack, 20},
                {monbase::MoveEffect::BuffMagicDefense, 30}
            }
        },
        {
            ItemID::DragonBoneNecklace,
            "龙骨项链",
            "增加30点物攻，增加40点物防，增加30点魔攻，增加40点魔防",
            1200,
            ItemType::EQUIPMENT,
            EquipSlot::NECKLACE,
            {
                {monbase::MoveEffect::BuffAttack, 30},
                {monbase::MoveEffect::BuffDefense, 40},
                {monbase::MoveEffect::BuffMagicAttack, 30},
                {monbase::MoveEffect::BuffMagicDefense, 40}
            }
        },
        //戒指
        {
            ItemID::IronRing,
            "铁戒指",
            "增加10点物理攻击，增加10点物理防御",
            200,
            ItemType::EQUIPMENT,
            EquipSlot::RING,
            {
                {monbase::MoveEffect::BuffAttack, 10},
                {monbase::MoveEffect::BuffDefense, 10}
            }
        },
        {
            ItemID::QuartzRing,
            "石英戒指",
            "增加10点速度",
            200,
            ItemType::EQUIPMENT,
            EquipSlot::RING,
            {
                {monbase::MoveEffect::BuffSpeed, 10}
            }
        },
        {
            ItemID::GoldenRing,
            "金戒指",
            "增加10点魔法攻击，增加10点魔法防御",
            200,
            ItemType::EQUIPMENT,
            EquipSlot::RING,
            {
                {monbase::MoveEffect::BuffMagicAttack, 10},
                {monbase::MoveEffect::BuffMagicDefense, 10}
            }
        },
        {
            ItemID::RubyRing,
            "红宝石戒指",
            "增加20点物理攻击，增加20点物理防御",
            500,
            ItemType::EQUIPMENT,
            EquipSlot::RING,
            {
                {monbase::MoveEffect::BuffAttack, 20},
                {monbase::MoveEffect::BuffDefense, 20}
            }
        },
        {
            ItemID::SapphireRing,
            "蓝宝石戒指",
            "增加20点魔法攻击，增加20点魔法防御",
            500,
            ItemType::EQUIPMENT,
            EquipSlot::RING,
            {
                {monbase::MoveEffect::BuffMagicAttack, 20},
                {monbase::MoveEffect::BuffMagicDefense, 20}
            }
        },
        {
            ItemID::EmeraldRing,
            "绿宝石戒指",
            "增加20点物理防御，增加20点魔法防御",
            500,
            ItemType::EQUIPMENT,
            EquipSlot::RING,
            {
                {monbase::MoveEffect::BuffDefense, 20},
                {monbase::MoveEffect::BuffMagicDefense, 20}
            }
        },
        {
            ItemID::DiamondRing,
            "钻石戒指",
            "增加30点物攻，增加20点物防，增加30点魔攻，增加20点魔防",
            800,
            ItemType::EQUIPMENT,
            EquipSlot::RING,
            {
                {monbase::MoveEffect::BuffAttack, 30},
                {monbase::MoveEffect::BuffDefense, 20},
                {monbase::MoveEffect::BuffMagicAttack, 30},
                {monbase::MoveEffect::BuffMagicDefense, 20}
            }
        },
        {
            ItemID::DragonBoneRing,
            "龙骨戒指",
            "增加40点物攻，增加30点物防，增加40点魔攻，增加30点魔防",
            1200,
            ItemType::EQUIPMENT,
            EquipSlot::RING,
            {
                {monbase::MoveEffect::BuffAttack, 40},
                {monbase::MoveEffect::BuffDefense, 30},
                {monbase::MoveEffect::BuffMagicAttack, 40},
                {monbase::MoveEffect::BuffMagicDefense, 30}
            }
        },

        //钥匙
        {
            ItemID::KEY01,
            "钥匙01",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY02,
            "钥匙02",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY11,
            "钥匙11",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY12,
            "钥匙12",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY21,
            "钥匙21",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY22,
            "钥匙22",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY31,
            "钥匙31",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY32,
            "钥匙32",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY41,
            "钥匙41",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY42,
            "钥匙42",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY51,
            "钥匙51",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        },
        {
            ItemID::KEY52,
            "钥匙52",
            "这是一把钥匙",
            0,
            ItemType::KEY,
            EquipSlot::NONE,
            {}
        }
    };

    // 循环生成道具并存入数据库
    for (const auto& t : templates)
    {
        auto item = std::make_unique<Item>(t.id, t.name, t.desc, t.price, t.type, t.slot);
        for (const auto& eff : t.effects)
        {
            item->addEffect(std::make_unique<Effect>(eff.first, eff.second));
        }
        database[t.id] = std::move(item);
    }
}

// 实现获取道具
std::unique_ptr<Item> ItemDB::getItem(ItemID id)
{
    auto it = database.find(id);
    if (it != database.end())
    {
        return it->second->clone();
    }
    return nullptr;
}

void ItemDB::registerTexturePath(ItemID id, const std::string& path)
{
    texturePaths[id] = path;
}

std::string ItemDB::getTexturePath(ItemID id)
{
    auto it = texturePaths.find(id);
    if (it != texturePaths.end()) {
        return it->second;
    }
    return "";
}