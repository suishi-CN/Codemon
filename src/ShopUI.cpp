#include "MapManager.h"
#include "Map.h"
#include "ItemDB.h"
#include <memory>
#include <iostream>
using namespace std;

ChestRewardData CreateChestX1(int index)
{
    ChestRewardData chest;

    switch (index)
    {
    case 0:
        chest.exp = 90;
        chest.gold = 60;
        chest.items.push_back(ItemDB::getItem(ItemID::KEY01));
        break;

    case 1:
        chest.exp = 150;
        chest.gold = 100;
        break;

    case 2:
        chest.exp = 200;
        chest.gold = 150;
        chest.items.push_back(ItemDB::getItem(ItemID::KEY21));
        break;

    case 3:
        chest.exp = 250;
        chest.gold = 200;
        break;

    case 4:
        chest.exp = 300;
        chest.gold = 250;
        break;

    case 5:
        chest.exp = 400;
        chest.gold = 300;
        chest.items.push_back(ItemDB::getItem(ItemID::KEY51));
        break;

    default:
        chest.exp = 0;
        chest.gold = 0;
        break;
    }
    return chest;
}

ChestRewardData CreateChestX2(int index)
{
    ChestRewardData chest;

    switch (index)
    {
    case 1:
        chest.exp = 150;
        chest.gold = 100;
        chest.items.push_back(ItemDB::getItem(ItemID::KEY12));
        break;

    case 2:
        chest.exp = 200;
        chest.gold = 150;
        chest.items.push_back(ItemDB::getItem(ItemID::KEY22));
        break;

    case 3:
        chest.exp = 250;
        chest.gold = 200;
        chest.items.push_back(ItemDB::getItem(ItemID::KEY32));
        break;

    case 4:
        chest.exp = 300;
        chest.gold = 250;
        chest.items.push_back(ItemDB::getItem(ItemID::KEY42));
        break;

    case 5:
        chest.exp = 400;
        chest.gold = 300;
        chest.items.push_back(ItemDB::getItem(ItemID::KEY52));
        break;

    default:
        chest.exp = 0;
        chest.gold = 0;
        break;
    }
    return chest;
}

ChestRewardData CreateChestY(int index)
{
    ChestRewardData chest;

    switch (index)
    {
    case 1:
        chest.exp = 300;
        chest.gold = 400;
        chest.items.push_back(ItemDB::getItem(ItemID::NormalHealthPotion));
        chest.items.push_back(ItemDB::getItem(ItemID::BasicMagicAttackPotion));
        chest.items.push_back(ItemDB::getItem(ItemID::IronSword));
        break;

    case 2:
        chest.exp = 350;
        chest.gold = 600;
        chest.items.push_back(ItemDB::getItem(ItemID::AdvancedHealthPotion));
        chest.items.push_back(ItemDB::getItem(ItemID::AdvancedAttackPotion));
        chest.items.push_back(ItemDB::getItem(ItemID::SteelSword));
        chest.items.push_back(ItemDB::getItem(ItemID::SteelArmour));
        break;

    case 3:
        chest.exp = 400;
        chest.gold = 800;
        chest.items.push_back(ItemDB::getItem(ItemID::AdvancedHealthPotion));
        chest.items.push_back(ItemDB::getItem(ItemID::AdvancedMagicAttackPotion));
        chest.items.push_back(ItemDB::getItem(ItemID::DiamondRing));
        chest.items.push_back(ItemDB::getItem(ItemID::DragonBoneSword));
        break;

    case 4:
        chest.exp = 500;
        chest.gold = 900;
        chest.items.push_back(ItemDB::getItem(ItemID::AdvancedHealthPotion));
        chest.items.push_back(ItemDB::getItem(ItemID::AdvancedMagicAttackPotion));
        chest.items.push_back(ItemDB::getItem(ItemID::DragonScaleArmour));
        chest.items.push_back(ItemDB::getItem(ItemID::DragonBoneNecklace));
        break;

    case 5:
        chest.exp = 600;
        chest.gold = 1000;
        chest.items.push_back(ItemDB::getItem(ItemID::SacredElixir));
        chest.items.push_back(ItemDB::getItem(ItemID::BloodragePotion));
        chest.items.push_back(ItemDB::getItem(ItemID::DragonBoneRing));
        chest.items.push_back(ItemDB::getItem(ItemID::DragonBoneWand));
        chest.items.push_back(ItemDB::getItem(ItemID::CrystalArmour));
        break;

    default:
        break;
    }
    return chest;
}
unique_ptr<Map> CreateLevel0()
{
    auto Level0 = make_unique<Map>();
    Level0->InitMap(64);

    // =============== 关卡 0 配置 ===============
    // 总经验：150     总金币：260

    // 起始房间
    vector<vector<int>> room00 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,1,0,1,0,1,0,1},
        {1,0,1,1,1,0,1,0,1},
        {1,0,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,3},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    // 在（4，4）设置一个宝箱
    vector<vector<int>> room01 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 奖励房间
    // 在（4，4）放置一个宝箱
    // 在周围放置路障（"保护屏障"）
    vector<vector<int>> room03 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,1,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,2,2,2,0,0,1},
        {1,0,0,2,2,2,0,0,3},
        {1,0,0,2,2,2,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room04 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,1,0,0,0,0,0,1,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,1,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 设置房间布局
    Level0->SetRoom(0, room00);
    Level0->SetRoom(1, room01);
    Level0->SetRoom(3, room03);
    Level0->SetRoom(4, room04);

    // 设置敌人
    Level0->SetEnemy(4, sf::Vector2i(4, 4), "火系训练师", 5, true);

    // 设置路障
    // 奖励房间(3)的3×3保护屏障
    Level0->SetBlock(3, sf::Vector2i(3, 3), "KEY01");
    Level0->SetBlock(3, sf::Vector2i(4, 3), "KEY01");
    Level0->SetBlock(3, sf::Vector2i(5, 3), "KEY01");
    Level0->SetBlock(3, sf::Vector2i(3, 4), "KEY01");
    Level0->SetBlock(3, sf::Vector2i(5, 4), "KEY01");
    Level0->SetBlock(3, sf::Vector2i(3, 5), "KEY01");
    Level0->SetBlock(3, sf::Vector2i(4, 5), "KEY01");
    Level0->SetBlock(3, sf::Vector2i(5, 5), "KEY01");

    // 设置宝箱
    // 起始宝箱 (房间1)
    ChestRewardData startChest;
    startChest.exp = 50;
    startChest.gold = 30;
    startChest.items.push_back(ItemDB::getItem(ItemID::CopperSword));
    Level0->SetChest(1, sf::Vector2i(4, 4), move(startChest));

    // 中心宝箱 (房间3)
    ChestRewardData centerChest;
    centerChest.exp = 100;
    centerChest.gold = 60;
    centerChest.items.push_back(ItemDB::getItem(ItemID::CopperArmour));
    centerChest.items.push_back(ItemDB::getItem(ItemID::BasicHealthPotion));
    Level0->SetChest(3, sf::Vector2i(4, 4), move(centerChest));

    // 生成房间连接
    Level0->GenerateConnections();
    auto issues = Level0->CheckSet();
    if (!issues.empty()) cout << "关卡 Level0 设置有问题：" << endl;
    return Level0;
}

unique_ptr<Map> CreateLevel1()
{
    auto Level1 = make_unique<Map>();
    Level1->InitMap(64);

    // =============== 关卡 1 配置 ===============
    // 总经验：820     总金币：1300

    // 起始房间
    vector<vector<int>> room10 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,0,0,0,1},
        {1,0,1,1,0,0,0,0,1},
        {1,0,1,0,2,0,0,0,3},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    // 在（4，4）设置一个宝箱
    vector<vector<int>> room11 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {3,0,0,0,2,0,0,0,3},
        {1,1,1,0,0,0,0,0,1},
        {1,0,1,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    // 在（2，2）设置一个宝箱
    vector<vector<int>> room12 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,1,0,0,1},
        {1,0,2,0,0,1,0,0,1},
        {1,0,0,0,1,1,0,0,1},
        {3,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    vector<vector<int>> room13 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,1,0,0,1},
        {1,0,0,1,1,1,1,0,3},
        {1,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,2,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room14 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room15 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    // 在（4，4）设置一个宝箱
    vector<vector<int>> room16 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,1},
        {1,0,0,0,0,0,1,0,1},
        {1,0,0,0,2,0,0,0,1},
        {1,1,1,0,0,0,0,0,1},
        {1,0,1,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    // 在（4，6）设置一个宝箱
    vector<vector<int>> room17 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,0,0,0,0,1,0,1},
        {1,0,0,0,1,1,1,2,1},
        {1,0,0,0,0,0,1,2,3},
        {1,1,0,0,0,0,0,2,1},
        {1,0,0,0,2,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // boss 房间
    // 在（4，4）放置一个 boss
    vector<vector<int>> room18 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 设置房间布局
    Level1->SetRoom(0, room10);
    Level1->SetRoom(1, room11);
    Level1->SetRoom(2, room12);
    Level1->SetRoom(3, room13);
    Level1->SetRoom(4, room14);
    Level1->SetRoom(5, room15);
    Level1->SetRoom(6, room16);
    Level1->SetRoom(7, room17);
    Level1->SetRoom(8, room18);

	// 设置路障
	Level1->SetBlock(7, sf::Vector2i(7, 3), "KEY12");
    Level1->SetBlock(7, sf::Vector2i(7, 4), "KEY12");
    Level1->SetBlock(7, sf::Vector2i(7, 5), "KEY12");

    // 设置敌人
    Level1->SetEnemy(4, sf::Vector2i(4, 4), "龙系训练师", 10, true);
    Level1->SetEnemy(5, sf::Vector2i(4, 4), "水系训练师", 10, true);
    Level1->SetEnemy(8, sf::Vector2i(4, 4), "火系首领", 20, false);

    // 设置宝箱
    // 起始宝箱 (房间0)
    ChestRewardData startChest;
    startChest.exp = 200;
    startChest.gold = 150;
    startChest.items.push_back(ItemDB::getItem(ItemID::CopperSword));
    startChest.items.push_back(ItemDB::getItem(ItemID::BasicHealthPotion));
    Level1->SetChest(0, sf::Vector2i(4, 4), move(startChest));

    // 探索宝箱1 (房间1)
    ChestRewardData exploreChest1;
    exploreChest1.exp = 150;
    exploreChest1.gold = 100;
    exploreChest1.items.push_back(ItemDB::getItem(ItemID::LeatherArmour));
    Level1->SetChest(1, sf::Vector2i(4, 4), move(exploreChest1));

    // 探索宝箱2 (房间2)
    ChestRewardData exploreChest2;
    exploreChest2.exp = 150;
    exploreChest2.gold = 100;
    exploreChest2.items.push_back(ItemDB::getItem(ItemID::IronRing));
    exploreChest2.items.push_back(ItemDB::getItem(ItemID::BasicAttackPotion));
    Level1->SetChest(2, sf::Vector2i(2, 2), move(exploreChest2));

    // 隐藏宝箱 (房间3)
    ChestRewardData hiddenChest;
    hiddenChest.exp = 150;
    hiddenChest.gold = 100;
    hiddenChest.items.push_back(ItemDB::getItem(ItemID::IronSword));
    Level1->SetChest(3, sf::Vector2i(6, 6), move(hiddenChest));

    // Boss前宝箱 (房间6)
    ChestRewardData preBossChest;
    preBossChest.exp = 200;
    preBossChest.gold = 150;
    preBossChest.items.push_back(ItemDB::getItem(ItemID::NormalHealthPotion));
    preBossChest.items.push_back(ItemDB::getItem(ItemID::BasicMagicAttackPotion));
    Level1->SetChest(6, sf::Vector2i(4, 4), move(preBossChest));

    // 备用宝箱 (房间7)
    ChestRewardData optionalChest;
    optionalChest.exp = 100;
    optionalChest.gold = 70;
    optionalChest.items.push_back(ItemDB::getItem(ItemID::CopperArmour));
    Level1->SetChest(7, sf::Vector2i(4, 6), move(optionalChest));

    // 生成房间连接
    Level1->GenerateConnections();
    auto issues = Level1->CheckSet();
    if (!issues.empty()) cout << "关卡 Level1 设置有问题：" << endl;
    return Level1;
}

unique_ptr<Map> CreateLevel2()
{
    auto Level2 = make_unique<Map>();
    Level2->InitMap(64);

    // =============== 关卡 2 配置 ===============
    // 总经验：820     总金币：1320

    // 起始房间
    // 在（4，4）放置一个宝箱
    vector<vector<int>> room20 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,1},
        {1,1,1,0,2,0,0,0,1},
        {1,0,1,0,0,1,1,1,1},
        {1,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    vector<vector<int>> room21 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,1},
        {1,0,0,1,1,1,0,0,3},
        {1,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    // 在（4，3）放置一个宝箱
    vector<vector<int>> room22 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,1,2,1,0,0,1},
        {3,0,0,0,1,0,0,0,1},
        {1,0,0,1,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    vector<vector<int>> room23 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,1},
        {1,0,1,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,3},
        {1,0,1,1,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room24 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room25 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    // 在（4，4）放置一个宝箱（这个宝箱可以奖励丰厚点）
    // 在周围生成路障
    vector<vector<int>> room26 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,2,0,1,0,1},
        {1,0,0,2,2,2,0,0,3},
        {1,0,1,0,2,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    vector<vector<int>> room27 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,0,1},
        {1,0,0,0,1,0,0,2,1},
        {3,0,0,0,2,0,0,2,3},
        {1,0,0,0,1,0,0,2,1},
        {1,0,0,1,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // boss 房间
    // 在（4，4）放置一个 boss
    vector<vector<int>> room28 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 设置房间布局
    Level2->SetRoom(0, room20);
    Level2->SetRoom(1, room21);
    Level2->SetRoom(2, room22);
    Level2->SetRoom(3, room23);
    Level2->SetRoom(4, room24);
    Level2->SetRoom(5, room25);
    Level2->SetRoom(6, room26);
    Level2->SetRoom(7, room27);
    Level2->SetRoom(8, room28);

    // 设置敌人
    Level2->SetEnemy(4, sf::Vector2i(4, 4), "草系训练师", 15, true);
    Level2->SetEnemy(5, sf::Vector2i(4, 4), "水系训练师", 15, true);
    Level2->SetEnemy(8, sf::Vector2i(4, 4), "龙系首领", 25, false);

    // 设置路障
    // 房间6的特殊路障配置
    Level2->SetBlock(6, sf::Vector2i(4, 3), "KEY21");
    Level2->SetBlock(6, sf::Vector2i(3, 4), "KEY21");
    Level2->SetBlock(6, sf::Vector2i(4, 5), "KEY21");
    Level2->SetBlock(6, sf::Vector2i(5, 4), "KEY21");
	// 房间7的特殊路障配置
	Level2->SetBlock(7, sf::Vector2i(7, 3), "KEY22");
    Level2->SetBlock(7, sf::Vector2i(7, 4), "KEY22");
    Level2->SetBlock(7, sf::Vector2i(7, 5), "KEY22");

    // 设置宝箱
    // 起始宝箱 (房间0)
    ChestRewardData startChest;
    startChest.exp = 150;
    startChest.gold = 100;
    startChest.items.push_back(ItemDB::getItem(ItemID::IronSword));
    startChest.items.push_back(ItemDB::getItem(ItemID::NormalHealthPotion));
    Level2->SetChest(0, sf::Vector2i(4, 4), move(startChest));

    // 探索宝箱2 (房间2)
    ChestRewardData exploreChest2;
    exploreChest2.exp = 150;
    exploreChest2.gold = 100;
    exploreChest2.items.push_back(ItemDB::getItem(ItemID::IronNecklace));
    exploreChest2.items.push_back(ItemDB::getItem(ItemID::NormalAttackPotion));
    Level2->SetChest(2, sf::Vector2i(4, 3), move(exploreChest2));

    // 核心宝箱 (房间6)
    ChestRewardData coreChest;
    coreChest.exp = 300;
    coreChest.gold = 300;
    coreChest.items.push_back(ItemDB::getItem(ItemID::SteelSword));
    coreChest.items.push_back(ItemDB::getItem(ItemID::SteelArmour));
    coreChest.items.push_back(ItemDB::getItem(ItemID::RubyNecklace));
    coreChest.items.push_back(ItemDB::getItem(ItemID::AdvancedHealthPotion));
    Level2->SetChest(6, sf::Vector2i(4, 4), move(coreChest));

    // Boss前宝箱 (房间7)
    ChestRewardData preBossChest;
    preBossChest.exp = 200;
    preBossChest.gold = 200;
    preBossChest.items.push_back(ItemDB::getItem(ItemID::AdvancedHealthPotion));
    preBossChest.items.push_back(ItemDB::getItem(ItemID::AdvancedAttackPotion));
    Level2->SetChest(7, sf::Vector2i(4, 4), move(preBossChest));

    // 生成房间连接
    Level2->GenerateConnections();
    auto issues = Level2->CheckSet();
    if (!issues.empty()) cout << "关卡 Level2 设置有问题：" << endl;
    return Level2;
}

unique_ptr<Map> CreateLevel3()
{
    auto Level3 = make_unique<Map>();
    Level3->InitMap(64);

    // =============== 关卡 3 配置 ===============
    // 总经验：1710    总金币：2950

    // 起始房间
    // 在（1，3）放置一个宝箱
    vector<vector<int>> room30 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,1},
        {1,2,1,0,0,0,0,0,1},
        {1,1,1,0,0,0,0,0,3},
        {1,0,1,1,1,1,0,0,1},
        {1,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    vector<vector<int>> room31 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,0,1},
        {3,0,0,1,0,1,0,0,3},
        {1,0,0,1,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    // 在（4，4）放置一个宝箱
    vector<vector<int>> room32 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,1,0,0,1},
        {1,0,0,1,1,1,0,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room33 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 迷宫房间
    // 在（1，7）设置一个宝箱
    vector<vector<int>> room34 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,0,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,0,0,1},
        {1,0,1,1,1,0,1,1,1},
        {1,2,1,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room35 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    // 在（1，2）放置一个隐藏宝箱
    vector<vector<int>> room36 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,2,1,1,0,0,1,1,1},
        {1,1,1,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,3},
        {1,0,0,0,1,0,0,0,1},
        {1,0,1,1,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room37 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,2,1},
        {3,0,0,0,2,0,0,2,3},
        {1,0,1,0,0,0,1,2,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // boss 房间
    // 在（4，4）放置一个 boss
    vector<vector<int>> room38 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 设置房间布局
    Level3->SetRoom(0, room30);
    Level3->SetRoom(1, room31);
    Level3->SetRoom(2, room32);
    Level3->SetRoom(3, room33);
    Level3->SetRoom(4, room34);
    Level3->SetRoom(5, room35);
    Level3->SetRoom(6, room36);
    Level3->SetRoom(7, room37);
    Level3->SetRoom(8, room38);

	// 设置路障
	Level3->SetBlock(7, sf::Vector2i(7, 3), "KEY32");
    Level3->SetBlock(7, sf::Vector2i(7, 4), "KEY32");
    Level3->SetBlock(7, sf::Vector2i(7, 5), "KEY32");

    // 设置敌人
    Level3->SetEnemy(3, sf::Vector2i(4, 4), "水系训练师", 20, true);
    Level3->SetEnemy(5, sf::Vector2i(4, 4), "火系训练师", 20, true);
    Level3->SetEnemy(7, sf::Vector2i(4, 4), "龙系训练师", 20, true);
    Level3->SetEnemy(8, sf::Vector2i(4, 4), "草系首领", 30, false);

    // 设置宝箱
    // 起始宝箱 (房间0)
    ChestRewardData startChest;
    startChest.exp = 180;
    startChest.gold = 180;
    startChest.items.push_back(ItemDB::getItem(ItemID::SteelSword));
    startChest.items.push_back(ItemDB::getItem(ItemID::NormalHealthPotion));
    Level3->SetChest(0, sf::Vector2i(1, 3), move(startChest));

    // 探索宝箱 (房间2)
    ChestRewardData exploreChest;
    exploreChest.exp = 230;
    exploreChest.gold = 230;
    exploreChest.items.push_back(ItemDB::getItem(ItemID::RubyNecklace));
    exploreChest.items.push_back(ItemDB::getItem(ItemID::NormalAttackPotion));
    Level3->SetChest(2, sf::Vector2i(4, 4), move(exploreChest));

    // 迷宫宝箱 (房间4)
    ChestRewardData mazeChest;
    mazeChest.exp = 250;
    mazeChest.gold = 250;
    mazeChest.items.push_back(ItemDB::getItem(ItemID::DragonBoneSword));
    mazeChest.items.push_back(ItemDB::getItem(ItemID::AdvancedHealthPotion));
    Level3->SetChest(4, sf::Vector2i(1, 7), move(mazeChest));

    // 隐藏宝箱 (房间6)
    ChestRewardData hiddenChest;
    hiddenChest.exp = 250;
    hiddenChest.gold = 250;
    hiddenChest.items.push_back(ItemDB::getItem(ItemID::DragonScaleArmour));
    hiddenChest.items.push_back(ItemDB::getItem(ItemID::AdvancedAttackPotion));
    Level3->SetChest(6, sf::Vector2i(1, 2), move(hiddenChest));

    // 生成房间连接
    Level3->GenerateConnections();
    auto issues = Level3->CheckSet();
    if (!issues.empty()) cout << "关卡 Level3 设置有问题：" << endl;
    return Level3;
}

unique_ptr<Map> CreateLevel4()
{
    auto Level4 = make_unique<Map>();
    Level4->InitMap(64);

    // =============== 关卡 4 配置 ===============
    // 总经验：1840    总金币：3050

    // 起始房间
    // 在（1，7）放置一个宝箱
    vector<vector<int>> room40 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,3},
        {1,0,1,1,1,1,0,0,1},
        {1,0,1,0,0,0,0,0,1},
        {1,2,1,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    vector<vector<int>> room41 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1},
        {3,0,0,1,0,1,0,0,3},
        {1,0,0,1,1,1,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room42 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间（资源房间）
    // 在（1，1）放置一个宝箱
    // 在（6，6）放置一个宝箱
    vector<vector<int>> room43 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,2,1,0,0,0,0,0,1},
        {1,0,1,1,0,0,1,0,1},
        {1,0,0,1,0,0,1,0,1},
        {1,0,0,1,0,0,1,0,3},
        {1,0,0,0,0,1,1,0,1},
        {1,0,1,0,0,1,2,0,1},
        {1,0,1,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 迷宫房间
    // 在（3，5）设置一个宝箱（奖励可以丰厚点）
    vector<vector<int>> room44 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,0,1,0,1},
        {1,1,1,0,0,0,1,0,1},
        {3,0,1,1,1,1,1,0,3},
        {1,0,1,2,0,0,1,0,1},
        {1,0,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    vector<vector<int>> room45 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,1,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    // 在（1，7）放置一个隐藏宝箱（奖励可以丰厚点）
    vector<vector<int>> room46 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,0,0,0,1,1,1},
        {1,0,1,0,0,1,1,0,1},
        {1,0,1,1,0,0,0,0,3},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,0,0,0,1},
        {1,2,1,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room47 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,2,1},
        {3,0,0,0,2,0,0,2,3},
        {1,0,1,0,0,0,1,2,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // boss 房间
    // 在（4，4）放置一个 boss
    vector<vector<int>> room48 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 设置房间布局
    Level4->SetRoom(0, room40);
    Level4->SetRoom(1, room41);
    Level4->SetRoom(2, room42);
    Level4->SetRoom(3, room43);
    Level4->SetRoom(4, room44);
    Level4->SetRoom(5, room45);
    Level4->SetRoom(6, room46);
    Level4->SetRoom(7, room47);
    Level4->SetRoom(8, room48);

    // 设置敌人
    Level4->SetEnemy(2, sf::Vector2i(4, 4), "电系训练师", 25, true);
    Level4->SetEnemy(7, sf::Vector2i(4, 4), "草系训练师", 25, true);
    Level4->SetEnemy(8, sf::Vector2i(4, 4), "电系首领", 35, false);

	// 设置路障
	Level4->SetBlock(7, sf::Vector2i(7, 3), "KEY42");
    Level4->SetBlock(7, sf::Vector2i(7, 4), "KEY42");
    Level4->SetBlock(7, sf::Vector2i(7, 5), "KEY42");

    // 设置宝箱
    // 起始宝箱 (房间0)
    ChestRewardData startChest;
    startChest.exp = 200;
    startChest.gold = 200;
    startChest.items.push_back(ItemDB::getItem(ItemID::DragonBoneSword));
    startChest.items.push_back(ItemDB::getItem(ItemID::AdvancedHealthPotion));
    Level4->SetChest(0, sf::Vector2i(1, 7), move(startChest));

    // 资源宝箱1 (房间3)
    ChestRewardData resourceChest1;
    resourceChest1.exp = 250;
    resourceChest1.gold = 250;
    resourceChest1.items.push_back(ItemDB::getItem(ItemID::RubyNecklace));
    resourceChest1.items.push_back(ItemDB::getItem(ItemID::AdvancedAttackPotion));
    Level4->SetChest(3, sf::Vector2i(1, 1), move(resourceChest1));

    // 资源宝箱2 (房间3)
    ChestRewardData resourceChest2;
    resourceChest2.exp = 250;
    resourceChest2.gold = 250;
    resourceChest2.items.push_back(ItemDB::getItem(ItemID::DiamondRing));
    resourceChest2.items.push_back(ItemDB::getItem(ItemID::AdvancedMagicAttackPotion));
    Level4->SetChest(3, sf::Vector2i(6, 6), move(resourceChest2));

    // 迷宫宝箱 (房间4)
    ChestRewardData mazeChest;
    mazeChest.exp = 400;
    mazeChest.gold = 400;
    mazeChest.items.push_back(ItemDB::getItem(ItemID::DiamondSword));
    mazeChest.items.push_back(ItemDB::getItem(ItemID::DiamondArmour));
    Level4->SetChest(4, sf::Vector2i(3, 5), move(mazeChest));

    // 隐藏宝箱 (房间6)
    ChestRewardData hiddenChest;
    hiddenChest.exp = 400;
    hiddenChest.gold = 400;
    hiddenChest.items.push_back(ItemDB::getItem(ItemID::DragonBoneWand));
    hiddenChest.items.push_back(ItemDB::getItem(ItemID::CrystalArmour));
    Level4->SetChest(6, sf::Vector2i(1, 7), move(hiddenChest));

    // 生成房间连接
    Level4->GenerateConnections();
    auto issues = Level4->CheckSet();
    if (!issues.empty()) cout << "关卡 Level4 设置有问题：" << endl;
    return Level4;
}

unique_ptr<Map> CreateLevel5()
{
    auto Level5 = make_unique<Map>();
    Level5->InitMap(64);

    // =============== 关卡 5 配置 ===============
    // 总经验：1540     总金币：3000

    // 起始房间
    // 在（2，6）放置一个宝箱
    vector<vector<int>> room50 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,3},
        {1,1,1,1,1,0,0,0,1},
        {1,0,2,0,0,0,1,0,1},
        {1,0,0,0,0,0,1,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room51 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,1,0,0,0,0,0,1,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,1,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room52 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,1,0,0,0,0,0,1,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,1,0,0,0,0,0,1,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    // 在（6，6）放置一个宝箱
    vector<vector<int>> room53 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,1,0,0,0,0,0,1},
        {1,0,1,1,0,0,1,0,1},
        {1,0,0,0,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,3},
        {1,0,1,1,0,0,0,0,1},
        {1,0,1,0,0,0,2,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,3,1,1,1,1}
    };

    // 普通房间
    // 在（3，5）设置一个宝箱
    vector<vector<int>> room54 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,0,1,0,0,0,0,0,1},
        {1,0,0,1,0,0,1,0,1},
        {1,0,0,0,1,1,0,0,1},
        {3,0,0,0,1,0,0,0,3},
        {1,0,1,1,1,0,0,0,1},
        {1,0,1,0,0,0,2,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 迷宫房间
    // 在（2，1）设置一个宝箱（奖励可以丰厚点）
    // 在（1，2）设置一个路障（"宝箱路障"）
    // 在（3，5）设置一个宝箱（奖励可以丰厚点）
    // 在（3，4）设置一个路障（"宝箱路障"）
    vector<vector<int>> room55 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,2,1,0,0,0,0,1},
        {1,2,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,1,2,1,0,1,0,1},
        {1,0,1,2,1,0,0,0,1},
        {1,0,1,1,1,0,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 普通房间
    vector<vector<int>> room56 =
    {
        {1,1,1,1,3,1,1,1,1},
        {1,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,0,0,1},
        {1,0,1,1,1,1,1,0,3},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 小怪 房间
    // 在（4，4）放置一个 小怪
    vector<vector<int>> room57 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,1,0,0,0,0,0,1,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,2,1},
        {3,0,0,0,2,0,0,2,3},
        {1,0,1,0,0,0,1,2,1},
        {1,0,1,1,0,1,1,0,1},
        {1,1,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 终章大 boss 房间
    // 在（4，4）放置一个大 boss
    vector<vector<int>> room58 =
    {
        {1,1,1,1,1,1,1,1,1},
        {1,1,0,0,0,0,0,1,1},
        {1,0,1,1,0,1,1,0,1},
        {1,0,1,0,0,0,1,0,1},
        {3,0,0,0,2,0,0,0,1},
        {1,0,1,0,0,0,1,0,1},
        {1,0,1,1,0,1,1,0,1},
        {1,1,0,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1}
    };

    // 设置房间布局
    Level5->SetRoom(0, room50);
    Level5->SetRoom(1, room51);
    Level5->SetRoom(2, room52);
    Level5->SetRoom(3, room53);
    Level5->SetRoom(4, room54);
    Level5->SetRoom(5, room55);
    Level5->SetRoom(6, room56);
    Level5->SetRoom(7, room57);
    Level5->SetRoom(8, room58);

    // 设置敌人
    Level5->SetEnemy(1, sf::Vector2i(4, 4), "水系训练师", 30, true);
    Level5->SetEnemy(2, sf::Vector2i(4, 4), "火系训练师", 30, true);
    Level5->SetEnemy(7, sf::Vector2i(4, 4), "龙系训练师", 30, true);
    Level5->SetEnemy(8, sf::Vector2i(4, 4), "水系首领", 40, false);

    // 设置路障
    // 迷宫房间 5 的特殊配置
    Level5->SetBlock(5, sf::Vector2i(1, 2), "KEY51");
    Level5->SetBlock(5, sf::Vector2i(3, 4), "KEY51");
	// 房间 7 的特殊配置
    Level5->SetBlock(7, sf::Vector2i(7, 3), "KEY52");
    Level5->SetBlock(7, sf::Vector2i(7, 4), "KEY52");
    Level5->SetBlock(7, sf::Vector2i(7, 5), "KEY52");

    // 设置宝箱
    // 起始宝箱 (房间0)
    ChestRewardData startChest;
    startChest.exp = 200;
    startChest.gold = 200;
    startChest.items.push_back(ItemDB::getItem(ItemID::DiamondSword));
    startChest.items.push_back(ItemDB::getItem(ItemID::AdvancedHealthPotion));
    Level5->SetChest(0, sf::Vector2i(2, 6), move(startChest));

    // 探索宝箱1 (房间3)
    ChestRewardData exploreChest1;
    exploreChest1.exp = 250;
    exploreChest1.gold = 250;
    exploreChest1.items.push_back(ItemDB::getItem(ItemID::DiamondArmour));
    Level5->SetChest(3, sf::Vector2i(6, 6), move(exploreChest1));

    // 探索宝箱2 (房间4)
    ChestRewardData exploreChest2;
    exploreChest2.exp = 250;
    exploreChest2.gold = 250;
    exploreChest2.items.push_back(ItemDB::getItem(ItemID::RubyNecklace));
    exploreChest2.items.push_back(ItemDB::getItem(ItemID::AdvancedAttackPotion));
    Level5->SetChest(4, sf::Vector2i(6, 6), move(exploreChest2));

    // 迷宫宝箱1 (房间5)
    ChestRewardData mazeChest1;
    mazeChest1.exp = 350;
    mazeChest1.gold = 350;
    mazeChest1.items.push_back(ItemDB::getItem(ItemID::DragonBoneWand));
    mazeChest1.items.push_back(ItemDB::getItem(ItemID::AdvancedMagicAttackPotion));
    Level5->SetChest(5, sf::Vector2i(2, 1), move(mazeChest1));

    // 迷宫宝箱2 (房间5)
    ChestRewardData mazeChest2;
    mazeChest2.exp = 350;
    mazeChest2.gold = 350;
    mazeChest2.items.push_back(ItemDB::getItem(ItemID::CrystalArmour));
    mazeChest2.items.push_back(ItemDB::getItem(ItemID::DiamondRing));
    Level5->SetChest(5, sf::Vector2i(3, 5), move(mazeChest2));

    // 生成房间连接
    Level5->GenerateConnections();
    auto issues = Level5->CheckSet();
    if (!issues.empty()) cout << "关卡 Level5 设置有问题：" << endl;
    return Level5;
}