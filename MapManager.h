#pragma once
#include "Room.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
using namespace std;

class Map
{
private:
    // 固定9个房间 (3x3网格)
    static const int squre_rows = 3;
    static const int squre_cols = 3;
    static const int max_rooms = 9;
    // 房间数组，智能指针管理
    unique_ptr<Room> rooms[max_rooms];
    // 房间连接关系数组
    vector<vector<int>> roomConnections;
    // 当前玩家当前所在的房间索引 (0-8)
    int currentRoomIndex;

public:
    Map();
    ~Map() = default;
    // ==========初始化与设置接口==========
    // 初始化整个地图（重置所有房间）
    void InitMap(int squreSize = 64);
    // 设置房间的详细布局
    // roomIndex - 房间索引(0-8)
    // layout - 9x9的二维vector，元素含义:
    // 0: 地面 (SqureType::Groud)
    // 1: 墙壁 (SqureType::Wall)
    // 2: 物品位置预留 (SqureType::Object)
    // 3: 传送点 (SqureType::Transmit)
    // 此函数会覆盖房间原有布局，包括已设置的物品
    void SetRoom(int roomIndex, const vector<vector<int>>& layout);

    // ==========物品设置接口==========
    // 在指定房间的指定位置设置宝箱
    // 位置必须是在SetRoom中用2标记过的
    // 如果该位置已有物品，会被新物品覆盖
    bool SetChest(int roomIndex, const sf::Vector2i& position,
        ChestRewardData&& reward);
    // 在指定房间的指定位置设置路障
    // 位置必须是在SetRoom中用2标记过的
    // 如果该位置已有物品，会被新物品覆盖
    bool SetBlock(int roomIndex, const sf::Vector2i& position,
        const string& keyName);
    // 在指定房间的指定位置设置敌人
    // 位置必须是在SetRoom中用2标记过的
    // 如果该位置已有物品，会被新物品覆盖
    bool SetEnemy(int roomIndex, const sf::Vector2i& position,
        const string& enemyName, int level, bool canRespawn = true);
    // 检查所有标记为2的位置是否都已设置具体物品
    // 返回值: 未设置物品的位置列表，每个元素是{房间索引, 格子坐标}
    vector<pair<int, sf::Vector2i>> CheckSet() const;

    // ==========传送和房间切换==========
    // 传送主函数 (由Player在触碰传送点的时候调用)
    // 返回值: 如果传送成功，返回传送到的世界坐标
    // 如果失败，返回原坐标 playerPos
    sf::Vector2f TransmitToRoom(const sf::Vector2f& playerPos);
    // 直接切换当前房间 (用于调试或特殊事件)
    void SetCurrentRoom(int roomIndex);
    // 打印切换信息（显示触发了传送事件）
    void RoomChanged(int fromRoom, int toRoom, const sf::Vector2f& newPlayerPos);

    // ==========查询和获取函数==========
    // 获取当前房间指针
    Room* GetCurrentRoom() const;
    // 获取指定房间指针
    Room* GetRoom(int roomIndex) const;
    // 获取当前房间索引
    int GetCurrentRoomIndex() const;
    // 检查两个房间是否直接相连
    bool AreRoomsConnected(int roomA, int roomB) const;
    // 获取整个地图的边界矩形 (世界坐标)
    sf::FloatRect GetMapBounds() const;
    // 获取房间的世界坐标位置 (用于相机系统)
    sf::Vector2f RoomToWorld(int roomIndex) const;
    // 判断指定房间是否完成
    bool isRoomCompleted(int roomIndex) const;
    // 生成邻接表
    void GenerateConnections();

    // ==========游戏循环接口==========
    // 更新地图状态
    void Update(float deltaTime);
    // 绘制当前房间
    void Draw(sf::RenderWindow& window) const;

    // ==========序列化和反序列化==========
    // 序列化
    void Serialize(ostream& os) const;
    // 反序列化
    void Deserialize(istream& is);

private:
    // ==========辅助私有函数==========
    // 检查位置是否可以设置物品 (必须是之前用2标记过的位置)
    bool CanSetObject(int roomIndex, const sf::Vector2i& position) const;
    // 清除指定位置的现有物品
    void ClearObject(int roomIndex, const sf::Vector2i& position);
    // 获取相邻房间的索引
    int GetNorthRoom(int roomIndex) const;
    int GetSouthRoom(int roomIndex) const;
    int GetEastRoom(int roomIndex) const;
    int GetWestRoom(int roomIndex) const;
};