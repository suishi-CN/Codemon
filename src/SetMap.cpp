#include "../include/Room.h"
#include "../include/Obj.h"
#include <iostream>
#include <algorithm>
using namespace std;

// 构造函数
Room::Room(int RoomLen, int RoomWidth, int squreSize)
    : RoomSize(RoomLen, RoomWidth), SqureSize(squreSize), WorldPosition(0.f, 0.f)
{
    int totalSqures = RoomLen * RoomWidth;
    squres.resize(totalSqures, SqureType::Groud);  // 全部初始化为地面
    CollisonSqure.resize(totalSqures, false);      // 全部初始化为可通行
    objects.clear();                               // 清空物体列表

    // 加载地面纹理
    if (!textureGround.loadFromFile("Resources/Squre.png"))
    {
        cerr << "加载地面纹理失败！" << endl;
    }
    // 加载传送点纹理
    if (!transmitTexture.loadFromFile("Resources/Transmit.png"))
    {
        cerr << "加载传送点纹理失败！" << endl;
    }
}

// 坐标转换：格子坐标转世界坐标
sf::Vector2f Room::SqureToWorld(int RoomX, int RoomY) const
{
    float WorldX = RoomX * SqureSize + WorldPosition.x;
    float WorldY = RoomY * SqureSize + WorldPosition.y;
    return sf::Vector2f(WorldX, WorldY);
}

// 坐标转换：世界坐标转格子坐标
sf::Vector2i Room::WorldToSqure(const sf::Vector2f& WorldPos) const
{
    int RoomX = static_cast<int>((WorldPos.x - WorldPosition.x) / SqureSize);
    int RoomY = static_cast<int>((WorldPos.y - WorldPosition.y) / SqureSize);
    return sf::Vector2i(RoomX, RoomY);
}

// 碰撞检测：指定格子是否可通行
bool Room::IsSqureWalkable(int RoomX, int RoomY) const
{
    int Size = RoomSize.y;
    if (RoomX < 0 || RoomX >= Size || RoomY < 0 || RoomY >= Size)
    {
        return false;
    }
    return !CollisonSqure[RoomY * Size + RoomX];
}

// 碰撞检测：指定位置是否可通行
bool Room::IsPosWalkable(const sf::Vector2f& WorldPos) const
{
    sf::Vector2i SqurePos = WorldToSqure(WorldPos);
    return IsSqureWalkable(SqurePos.x, SqurePos.y);
}

// 碰撞检测：矩形边界碰撞
bool Room::CheckCollision(const sf::FloatRect& bounds) const
{
    // 获取矩形边界覆盖的格子范围
    sf::Vector2i minGrid = WorldToSqure(sf::Vector2f(bounds.left, bounds.top));
    sf::Vector2i maxGrid = WorldToSqure(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height));

    // 遍历所有格子
    for (int y = minGrid.y; y <= maxGrid.y; y++)
    {
        for (int x = minGrid.x; x <= maxGrid.x; x++)
        {
            if (!IsSqureWalkable(x, y))
            {
                return true;  // 发生碰撞
            }
        }
    }
    return false;  // 未发生碰撞
}

// 获取房间边界
sf::FloatRect Room::GetBounds() const
{
    return sf::FloatRect(
        WorldPosition.x,
        WorldPosition.y,
        static_cast<float>(RoomSize.x * SqureSize),
        static_cast<float>(RoomSize.y * SqureSize));
}

// 设置墙
void Room::SetWall(int RoomX, int RoomY)
{
    int Size = RoomSize.y;
    if (RoomX >= 0 && RoomX < RoomSize.x && RoomY >= 0 && RoomY < RoomSize.y)
    {
        squres[RoomY * Size + RoomX] = SqureType::Wall;
        CollisonSqure[RoomY * Size + RoomX] = true;
    }
}

// 设置地面
void Room::SetGround(int RoomX, int RoomY)
{
    int Size = RoomSize.y;
    if (RoomX >= 0 && RoomX < RoomSize.x && RoomY >= 0 && RoomY < RoomSize.y)
    {
        squres[RoomY * Size + RoomX] = SqureType::Groud;
        CollisonSqure[RoomY * Size + RoomX] = false;
    }
}

// 设置物体
void Room::SetObject(int RoomX, int RoomY)
{
    int Size = RoomSize.y;
    if (RoomX >= 0 && RoomX < RoomSize.x && RoomY >= 0 && RoomY < RoomSize.y)
    {
        squres[RoomY * Size + RoomX] = SqureType::Object;
        CollisonSqure[RoomY * Size + RoomX] = true;
    }
}

// 设置传送点
void Room::SetTransmit(int RoomX, int RoomY)
{
    int Size = RoomSize.y;
    if (RoomX >= 0 && RoomX < RoomSize.x && RoomY >= 0 && RoomY < RoomSize.y)
    {
        squres[RoomY * Size + RoomX] = SqureType::Transmit;
        CollisonSqure[RoomY * Size + RoomX] = false;
    }
}

// 获取指定格子类型
SqureType Room::GetSqureType(int RoomX, int RoomY) const
{
    int Size = RoomSize.y;
    if (RoomX >= 0 && RoomX < RoomSize.x && RoomY >= 0 && RoomY < RoomSize.y)
    {
        return squres[RoomY * Size + RoomX];
    }
    return SqureType::Wall;  // 越界返回墙
}

// 是否是传送点
bool Room::IsTransmitPoint(int RoomX, int RoomY) const
{
    return GetSqureType(RoomX, RoomY) == SqureType::Transmit;
}

// 是否是物体位
bool Room::IsObjectSlot(int RoomX, int RoomY) const
{
    return GetSqureType(RoomX, RoomY) == SqureType::Object;
}

// 添加物体到房间
void Room::AddObject(unique_ptr<Obj> obj)
{
    if (obj)
    {
        // 获取位置
        sf::Vector2i squrePos = obj->GetSqurePos();
        int Size = RoomSize.y;

        // 更新碰撞
        if (squrePos.x >= 0 && squrePos.x < RoomSize.x &&
            squrePos.y >= 0 && squrePos.y < RoomSize.y)
        {
            CollisonSqure[squrePos.y * Size + squrePos.x] = !obj->GetIsWalkable();
            squres[squrePos.y * Size + squrePos.x] = SqureType::Object;
        }

        // 添加到列表
        objects.push_back(move(obj));
    }
}

void Room::AddChest(int RoomX, int RoomY, const ChestRewardData& reward)
{

    vector<unique_ptr<Item>> chestItems;
    // 遍历奖励数据中的物品，克隆它们
    for (const auto& item : reward.items)
    {
        if (item)
            chestItems.push_back(item->clone());
    }
    auto chest = make_unique<Chest>(reward.exp, reward.gold, move(chestItems), "Chest");
    chest->SetPosition(sf::Vector2i(RoomX, RoomY), SqureSize);
    AddObject(move(chest));
}

// 添加障碍物
void Room::AddBlock(int RoomX, int RoomY, const string condition)
{
    // 创建障碍物对象
    auto block = make_unique<Block>(condition, "Block");
    block->SetPosition(sf::Vector2i(RoomX, RoomY), SqureSize);
    AddObject(move(block));
}

// 更新所有物体
void Room::UpdateObjects(float deltaTime)
{
    bool shouldClearChests = false;
    for (auto& obj: objects)
    {
        obj->Update(deltaTime);
		// 检查是否有敌人正在重生
        if (obj->GetType() == ObjType::Enemy)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
            if (enemy && enemy->IsRespawning())
                shouldClearChests = true;
        }
    }
    if (shouldClearChests)
        ClearAllChests();
}

// 绘制房间
void Room::Draw(sf::RenderWindow& window) const
{
	// 添加绘制墙壁的纹理加载逻辑
    static sf::Texture textureWall;
    static bool textureLoaded = false;

    if (!textureLoaded)
    {
        if (!textureWall.loadFromFile("Resources/Wall.png"))
        {
            cerr << "加载墙壁纹理失败！" << endl;
        }
        textureLoaded = true;
    }

    // 循环所有网格
    for (int y = 0; y < RoomSize.y; y++)
    {
        for (int x = 0; x < RoomSize.x; x++)
        {
            int index = y * RoomSize.y + x;
            sf::Vector2f worldPos = SqureToWorld(x, y);
            sf::Sprite sprite;

            // 根据网格类型绘制不同的纹理
            switch (squres[index])
            {
            case SqureType::Groud:
                // 绘制地板
                sprite.setTexture(textureGround);
                break;

            case SqureType::Wall:
                // 绘制墙壁
                sprite.setTexture(textureWall);
                break;

            case SqureType::Transmit:
                // 绘制传送点
                sprite.setTexture(transmitTexture);
                break;

            case SqureType::Object:
                // 先绘制地板
                sprite.setTexture(textureGround);
                break;

            default:
                continue;  // 跳过未知类型
            }

            // 设置缩放和位置
            const sf::Texture* tex = sprite.getTexture();
            if (tex)
            {
                float scaleX = static_cast<float>(SqureSize) / tex->getSize().x;
                float scaleY = static_cast<float>(SqureSize) / tex->getSize().y;
                sprite.setScale(scaleX, scaleY);
                sprite.setPosition(worldPos);
                window.draw(sprite);
            }
        }
    }

    // 绘制所有物体，但需要特殊处理敌人
    for (const auto& obj: objects)
    {
        if (obj->GetType() == ObjType::Enemy)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
            if (enemy)
            {
                // 检查敌人的状态
                bool isDefeated = enemy->IsDefeated();
                bool isRespawning = enemy->IsRespawning();
                // 特殊情况下绘制
                if (isDefeated && !isRespawning) { }
                else obj->Draw(window);
            }
        }
        else obj->Draw(window);
    }
}

// Getter 方法和 Setter 方法
sf::Vector2i Room::GetRoomSize() const { return RoomSize; }
int Room::GetSqureSize() const { return SqureSize; }
sf::Vector2f Room::GetWorldPosition() const { return WorldPosition; }
float Room::GetWidth() const { return static_cast<float>(RoomSize.x * SqureSize); }
float Room::GetHeight() const { return static_cast<float>(RoomSize.y * SqureSize); }
void Room::SetWorldPosition(const sf::Vector2f& pos) { WorldPosition = pos; }

// 核心接口：移除障碍物
bool Room::RemoveBlocksByKey(const string& condition)
{
    bool removedAny = false;
    for (auto it = objects.begin(); it != objects.end(); )
    {
        if ((*it)->GetType() == ObjType::Block)
        {
            Block* blockPtr = dynamic_cast<Block*>(it->get());
            // 检查钥匙是否匹配
            if (blockPtr && blockPtr->MatchCondition(condition))
            {
                sf::Vector2i pos = blockPtr->GetSqurePos();  // 获取位置
                blockPtr->UseKey();                          // 使用钥匙
                int index = pos.y * RoomSize.y + pos.x;
                if (index >= 0 && index < CollisonSqure.size())
                {
                    CollisonSqure[index] = false;
                    squres[index] = SqureType::Groud;        // 设为地面
                }
                it = objects.erase(it);                      // 从列表中移除
                removedAny = true;
                continue;
            }
        }
		++it;  // 防止在删除元素后继续迭代导致访问无效内存
    }
    return removedAny;
}

// 核心接口：搜索宝箱
ChestRewardData Room::SearchChest(const sf::Vector2f& centerPos)
{
    for (const auto& obj: objects)
    {
        if (obj->GetType() == ObjType::Chest)
        {
            Chest* chestPtr = dynamic_cast<Chest*>(obj.get());
            if (chestPtr && chestPtr->IsPlayerInrange(centerPos, 1.5f))
            {
                // 如果宝箱已打开
                if (chestPtr->IsOpen())
                {
                    cout << "宝箱已领取过奖励" << endl;
                    return ChestRewardData{ };
                }
                else
                {
                    chestPtr->OnInteract();
                    // 领取奖励
                    auto reward = chestPtr->ClaimReward();
                    chestPtr->PrintInfo();
                    return reward;
                }
            }
        }
    }
    return ChestRewardData{ };
}

// 新增补丁
bool Room::IsEmptyObjectSlot(int RoomX, int RoomY) const
{
    // 如果没有放置物品
    if (!IsObjectSlot(RoomX, RoomY))
        return false;

    sf::Vector2i pos(RoomX, RoomY);
    for (const auto& obj : objects)
    {
        if (obj->GetSqurePos() == pos)
            return false;
    }
    // 是物品位但没有物品
    return true;
}

void Room::ClearAllChests()
{
    int Size = RoomSize.y;  // 列数
    for (auto it = objects.begin(); it != objects.end(); )
    {
        if ((*it)->GetType() == ObjType::Chest)
        {
            sf::Vector2i pos = (*it)->GetSqurePos();
            int index = pos.y * Size + pos.x;
            squres[index] = SqureType::Groud;
            CollisonSqure[index] = false;
            it = objects.erase(it);  // 删除并返回下一个
        }
        else ++it;
    }
}

// ==================== 敌人接口实现 ====================

// 设置地图上的敌人
void Room::AddEnemy(const string& enemyName, int level, const sf::Vector2i& gridPos, bool canRespawn)
{
    // 创建敌人对象
    Enemy* enemy = new Enemy(enemyName, level, gridPos, canRespawn);
    AddObject(unique_ptr<Obj>(enemy));
}

// 搜索附近敌人
Enemy* Room::SearchEnemy(const sf::Vector2f& playerCenterPos) const
{
    for (const auto& obj: objects)
    {
        if (obj->GetType() == ObjType::Enemy)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
            if (enemy && !enemy->IsDefeated() &&
                enemy->IsPlayerInBattleRange(playerCenterPos))
                return enemy;
        }
    }
    return nullptr;
}

// 核心接口：开始与指定敌人的战斗
unique_ptr<BattleSystem> Room::StartBattleWithEnemy(Enemy* enemy, Player* player)
{
    if (!enemy)
    {
        cerr << "无法战斗，尝试与 nullptr 敌人开始战斗" << endl;
        return nullptr;
    }
    if (enemy->IsDefeated())
    {
        cout << "敌人已被击败，无法战斗！" << endl;
        return nullptr;
    }
    cout << "开始与敌人 " << enemy->GetName() << " 战斗！" << endl;
    return enemy->StartBattle(player);
}

// 根据位置查找敌人
Enemy* Room::GetEnemyAtPosition(const sf::Vector2i& gridPos) const
{
    for (const auto& obj : objects)
    {
        if (obj->GetType() == ObjType::Enemy)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
            if (enemy && enemy->GetSqurePos() == gridPos)
            {
                return enemy;
            }
        }
    }
    return nullptr;
}

bool clearfunction(const unique_ptr<Obj>& obj)
{
    if (obj->GetType() == ObjType::Enemy)
    {
        Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
        return enemy && enemy->IsDefeated() && !enemy->CanRespawn();
    }
    else return false;
}

// 击败敌人
unique_ptr<monbase::Codemon> Room::RemoveEnemy(Enemy* enemy, const ChestRewardData& reward)
{
    if (!enemy) return nullptr;
    enemy->OnDefeated();

    // 偷取精灵（不破坏敌人）
    unique_ptr<monbase::Codemon> stolenCodemon =
        enemy->StealRandomCodemon();

    // 掉落宝箱
    sf::Vector2i chestPos = enemy->GetSqurePos();
    chestPos.x += 1;
    AddChest(chestPos.x, chestPos.y, reward);

    // 只有不可重生的敌人才从列表中删除
    if (!enemy->CanRespawn())
    {
        objects.erase(
            remove_if(objects.begin(), objects.end(), clearfunction),
            objects.end());
    }
    return stolenCodemon;
}

// 重置房间内所有敌人状态
void Room::ResetAllEnemies()
{
    for (const auto& obj : objects)
    {
        if (obj->GetType() == ObjType::Enemy)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
            if (enemy)
            {
                // 重置敌人状态
                enemy->SetState(ObjState::Active);
                enemy->SetInteractable(true);
                enemy->ResetForBattle();
                cout << "成功重置敌人: " << enemy->GetName() << endl;
            }
        }
    }
}

// ==================== 地图完成接口 ====================

bool Room::isAllChestsOpened() const
{
    for (const auto& obj : objects)
    {
        if (obj->GetType() == ObjType::Chest)
        {
            const Chest* chest = dynamic_cast<const Chest*>(obj.get());
            if (chest && !chest->IsOpen())
            {
                return false; // 只要有一个没开，就不算完成
            }
        }
    }
	return true;  // 所有宝箱都已打开
}

bool Room::isBossDefeated() const
{
    for (const auto& obj : objects)
    {
        if (obj->GetType() == ObjType::Enemy)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
            if (enemy && !enemy->IsDefeated())
            {
                return false; // 还有 Boss 活着
            }
        }
    }
    return true; // 所有 Boss 都被击败
}

// ================ 序列化和反序列化接口 ================

void Room::Serialize(ostream& os) const
{
    // Room
    os << RoomSize.x << " " << RoomSize.y << " ";
    os << SqureSize << " ";
    os << WorldPosition.x << " " << WorldPosition.y << " ";
    // 格子类型
    for (int i = 0; i < RoomSize.x * RoomSize.y; i++)
    {
        os << static_cast<int>(squres[i]) << " ";
    }
    // 碰撞 
    for (int i = 0; i < RoomSize.x * RoomSize.y; i++)
    {
        os << (CollisonSqure[i] ? 1 : 0) << " ";
    }
    // 物品
    os << objects.size() << " ";
    for (const auto& obj : objects)
    {
        os << static_cast<int>(obj->GetType()) << " ";
        obj->Serialize(os);
    }
}

void Room::Deserialize(istream& is)
{
    // Room 基础
    is >> RoomSize.x >> RoomSize.y;
    is >> SqureSize;
    is >> WorldPosition.x >> WorldPosition.y;

    // 格子
    squres.resize(RoomSize.x * RoomSize.y);
    for (int i = 0; i < static_cast<int>(squres.size()); i++)
    {
        int t; is >> t;
        squres[i] = static_cast<SqureType>(t);
    }

    // 碰撞
    CollisonSqure.resize(RoomSize.x * RoomSize.y);
    for (int i = 0; i < static_cast<int>(CollisonSqure.size()); i++)
    {
        int temp; is >> temp;
        CollisonSqure[i] = (temp != 0);
    }

    // 物品
    int objCount = 0;
    is >> objCount;
    objects.clear();

    for (int i = 0; i < objCount; i++)
    {
        int typeInt = 0;
        is >> typeInt;
        ObjType type = static_cast<ObjType>(typeInt);
        unique_ptr<Obj> obj = CreateObjByType(type);
        if (obj)
        {
            // 反序列化对象内容
            obj->Deserialize(is);
            obj->SetPosition(obj->GetSqurePos(), SqureSize);
            // 通过 AddObject 将对象加入并同步更新 room 的 squres / CollisonSqure
            AddObject(move(obj));
        }
        else continue;
    }
}

unique_ptr<Obj> Room::CreateObjByType(ObjType type)
{
    switch (type)
    {
    case ObjType::Enemy:
        return make_unique<Enemy>();

    case ObjType::Chest:
        return make_unique<Chest>(0, 0, vector<unique_ptr<Item>>{}, "Chest");

    case ObjType::Block:
        return make_unique<Block>("");

    default:
        return nullptr;
    }
}