#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Obj.h"
#include "Enemy.h"
#include "battlesystem.h"
#include "Player.h"
using namespace std;

// 枚举类型
enum class SqureType
{
    Groud = 0,    // 地面
    Wall = 1,     // 墙壁
    Object = 2,   // 物体
    Transmit = 3  // 传送点
};

class Room
{
private:
    // 房间属性
    sf::Vector2i RoomSize;        // 尺寸，默认9 * 9
    int SqureSize;                // 格子大小
    sf::Vector2f WorldPosition;   // 世界中的位置

    // 纹理
    sf::Texture textureGround;    // 地面纹理
    sf::Texture transmitTexture;  // 传送点纹理

    // 房间数据
    vector<SqureType> squres;              // 格子类型
    vector<bool> CollisonSqure;            // 碰撞格子
    vector<unique_ptr<Obj>> objects;       // 物体列表

    // 用于反序列化
    unique_ptr<Obj> CreateObjByType(ObjType type);

public:
    // 构造函数
    Room(int RoomLen = 9, int RoomWidth = 9, int squreSize = 64);

    // ==================== 地图接口 ====================

    // 坐标转换
    sf::Vector2f SqureToWorld(int RoomX, int RoomY) const;
    sf::Vector2i WorldToSqure(const sf::Vector2f& WorldPos) const;

    // 碰撞检测
    bool IsSqureWalkable(int RoomX, int RoomY) const;
    bool IsPosWalkable(const sf::Vector2f& WorldPos) const;
    bool CheckCollision(const sf::FloatRect& bounds) const;

    // 获取边界
    sf::FloatRect GetBounds() const;

    // 设置格子类型
    void SetWall(int RoomX, int RoomY);
    void SetGround(int RoomX, int RoomY);
    void SetObject(int RoomX, int RoomY);
    void SetTransmit(int RoomX, int RoomY);

    // 获取格子类型
    SqureType GetSqureType(int RoomX, int RoomY) const;
    bool IsTransmitPoint(int RoomX, int RoomY) const;
    bool IsObjectSlot(int RoomX, int RoomY) const;

    // 添加物体
    void AddObject(unique_ptr<Obj> obj);
    void AddChest(int RoomX, int RoomY, const ChestRewardData& reward);
    void AddBlock(int RoomX, int RoomY, const string condition = "");

    // 更新物体和绘制
    void UpdateObjects(float deltaTime);
    void Draw(sf::RenderWindow& window) const;

	// Getter 方法和 Setter 方法
    sf::Vector2i GetRoomSize() const;
    int GetSqureSize() const;
    sf::Vector2f GetWorldPosition() const;
    float GetWidth() const;
    float GetHeight() const;
    void SetWorldPosition(const sf::Vector2f& pos);

    // 通关障碍物移除
    bool RemoveBlocksByKey(const string& condition);
    // 搜索宝箱
    ChestRewardData SearchChest(const sf::Vector2f& centerPos);
    // 新增补丁
    bool IsEmptyObjectSlot(int RoomX, int RoomY) const;
    // 去除宝箱
    void ClearAllChests();

    // ==================== 新增敌人接口 ====================

    // 设置地图上的敌人
    void AddEnemy(const string& enemyName, int level, const sf::Vector2i& gridPos, bool canRespawn = true);
    // 搜索附近敌人
    Enemy* SearchEnemy(const sf::Vector2f& playerCenterPos) const;
    // 开始与指定敌人的战斗
    unique_ptr<BattleSystem> StartBattleWithEnemy(Enemy* enemy, Player* player);
    // 移除一个敌人，并生成宝箱
    unique_ptr<monbase::Codemon> RemoveEnemy(Enemy* enemy, const ChestRewardData& reward);
    // 根据位置查找敌人
    Enemy* GetEnemyAtPosition(const sf::Vector2i& gridPos) const;
    // 重置房间内所有敌人状态
    void ResetAllEnemies();

    // ==================== 地图完成接口 ====================
    
    // 是否所有宝箱被打开
    bool isAllChestsOpened() const;
    // 房间敌人是否被击败
    bool isBossDefeated() const;

    // ================ 序列化和反序列化接口 ================

	// 序列化房间数据
    void Serialize(ostream& os) const;
	// 反序列化房间数据
    void Deserialize(istream& is);
};