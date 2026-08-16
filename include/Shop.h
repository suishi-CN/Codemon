#pragma once
#include "MapManager.h"
#include "Map.h"
#include "ItemDB.h"
#include <memory>
#include <iostream>
using namespace std;

// 关卡创建函数声明：
unique_ptr<Map> CreateLevel0();
unique_ptr<Map> CreateLevel1();
unique_ptr<Map> CreateLevel2();
unique_ptr<Map> CreateLevel3();
unique_ptr<Map> CreateLevel4();
unique_ptr<Map> CreateLevel5();

// 宝箱创建函数声明
ChestRewardData CreateChestX1(int index);
ChestRewardData CreateChestX2(int index);
ChestRewardData CreateChestY(int index);