#include "../include/Enemy.h"
#include "../include/species.h"
#include "../include/PathUtil.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

static int RandomNumber(int min = 0, int max = 10)
{
    static bool seeded = false;
    if (!seeded)
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    return min + (rand() % (max - min + 1));
}
// 无参构造
Enemy::Enemy():
    Obj(ObjType::Enemy, "Unknown"),
    level(1),
    canRespawn(false),
    isDefeated(false),
    isRespawning(false),
    respawnTimer(0.f),
    respawnTime(5.f),
    battleRange(1.f),
    dropsChest(true)
{
    SqurePos = { 0, 0 };
    WorldPos = { 0.f, 0.f };
    CollisionBounds = sf::FloatRect(0, 0, 64.f, 64.f);
    SetWalkable(false);
    SetInteractable(true);
}

// 构造函数
Enemy::Enemy(const string& enemyName, int enemyLevel, const sf::Vector2i& gridPos, bool canRespawn)
    :Obj(ObjType::Enemy, enemyName), name(enemyName), level(enemyLevel), canRespawn(canRespawn)
{
    // 设置网格位置
    SqurePos = gridPos;
    // 计算世界位置
    WorldPos.x = static_cast<float>(gridPos.x * 64);
    WorldPos.y = static_cast<float>(gridPos.y * 64);
    // 设置碰撞边界
    CollisionBounds = sf::FloatRect(WorldPos.x, WorldPos.y, 64.0f, 64.0f);
    // 设置敌人属性
    SetWalkable(false);      // 敌人不可通行
    SetInteractable(true);   // 敌人可交互
    state = ObjState::Active;
    // 加载纹理
    if (!LoadTexture()) 
        cerr << "加载敌人纹理失败！" << name << endl;
    if (!LoadRespawnTexture())
        cerr << "加载敌人复活纹理失败！" << name << endl;
    // 创建默认精灵
    CreateDefaultCodemon(level);
	// 初始化敌人状态
    for (auto& codemon : EnemyTeam)
        if (codemon)
            codemon->rest();
    // 初始化其他状态
    isDefeated = false;
    isRespawning = false;
    respawnTimer = 0.0f;
    canRespawn = true;
}

// 配置
// 水系
const vector<function<unique_ptr<monbase::Codemon>(int)>> WaterBoss = 
{
    [](int lvl) { return make_unique<Shuilanlan>(lvl); },
    [](int lvl) { return make_unique<Haibao>(lvl); },
    [](int lvl) { return make_unique<Guaiguaihu>(lvl); },
    [](int lvl) { return make_unique<FeicuiShuimu>(lvl); }
};

// 火系
const vector<function<unique_ptr<monbase::Codemon>(int)>> FireBoss = 
{
    [](int lvl) { return make_unique<Huohua>(lvl); },
    [](int lvl) { return make_unique<KeaiYuan>(lvl); },
    [](int lvl) { return make_unique<ShuidiShe>(lvl); },
    [](int lvl) { return make_unique<Hualing>(lvl); }
};

// 草系
const vector<function<unique_ptr<monbase::Codemon>(int)>> GrassBoss = 
{
    [](int lvl) { return make_unique<Jimi>(lvl); },
    [](int lvl) { return make_unique<Amiyate>(lvl); },
    [](int lvl) { return make_unique<ShenlanJing>(lvl); },
    [](int lvl) { return make_unique<JiayouHaikui>(lvl); }
};

// 电系
const vector<function<unique_ptr<monbase::Codemon>(int)>> ElectricBoss = 
{
    [](int lvl) { return make_unique<Xiaoxingguang>(lvl); },
    [](int lvl) { return make_unique<Mengyou>(lvl); },
    [](int lvl) { return make_unique<DanxiaoManyu>(lvl); },
    [](int lvl) { return make_unique<XiaoShijiu>(lvl); }
};

// 龙系
const vector<function<unique_ptr<monbase::Codemon>(int)>> DragonBoss = 
{
    [](int lvl) { return make_unique<DatouGulong>(lvl); },
    [](int lvl) { return make_unique<DaerMaodou>(lvl); },
    [](int lvl) { return make_unique<XiaoShijiu>(lvl); },
    [](int lvl) { return make_unique<ShuidiShe>(lvl); }
};

bool Enemy::IsBoss(const string& name) const
{
    return name.find("Boss") != string::npos ||
           name.find("首领") != string::npos;
}

void Enemy::ApplyTextureToSprite(const sf::Texture& tex)
{
    sprite.setTexture(tex);
    sprite.setTextureRect(sf::IntRect(0, 0, tex.getSize().x, tex.getSize().y));
    // 进行缩放
    sf::Vector2u texSize = tex.getSize();
    float scaleX = 64.0f / texSize.x;
    float scaleY = 64.0f / texSize.y;
    sprite.setScale(scaleX, scaleY);
}

// 根据名称加载纹理
bool Enemy::LoadTexture()
{
    texturePath = "Resources/" + name + ".png";
    if (!loadTextureFromUtf8(texture, texturePath))
    {
        cerr << "加载敌人纹理失败！" << texturePath << endl;
        // 尝试加载默认纹理
        if (!texture.loadFromFile("Resources/Enemy_Default.png"))
        {
            cerr << "加载默认敌人纹理失败！" << endl;
            return false;
        }
        texturePath = "Resources/Enemy_Default.png";
    }
    // 设置精灵
    ApplyTextureToSprite(texture);
    return true;
}

bool Enemy::LoadRespawnTexture()
{
    if (canRespawn)
    {
        if (!respawnTexture.loadFromFile("Resources/respawning.png"))
        {
            cerr << "加载敌人复活纹理失败！" << endl;
            // 如果复活纹理加载失败，使用正常纹理
            respawnTexture = texture;
            return false;
        }
    }
    return true;
}

// 根据名称创建默认精灵
bool Enemy::CreateDefaultCodemon(int level)
{
    // 水系
    if (name.find("水系") != string::npos ||
        name.find("Water") != string::npos)
    {
        if (IsBoss(name))
        {
            for (auto& f : WaterBoss)
                EnemyTeam.push_back(f(level));
        }
        else
        {
            vector<int> used = { -1, -1 };
            for (int i = 0; i < 2; )
            {
                int r = RandomNumber() % 4;
                if (r != used[0] && r != used[1])
                {
                    used[i] = r;
                    EnemyTeam.push_back(WaterBoss[r](level)); ++i;
                }
            }
        }
        return true;
    }

    // 火系
    if (name.find("火系") != string::npos ||
        name.find("Fire") != string::npos)
    {
        if (IsBoss(name))
        {
            for (auto& f : WaterBoss)
                EnemyTeam.push_back(f(level));
        }
        else
        {
            vector<int> used = { -1, -1 };
            for (int i = 0; i < 2; )
            {
                int r = RandomNumber() % 4;
                if (r != used[0] && r != used[1])
                {
                    used[i] = r;
                    EnemyTeam.push_back(FireBoss[r](level)); ++i;
                }
            }
        }
        return true;
    }

    // 草系
    if (name.find("草系") != string::npos ||
        name.find("Grass") != string::npos)
    {
        if (IsBoss(name))
        {
            for (auto& f : WaterBoss)
                EnemyTeam.push_back(f(level));
        }
        else
        {
            vector<int> used = { -1, -1 };
            for (int i = 0; i < 2; )
            {
                int r = RandomNumber() % 4;
                if (r != used[0] && r != used[1])
                {
                    used[i] = r;
                    EnemyTeam.push_back(GrassBoss[r](level)); ++i;
                }
            }
        }
        return true;
    }

    // 电系
    if (name.find("电系") != string::npos ||
        name.find("Electric") != string::npos)
    {
        if (IsBoss(name))
        {
            for (auto& f : WaterBoss)
                EnemyTeam.push_back(f(level));
        }
        else
        {
            vector<int> used = { -1, -1 };
            for (int i = 0; i < 2; )
            {
                int r = RandomNumber() % 4;
                if (r != used[0] && r != used[1])
                {
                    used[i] = r;
                    EnemyTeam.push_back(ElectricBoss[r](level)); ++i;
                }
            }
        }
        return true;
    }

    // 龙系
    if (name.find("龙系") != string::npos ||
        name.find("Dragon") != string::npos)
    {
        if (IsBoss(name))
        {
            for (auto& f : WaterBoss)
                EnemyTeam.push_back(f(level));
        }
        else
        {
            vector<int> used = { -1, -1 };
            for (int i = 0; i < 2; )
            {
                int r = RandomNumber() % 4;
                if (r != used[0] && r != used[1])
                {
                    used[i] = r;
                    EnemyTeam.push_back(DragonBoss[r](level)); ++i;
                }
            }
        }
        return true;
    }
    return false;
}

// 判断玩家是否在可触发战斗的范围内
bool Enemy::IsPlayerInBattleRange(const sf::Vector2f& playerPos) const 
{
    // 已被击败或非活动的敌人不可战斗
    if (isDefeated || state != ObjState::Active) 
        return false;
    float checkRange = battleRange;
    // 计算玩家与敌人中心的距离
    sf::Vector2f center(
        CollisionBounds.left + CollisionBounds.width / 2,
        CollisionBounds.top + CollisionBounds.height / 2);
    float dx = playerPos.x - center.x;
    float dy = playerPos.y - center.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= checkRange * CollisionBounds.width;
}

// 核心接口：开始战斗
unique_ptr<BattleSystem> Enemy::StartBattle(Player* player)
{
    if (isDefeated || state != ObjState::Active)
        return nullptr;
    if (!player || EnemyTeam.empty())
        return nullptr;
    for(auto& enemy : EnemyTeam)
        if (enemy)
            enemy->rest();
    auto battle = make_unique<BattleSystem>(player, EnemyTeam);
    return battle;
}

// 敌人被击败后的行为
bool Enemy::OnDefeated()
{
    // 设置状态
    isDefeated = true;
    isRespawning = false;
    respawnTimer = 0.0f;
    if (!canRespawn)
        state = ObjState::Disable;    // 不能复活 -> 完全禁用
    else state = ObjState::Inactive;  // 可以复活 -> 非活动状态
    // 设置不可交互
    SetInteractable(false);
    // 通知设置宝箱
    return true;
}

// 返回敌人队伍中的随机一只精灵
unique_ptr<monbase::Codemon> Enemy::StealRandomCodemon() const
{
    if (EnemyTeam.empty())
        return nullptr;
    int index = RandomNumber(0, EnemyTeam.size());
    if(index== EnemyTeam.size())
		return nullptr;
    return EnemyTeam[index]->clone();
}

// 开始复活过程
void Enemy::StartRespawn()
{
    if (!canRespawn || !isDefeated)
        return;
    isRespawning = true;
    respawnTimer = 0.0f;
    // 切换到复活纹理
    ApplyTextureToSprite(respawnTexture);
}

// 完成复活
void Enemy::Respawn()
{
    isDefeated = false;
    isRespawning = false;
    respawnTimer = 0.0f;
    state = ObjState::Active;
    SetInteractable(true);
    // 切换回正常纹理
    ApplyTextureToSprite(texture);
    // 重置精灵状态
    ResetForBattle();
}

// 重置战斗状态
void Enemy::ResetForBattle() 
{
    for (auto& codemon : EnemyTeam)
        if (codemon)
            codemon->rest();
}

// Update函数
void Enemy::Update(float deltaTime)
{
    if (state == ObjState::Disable)
        return;
    // 如果被击败，处理复活计时
    if (isDefeated && canRespawn)
    {
        respawnTimer += deltaTime;
        float beginRespawnTime = respawnTime * 0.7f;
        // 开始复活的条件
        if (!isRespawning && respawnTimer >= beginRespawnTime)
        {
            StartRespawn();
        }
        // 复活完成的逻辑
        if (respawnTimer >= respawnTime)
        {
            Respawn();
        }
    }
}

void Enemy::Draw(sf::RenderWindow& window)
{
    // 不绘制禁用敌人
    if (state == ObjState::Disable)
        return;
    // 被击败但还未开始复活的敌人不绘制
    if (isDefeated && !isRespawning)
        return;
    // 设置精灵位置
    sprite.setPosition(WorldPos.x, WorldPos.y);
    // 复活动画效果
    if (isRespawning)  // 正在复活状态
    {
        // 计算复活进度
        float beginRespawnTime = respawnTime * 0.7f;
        float respawnProgress = (respawnTimer - beginRespawnTime) / (respawnTime - beginRespawnTime);
        respawnProgress = min(max(respawnProgress, 0.0f), 1.0f);
        // 闪烁效果
        float alpha = 128 + 127 * sin(respawnProgress * 3.14159f * 4.0f);
        sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
        // 直接绘制，保持原始大小
        window.draw(sprite);
    }
    else
    {
        // 正常状态绘制
        sprite.setColor(sf::Color(255, 255, 255, 255));
        window.draw(sprite);
    }
}

// 序列化
void Enemy::Serialize(std::ostream& os) const
{
    os << static_cast<int>(type) << " ";
    os << static_cast<int>(state) << " ";
    os << SqurePos.x << " " << SqurePos.y << " ";
    os << WorldPos.x << " " << WorldPos.y << " ";
    os << IsWalkable << " " << IsInteractable << " ";

    MapWriteString(os, name);
    os << level << " ";
    os << isDefeated << " ";
    os << respawnTimer << " ";
    os << respawnTime << " ";
    os << battleRange << " ";
    os << dropsChest << " ";
    os << isRespawning << " ";
    os << canRespawn << " ";

    // ===== EnemyTeam =====
    os << EnemyTeam.size() << " ";
    for (auto& codemon : EnemyTeam)
    {
        if (codemon)
        {
            os << 1 << " ";
            MapWriteString(os, codemon->getName());
            os << codemon->getCodemonId() << " ";
            os << codemon->getLevel() << " ";
            os << codemon->getHP() << " ";
            os << codemon->getEnergy() << " ";
        }
        else os << 0 << " ";
    }
}

// 反序列化
void Enemy::Deserialize(std::istream& is)
{
    int typeInt, stateInt;
    is >> typeInt >> stateInt;
    type = static_cast<ObjType>(typeInt);
    state = static_cast<ObjState>(stateInt);

    is >> SqurePos.x >> SqurePos.y;
    is >> WorldPos.x >> WorldPos.y;
    is >> IsWalkable >> IsInteractable;

    name = MapReadString(is);
    is >> level;
    is >> isDefeated >> respawnTimer >> respawnTime;
    is >> battleRange >> dropsChest;
    is >> isRespawning >> canRespawn;

    // 纹理
    if (!loadTextureFromUtf8(texture, "Resources/" + name + ".png"))
        texture.loadFromFile("Resources/Enemy_Default.png");

    LoadRespawnTexture();
    sprite.setTexture(isRespawning ? respawnTexture : texture);
    sf::Vector2u texSize = texture.getSize();
    sprite.setScale(64.f / texSize.x, 64.f / texSize.y);
    CollisionBounds = sf::FloatRect(WorldPos.x, WorldPos.y, 64.f, 64.f);

    // EnemyTeam
    size_t teamSize;
    is >> teamSize;
    EnemyTeam.clear();

    for (size_t i = 0; i < teamSize; ++i)
    {
        int hasCodemon;
        is >> hasCodemon;
        if (hasCodemon)
        {
            string codemonName = MapReadString(is);
            unique_ptr<monbase::Codemon> codemon;
            int id, lvl, hp, energy;
            is >> id >> lvl >> hp >> energy;

            if (codemonName == "Shuilanlan" || codemonName == "水蓝蓝") codemon = make_unique<Shuilanlan>(lvl);
            else if (codemonName == "Huohua" || codemonName == "火花") codemon = make_unique<Huohua>(lvl);
            else if (codemonName == "Jimi" || codemonName == "吉米") codemon = make_unique<Jimi>(lvl);
            else if (codemonName == "Haibao" || codemonName == "海豹战士") codemon = make_unique<Haibao>(lvl);
            else if (codemonName == "Guaiguaihu" || codemonName == "乖乖鹄") codemon = make_unique<Guaiguaihu>(lvl);
            else if (codemonName == "DaerMaodou" || codemonName == "大耳帽兜") codemon = make_unique<DaerMaodou>(lvl);
            else if (codemonName == "Hualing" || codemonName == "画精灵") codemon = make_unique<Hualing>(lvl);
            else if (codemonName == "Xiaoxingguang" || codemonName == "小星光") codemon = make_unique<Xiaoxingguang>(lvl);
            else if (codemonName == "Mengyou" || codemonName == "梦游") codemon = make_unique<Mengyou>(lvl);
            else if (codemonName == "Amiyate" || codemonName == "阿米亚特") codemon = make_unique<Amiyate>(lvl);
            else if (codemonName == "DatouGulong" || codemonName == "大头骨龙") codemon = make_unique<DatouGulong>(lvl);
            else if (codemonName == "DanxiaoManyu" || codemonName == "胆小鳗鱼") codemon = make_unique<DanxiaoManyu>(lvl);
            else if (codemonName == "FeicuiShuimu" || codemonName == "翡翠水母") codemon = make_unique<FeicuiShuimu>(lvl);
            else if (codemonName == "ShenlanJing" || codemonName == "深蓝鲸") codemon = make_unique<ShenlanJing>(lvl);
            else if (codemonName == "ShuidiShe" || codemonName == "水滴蛇") codemon = make_unique<ShuidiShe>(lvl);
            else if (codemonName == "JiayouHaikui" || codemonName == "加油海葵") codemon = make_unique<JiayouHaikui>(lvl);
            else if (codemonName == "XiaoShijiu" || codemonName == "小狮鹫") codemon = make_unique<XiaoShijiu>(lvl);
            else if (codemonName == "KeaiYuan" || codemonName == "可爱猿") codemon = make_unique<KeaiYuan>(lvl);
            else codemon = make_unique<Jimi>(lvl);

            if (codemon)
            {
                codemon->setCodemonId(id);
                codemon->setHP(hp);
                codemon->setEnergy(energy);
                codemon->rest();
                EnemyTeam.push_back(move(codemon));
            }
        }
    }
}