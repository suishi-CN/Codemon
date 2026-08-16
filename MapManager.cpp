#include "../include/Map.h"
#include <iostream>
#include <algorithm>
using namespace std;

// 构造函数
Map::Map(): currentRoomIndex(0)
{
    roomConnections.resize(max_rooms, vector<int>
        (max_rooms, 0));
    for (int i = 0; i < max_rooms; i++)
        rooms[i] = nullptr;
}

// 初始化设置
void Map::InitMap(int squreSize)
{
    // 清空现有房间
    for (int i = 0; i < max_rooms; i++) 
    {
        rooms[i] = nullptr;
    }
    // 重置连接
    roomConnections.clear();
    roomConnections.resize(max_rooms, vector<int>(max_rooms, 0));
    // 从房间0开始
    currentRoomIndex = 0;
}

// 房间布局设置
void Map::SetRoom(int roomIndex, const vector<vector<int>>& layout)
{
    if (roomIndex > 8 || roomIndex < 0)
    {
        cout << "房间索引无效！" << endl;
        return;
    }
    if (layout.size() != 9) 
    {
        cout << "布局行数应为9！ " << endl;
        return;
    }
    for (int i = 0; i < 9; i++) 
    {
        if (layout[i].size() != 9) 
        {
            cout << "行列数应为9！"<< endl;
            return;
        }
    }
    // 如果房间不存在，先初始化
    if (!rooms[roomIndex]) 
    {
        rooms[roomIndex] = make_unique<Room>(9, 9, 64);
    }
    // 开始布局应用
    Room* room = rooms[roomIndex].get();
    for (int y = 0; y < 9; y++) 
    {
        for (int x = 0; x < 9; x++) 
        {
            int squreNum = layout[y][x];
            switch (squreNum) 
            {
            case 0: // 地面
                room->SetGround(x, y);
                break;
            case 1: // 墙壁
                room->SetWall(x, y);
                break;
            case 2: // 物品预留位置
                room->SetObject(x, y);
                break;
            case 3: // 传送点
                room->SetTransmit(x, y);
                break;
            default:
                room->SetGround(x, y);
                break;
            }
        }
    }
}

// 连接管理
bool IsValidRoomIndex(int roomIndex)
{
    return !(roomIndex > 8 || roomIndex < 0);
}

bool Map::AreRoomsConnected(int roomA, int roomB) const
{
    if (!IsValidRoomIndex(roomA) || !IsValidRoomIndex(roomB))
        return false;

    const vector<int>& connections = roomConnections[roomA];
    return find(connections.begin(), connections.end(), roomB) != connections.end();
}

void Map::GenerateConnections()
{
    // 先初始化邻接表
    for (int i = 0; i < max_rooms; i++)
    {
        for (int j = 0; j < max_rooms; j++)
        {
            roomConnections[i][j] = 0;
        }
    }
    // 遍历所有房间
    for (int roomIndex = 0; roomIndex < max_rooms; roomIndex++)
    {
        if (!rooms[roomIndex]) continue;
        Room* room = rooms[roomIndex].get();
        // 检查四个边界中间是否有传送点
        static const vector<sf::Vector2i> boundaryPositions = 
        {
            sf::Vector2i(4, 0),  // 上边界
            sf::Vector2i(4, 8),  // 下边界
            sf::Vector2i(0, 4),  // 左边界
            sf::Vector2i(8, 4)   // 右边界
        };
        for (int i = 0; i < boundaryPositions.size(); i++)
        {
            sf::Vector2i pos = boundaryPositions[i];
            if (room->IsTransmitPoint(pos.x, pos.y))
            {
                // 有传送点，找到相邻房间
                int neighborRoom = -1;
                switch (i)
                {
                case 0: // 上边界
                    neighborRoom = GetNorthRoom(roomIndex);
                    break;
                case 1: // 下边界
                    neighborRoom = GetSouthRoom(roomIndex);
                    break;
                case 2: // 左边界
                    neighborRoom = GetWestRoom(roomIndex);
                    break;
                case 3: // 右边界
                    neighborRoom = GetEastRoom(roomIndex);
                    break;
                }
                // 如果相邻房间存在且有对应的传送点，则建立连接
                int index = (i + 2) % 4;
                if (neighborRoom >= 0 && neighborRoom < max_rooms &&
                    rooms[neighborRoom] &&
                    rooms[neighborRoom]->IsTransmitPoint(boundaryPositions[index].x, boundaryPositions[index].y))
                {
                    // 建立双向连接
                    roomConnections[roomIndex][neighborRoom] = 1;
                    roomConnections[neighborRoom][roomIndex] = 1;
                }
            }
        }
    }
}

// 物品设置
bool Map::SetChest(int roomIndex, const sf::Vector2i& position,
    ChestRewardData&& reward)
{
    if (!IsValidRoomIndex(roomIndex)) 
    {
        cout << "房间索引无效！" << endl;
        return false;
    }
    if (!rooms[roomIndex]) 
    {
        cout << "房间未初始化！" << endl;
        return false;
    }
    if (!CanSetObject(roomIndex, position)) 
    {
        cerr << "位置 (" << position.x << "," << position.y << ") 不是物品预留位置！" << endl;
        return false;
    }
    // 清除该位置可能已有的物品
    ClearObject(roomIndex, position);
    // 添加宝箱
    rooms[roomIndex]->AddChest(position.x, position.y, reward);
    return true;
}

bool Map::SetBlock(int roomIndex, const sf::Vector2i& position,
    const string& keyName)
{
    if (!IsValidRoomIndex(roomIndex)) 
    {
        cout << "房间索引无效！" << endl;
        return false;
    }
    if (!rooms[roomIndex]) 
    {
        cout << "房间未初始化！" << endl;
        return false;
    }
    if (!CanSetObject(roomIndex, position)) 
    {
        cerr << "位置 (" << position.x << "," << position.y << ") 不是物品预留位置！" << endl;
        return false;
    }
    // 清除该位置可能已有的物品
    ClearObject(roomIndex, position);
    // 添加路障
    rooms[roomIndex]->AddBlock(position.x, position.y, keyName);
    return true;
}

bool Map::SetEnemy(int roomIndex, const sf::Vector2i& position,
    const string& enemyName, int level, bool canRespawn)
{
    if (!IsValidRoomIndex(roomIndex))
    {
        cout << "房间索引无效！" << endl;
        return false;
    }
    if (!rooms[roomIndex])
    {
        cout << "房间未初始化！" << endl;
        return false;
    }
    if (!CanSetObject(roomIndex, position))
    {
        cerr << "位置 (" << position.x << "," << position.y << ") 不是物品预留位置！" << endl;
        return false;
    }
    // 清除该位置可能已有的物品
    ClearObject(roomIndex, position);
    // 添加敌人
    sf::Vector2i pos(position.x, position.y);
    rooms[roomIndex]->AddEnemy(enemyName, level, pos, canRespawn);
    return true;
}

vector<pair<int, sf::Vector2i>> Map::CheckSet() const
{
    vector<pair<int, sf::Vector2i>> issues;
    // 允许的传送点位置
    vector<sf::Vector2i> validTransmitPositions = 
    {
        sf::Vector2i(4, 0),  // 上边界中间
        sf::Vector2i(4, 8),  // 下边界中间
        sf::Vector2i(0, 4),  // 左边界中间
        sf::Vector2i(8, 4)   // 右边界中间
    };
    for (int roomIndex = 0; roomIndex < max_rooms; roomIndex++)
    {
        if (!rooms[roomIndex]) { continue; }
        Room* room = rooms[roomIndex].get();
        // 检查9x9网格
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                SqureType type = room->GetSqureType(x, y);
                // 检查标记为2的位置是否有物品
                if (type == SqureType::Object)
                {
                    // 检查是否有实际物品
                    sf::Vector2f worldPos = room->SqureToWorld(x, y);
                    if (room->IsEmptyObjectSlot(x, y))
                    {
                        // 空的物品位，有问题
                        issues.emplace_back(roomIndex, sf::Vector2i(x, y));
                    }
                }
                // 检查传送点位置是否在允许的位置
                else if (type == SqureType::Transmit)
                {
                    sf::Vector2i pos(x, y);
                    bool isValidTransmitPos = false;
                    // 检查是否是允许的位置
                    for (const auto& validPos : validTransmitPositions)
                    {
                        if (pos == validPos)
                        {
                            isValidTransmitPos = true;
                            break;
                        }
                    }
                    if (!isValidTransmitPos)
                    {
                        issues.emplace_back(roomIndex, pos);
                    }
                }
            }
        }
    }
    return issues;
}

// 传送功能
sf::Vector2f Map::TransmitToRoom(const sf::Vector2f& playerPos)
{
    // 先记录原位置，如果条件不满足就返回这个
    sf::Vector2f originalPos = playerPos;
    // 基本安全检查
    if (currentRoomIndex < 0 || currentRoomIndex >= max_rooms)
        return originalPos;
    if (!rooms[currentRoomIndex])
        return originalPos;
    // 检查玩家是否在传送点上
    sf::Vector2i gridPos = rooms[currentRoomIndex]->WorldToSqure(playerPos);
    if (!rooms[currentRoomIndex]->IsTransmitPoint(gridPos.x, gridPos.y))
        return originalPos;

    // 确定玩家的传送方向，计算目标房间
    int targetRoomIndex = -1;
    sf::Vector2i targetGridPos = gridPos;
    // 根据传送点位置判断传送方向
    if (gridPos.x == 4 && gridPos.y == 0)  // 上边界传送点
    {
        targetRoomIndex = GetNorthRoom(currentRoomIndex);
        targetGridPos = sf::Vector2i(4, 8);  // 目标房间的下边界中间
    }
    else if (gridPos.x == 4 && gridPos.y == 8)  // 下边界传送点
    {
        targetRoomIndex = GetSouthRoom(currentRoomIndex);
        targetGridPos = sf::Vector2i(4, 0);  // 目标房间的上边界中间
    }
    else if (gridPos.x == 0 && gridPos.y == 4)  // 左边界传送点
    {
        targetRoomIndex = GetWestRoom(currentRoomIndex);
        targetGridPos = sf::Vector2i(8, 4);  // 目标房间的右边界中间
    }
    else if (gridPos.x == 8 && gridPos.y == 4)  // 右边界传送点
    {
        targetRoomIndex = GetEastRoom(currentRoomIndex);
        targetGridPos = sf::Vector2i(0, 4);  // 目标房间的左边界中间
    }
    else
    {
        // 传送点位置不符合规定
        cout << "传送失败！" << endl;
        return originalPos;
    }

    // 4. 检查目标房间是否有效
    if (targetRoomIndex < 0 || targetRoomIndex >= max_rooms)
        return originalPos;
    if (!rooms[targetRoomIndex])
        return originalPos;
    // 检查目标位置是否有传送点
    if (!rooms[targetRoomIndex]->IsTransmitPoint(targetGridPos.x, targetGridPos.y))
        return originalPos;

    // 计算新位置坐标
    sf::Vector2f newPos = rooms[targetRoomIndex]->SqureToWorld(targetGridPos.x, targetGridPos.y);
    int Size = rooms[targetRoomIndex]->GetSqureSize();
    newPos.x += Size / 2.0f;  // 调整到格子中心
    newPos.y += Size / 2.0f;
    // 切换当前房间索引
    int oldRoomIndex = currentRoomIndex;
    currentRoomIndex = targetRoomIndex;
    // 触发传送事件，打印信息
    RoomChanged(oldRoomIndex, targetRoomIndex, newPos);
    return newPos;
}

void Map::SetCurrentRoom(int roomIndex)
{
    if (!IsValidRoomIndex(roomIndex)) 
    {
        cout << "无法切换到无效房间 " << roomIndex << endl;
        return;
    }
    if (!rooms[roomIndex]) 
    {
        cout << "房间 " << roomIndex << " 未初始化" << endl;
        return;
    }
    currentRoomIndex = roomIndex;
}

void Map::RoomChanged(int fromRoom, int toRoom, const sf::Vector2f& newPlayerPos)
{
    cout << "传送成功！" << endl;
    cout << "房间切换: " << fromRoom << " -> " << toRoom << endl;
}

// 查询与获取
Room* Map::GetCurrentRoom() const
{
    if (currentRoomIndex < 0 || currentRoomIndex >= max_rooms)
        return nullptr;
    return rooms[currentRoomIndex].get();
}
Room* Map::GetRoom(int roomIndex) const
{
    if (!IsValidRoomIndex(roomIndex))
        return nullptr;
    return rooms[roomIndex].get();
}
int Map::GetCurrentRoomIndex() const
{
    return currentRoomIndex;
}
sf::FloatRect Map::GetMapBounds() const
{
    int Size = rooms[0]->GetSqureSize();
    float roomWidth = 9 * Size;
    float roomHeight = 9 * Size;
    return sf::FloatRect(0, 0, roomWidth * 3, roomHeight * 3);
}
sf::Vector2f Map::RoomToWorld(int roomIndex) const
{
    if (!IsValidRoomIndex(roomIndex))
        return sf::Vector2f(0, 0);

    // 计算房间在3x3网格中的行列位置
    int row = roomIndex / 3;
    int col = roomIndex % 3;
    // 默认房间大小为9x9，每个瓦片64像素
    float roomWidth = 9 * 64.0f;
    float roomHeight = 9 * 64.0f;
    // 计算房间的世界坐标（左上角位置）
    float worldX = col * roomWidth;
    float worldY = row * roomHeight;
    return sf::Vector2f(worldX, worldY);
}

// 游戏循环接口
void Map::Update(float deltaTime)
{
    if (rooms[currentRoomIndex]) 
    {
        rooms[currentRoomIndex]->UpdateObjects(deltaTime);
    }
}
void Map::Draw(sf::RenderWindow& window) const
{
    if (rooms[currentRoomIndex]) 
    {
        rooms[currentRoomIndex]->Draw(window);
    }
}

// 私有辅助函数
bool Map::CanSetObject(int roomIndex, const sf::Vector2i& position) const
{
    // 检查索引和初始化
    if (!IsValidRoomIndex(roomIndex) || !rooms[roomIndex])
        return false;
    // 检查位置是否合理
    if (position.x < 0 || position.x >= 9 || position.y < 0 || position.y >= 9)
        return false;
    // 检查是否应该放置物品
    return rooms[roomIndex]->IsObjectSlot(position.x, position.y);
}

void Map::ClearObject(int roomIndex, const sf::Vector2i& position)
{
    // 检查索引和初始化
    if (!IsValidRoomIndex(roomIndex) || !rooms[roomIndex])
        return;
    // 将物品位置重置为对象类型
    rooms[roomIndex]->SetObject(position.x, position.y);
}

int Map::GetNorthRoom(int roomIndex) const
{
    return roomIndex - 3;
}
int Map::GetSouthRoom(int roomIndex) const
{
    return roomIndex + 3;
}
int Map::GetEastRoom(int roomIndex) const
{
    return roomIndex + 1;
}
int Map::GetWestRoom(int roomIndex) const
{
    return roomIndex - 1;
}

// 指定房间是否完成
bool Map::isRoomCompleted(int roomIndex) const
{
    if (!IsValidRoomIndex(roomIndex))
        return false;

    Room* room = rooms[roomIndex].get();
    if (!room)
        return false;

    if (roomIndex == 3)
        return room->isAllChestsOpened();

    if (roomIndex == 8)
        return room->isBossDefeated();

    return false;
}

// 序列化和反序列化
void Map::Serialize(ostream& os) const
{
    // Map
    os << currentRoomIndex << " ";
    // 房间数量
    os << max_rooms << " ";
    // 每个 Room
    for (int i = 0; i < max_rooms; i++)
    {
        if (rooms[i])
        {
            os << 1 << " ";      // 存在标记
            rooms[i]->Serialize(os);
        }
        else os << 0 << " ";     // 空房间
    }
}

void Map::Deserialize(istream& is)
{
    // Map 
    is >> currentRoomIndex;
    int roomCount;
    is >> roomCount;
    // 安全校验
    if (roomCount > max_rooms)
        roomCount = max_rooms;
    // 先清空旧房间
    for (int i = 0; i < max_rooms; i++)
    {
        rooms[i].reset();
    }
    // 逐个读取 Room
    for (int i = 0; i < roomCount; i++)
    {
        int hasRoom;
        is >> hasRoom;
        if (hasRoom)
        {
            rooms[i] = make_unique<Room>();
            rooms[i]->Deserialize(is);
        }
    }
}