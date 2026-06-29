#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "Item.h" 

class ItemDB
{
public:
    //声明静态成员
    static std::map<ItemID, std::unique_ptr<Item>> database;
    static std::unordered_map<ItemID, std::string> texturePaths;

    //初始化函数
    static void initialize();

    //获取道具克隆
    static std::unique_ptr<Item> getItem(ItemID id);

    //获取所有道具
    static const auto& getAllItems() { return database; }

    //纹理管理
    static void registerTexturePath(ItemID id, const std::string& path);
    static std::string getTexturePath(ItemID id);
};

// 辅助结构体
struct ItemTemplate {
    ItemID id;
    std::string name;
    std::string desc;
    int price;
    ItemType type;
    EquipSlot slot;
    std::vector<std::pair<monbase::MoveEffect, int>> effects;
};