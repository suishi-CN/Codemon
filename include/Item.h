#pragma once
#include "Obj.h"
#include "battlesystem.h"
#include "codemon.h"
#include "species.h"
#include "Player.h"
#include <memory>
#include <vector>
#include <string>
#include <fstream>

class Enemy : public Obj
{
private:
    string name;              // 敌人名称
    int level;                // 敌人等级
    bool Canrespawn;          // 敌人能否重生
    // 纹理相关
    sf::Texture texture;           // 敌人纹理
    sf::Texture respawnTexture;    // 复活动画纹理
    sf::Sprite sprite;             // 敌人精灵
    string texturePath;            // 纹理路径
    // 当前战斗状态
    bool isDefeated = false;    // 是否被击败
    float respawnTimer = 0.0f;  // 重生计时器
    float respawnTime = 20.0f;  // 默认20秒后重生
    bool isRespawning = false;  // 是否正在复活
    bool canRespawn = true;     // 敌人能否重生
    float battleRange = 2.0f;   // 战斗触发范围
    bool dropsChest = true;     // 是否掉落宝箱
    // 私有方法：根据名称加载纹理
    bool LoadTexture();
    bool LoadRespawnTexture();
    // 私有方法：根据名称创建默认精灵
    bool CreateDefaultCodemon(int level);
	// 判断是否为首领
    bool IsBoss(const string& name) const;
    // 敌人数组：已经移动至共有部分
public:
    // 构造函数
    Enemy();
    Enemy(const string& enemyName, int enemyLevel, const sf::Vector2i& gridPos, bool canRespawn);
    void ApplyTextureToSprite(const sf::Texture& tex);
    // 敌人数组
    vector<unique_ptr<monbase::Codemon>> EnemyTeam;
    // 判断玩家是否在可触发战斗的范围内
    bool IsPlayerInBattleRange(const sf::Vector2f& playerPos) const;
    // 设置交互范围
    void SetBattleRange(float range) { battleRange = range; }
    float GetBattleRange() const { return battleRange; }
    // 开始战斗
    unique_ptr<BattleSystem> StartBattle(Player* player);
    // 敌人被击败后的行为
    bool OnDefeated();
    // 返回敌人队伍中的随机一只精灵（不修改敌人）
    unique_ptr<monbase::Codemon> StealRandomCodemon() const;
    // 宝箱掉落相关
    void SetDropChest(bool drop) { dropsChest = drop; }
    bool DoesDropChest() const { return dropsChest; }
    // 重生系统
    void SetRespawnTime(float seconds) { respawnTime = seconds; }
    float GetRespawnTime() const { return respawnTime; }
    bool IsDefeated() const { return isDefeated; }
    bool IsRespawning() const { return isRespawning; }  
    void SetCanRespawn(bool can) { canRespawn = can; }  
    bool CanRespawn() const { return canRespawn; }
    void StartRespawn();  // 开始复活过程
    void Respawn();       // 完成复活
    // 状态查询
    const string& GetName() const { return name; }
    int GetLevel() const { return level; }
    // 纹理路径获取
    const string& GetTexturePath() const { return texturePath; }
    // 重置敌人状态
    void ResetForBattle();
    // 基类方法重写
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    // 序列化/反序列化
    void Serialize(ostream& os) const;
    void Deserialize(istream& is);
};