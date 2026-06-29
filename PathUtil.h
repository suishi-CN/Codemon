#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint> 
#include <iostream>
#include <string>
#include <cmath>
#include "Item.h"
#include "ItemDB.h"
using namespace std;

enum class ObjType
{
	None = 0,
	Chest = 1,  // 宝箱
	Enemy = 2,  // 敌人
	Block = 3,  // 路障
};

enum class ObjState
{
	Inactive,
	Active,
	Interacting,  // 交互中
	Disable,  // 禁用
	Destroy,  // 破坏
};

class Obj  // 物体基类
{
protected:
	ObjType type;                   // 物体类型
	ObjState state;                 // 当前状态
	sf::Vector2i SqurePos;          // 网格坐标
	sf::Vector2f WorldPos;          // 世界坐标
	sf::FloatRect CollisionBounds;  // 碰撞边界
	bool IsWalkable;                // 是否可通行
	bool IsInteractable;            // 是否可交互
	string name;                    // 物体名称

public:
	Obj(ObjType Type = ObjType::None, string Name = "Unnamed"):
		type(Type), state(ObjState::Active), IsWalkable(false),
		IsInteractable(false), name(Name){ }
	virtual ~Obj() = default;
	// 纯虚函数，更新和绘制
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
	// 纯虚函数，序列化和反序列化
	virtual void Serialize(ostream& os) const = 0;
	virtual void Deserialize(istream& is) = 0;
	// 虚函数
	virtual void OnInteract(){ }   // 空交互
	virtual void OnCollision(){ }  // 空碰撞
	virtual void SetPosition(const sf::Vector2i& squrePos, int squreSize)
	{
		SqurePos = squrePos;
		WorldPos.x = static_cast<float>(squrePos.x * squreSize);
		WorldPos.y = static_cast<float>(squrePos.y * squreSize);
		// 碰撞边界为整个瓦片
		CollisionBounds = sf::FloatRect(WorldPos.x, WorldPos.y,
			static_cast<float>(squreSize),
			static_cast<float>(squreSize));
	}
	// 检测玩家是否在交互范围内
	virtual bool IsPlayerInrange(const sf::Vector2f& playerPos, float range = 1.5) const
	{
		sf::Vector2f center(
			CollisionBounds.left + CollisionBounds.width / 2,
			CollisionBounds.top + CollisionBounds.height / 2);
		float dx = playerPos.x - center.x;
		float dy = playerPos.y - center.y;
		float distance = sqrt(dx * dx + dy * dy);
		return distance <= range * (CollisionBounds.width);
	}
	// Getter 方法
	ObjType GetType() const { return type; }
	ObjState GetState() const { return state; }
	sf::Vector2i GetSqurePos() const { return SqurePos; }
	sf::Vector2f GetWorldPos() const { return WorldPos; }
	sf::FloatRect GetCollisionBounds() const { return CollisionBounds; }
	bool GetIsWalkable() const { return IsWalkable; }
	bool GetIsInteractable() const { return IsInteractable; }
	std::string GetName() const { return name; }
	// Setter 方法
	void SetState(ObjState s) { state = s; }
	void SetWalkable(bool walkable) { IsWalkable = walkable; }
	void SetInteractable(bool interactable) { IsInteractable = interactable; }
	void SetCollisionBounds(const sf::FloatRect& bounds) { CollisionBounds = bounds; }
	// 辅助方法，判断是否可交互
	bool IsActive() const { return state == ObjState::Active; }
	bool CanInteract() const { return IsInteractable && IsActive(); }
};

// 用于序列化的工具函数
static void MapWriteString(ostream& os, const string& str) 
{
	uint32_t len = static_cast<uint32_t>(str.size());
	os << len << " ";
	os << str << " ";
}

static string MapReadString(istream& is) 
{
	int len; 
	if (!(is >> len)) 
		return "";
	if (len <= 0 || len > 256) 
		return "";
	string str;
	if (!(is >> str)) 
		return "";
	return str;
}

// 更新后的宝箱奖励结构体
struct ChestRewardData
{
	int exp = 0;
	int gold = 0;
	vector<unique_ptr<Item>> items;
	ChestRewardData() = default;
	// 禁止拷贝
	ChestRewardData(const ChestRewardData&) = delete;
	ChestRewardData& operator=(const ChestRewardData&) = delete;
	// 允许移动
	ChestRewardData(ChestRewardData&&) = default;
	ChestRewardData& operator=(ChestRewardData&&) = default;
	void Clear()
	{ exp = 0; gold = 0; items.clear(); }
};

class Chest : public Obj
{
private:
	int expReward;                     // 经验奖励
	int goldReward;                    // 金币奖励
	vector<unique_ptr<Item>> items;    // 物品奖励
	bool isOpen;                       // 是否已打开
	bool hasGivenReward;               // 是否已给予奖励
	sf::Texture textureChest;          // 纹理
	sf::Texture textures[3];           // 三种状态，其中一种已经弃用
	sf::Sprite sprite;                 // 精灵

public:
	// 构造函数
	Chest(int exp, int gold, vector<unique_ptr<Item>> Items, string Name = "Chest"): Obj(ObjType::Chest, Name)
	{
		expReward = exp;
		goldReward = gold;
		isOpen = false;
		hasGivenReward = false;
		// 转移物品所有权
		for (auto& item : Items)
		{
			if (item)
				items.push_back(move(item));
		}
		SetWalkable(false);
		SetInteractable(true);
		// 加载纹理
		if (!textures[0].loadFromFile("Resources/Chest111.png"))
		{
			cerr << "加载宝箱关闭图片失败！" << endl;
		}
		if (!textures[1].loadFromFile("Resources/Chest222.png"))
		{
			cerr << "加载宝箱打开中图片失败！" << endl;
		}
		if (!textures[2].loadFromFile("Resources/Chest333.png"))
		{
			cerr << "加载宝箱打开图片失败！" << endl;
		}
		sprite.setTexture(textures[0]);
		// 缩放到 64 * 64
		sf::Vector2u texSize = textures[0].getSize();
		float scaleX = 64.0f / texSize.x;
		float scaleY = 64.0f / texSize.y;
		sprite.setScale(scaleX, scaleY);
	}
	// 更新逻辑
	void Update(float deltaTime) override
	{
		// 不再需要动画进度更新
	}
	// 绘制
	void Draw(sf::RenderWindow& window) override
	{
		// 根据是否打开选择纹理
		int textureIndex = 0;
		if (isOpen)
			textureIndex = 2;  // 打开状态
		else
			textureIndex = 0;  // 关闭状态
		// 设置纹理
		sprite.setTexture(textures[textureIndex]);
		// 设置位置
		sprite.setPosition(WorldPos.x, WorldPos.y);
		// 绘制精灵
		window.draw(sprite);
	}
	// 交互
	void OnInteract() override
	{
		if (!isOpen && state == ObjState::Active)
		{
			isOpen = true;
			state = ObjState::Disable;
		}
	}
	// 领取奖励，返回结构体
	ChestRewardData ClaimReward()
	{
		if (isOpen && !hasGivenReward)
		{
			hasGivenReward = true;
			return GetReward();
		}
		return ChestRewardData{ };
	}
	// 获取奖励
	ChestRewardData GetReward() const
	{
		ChestRewardData reward;
		reward.exp = expReward;
		reward.gold = goldReward;
		// 克隆物品转移所有权
		for (const auto& item : items)
			if (item)
				reward.items.push_back(item->clone());
		return reward;
	}
	// Getter 方法
	int GetExpReward() const { return expReward; }
	int GetGoldReward() const { return goldReward; }
	int GetItemCount() const { return static_cast<int>(items.size()); }
	bool IsOpen() const { return isOpen; }
	bool HasGivenReward() const { return hasGivenReward; }
	bool HasReward() const { return expReward > 0 || goldReward > 0 || !items.empty(); }
	// Setter 方法
	void ClearItems() { items.clear(); }
	void SetExpReward(int exp) { expReward = exp; }
	void SetGoldReward(int gold) { goldReward = gold; }
	// 添加物品
	void AddItem(unique_ptr<Item> item)
	{
		if (item)
			items.push_back(move(item));
	}
	// 打印信息函数
	void PrintInfo() const
	{
		cout << "=============== 宝箱奖励 ===============" << endl;
		cout << "经验奖励: " << expReward << endl;
		cout << "金币奖励: " << goldReward << endl;
		int Size = items.size();
		for(int i = 0; i < Size; ++i)
			if (items[i])
				cout << "物品奖励: " << items[i]->getName() << endl;
		cout << "========================================" << endl;
	}
	// 序列化和反序列化
	void Serialize(ostream& os) const
	{
		// Obj 基础
		os << static_cast<int>(type) << " ";
		os << static_cast<int>(state) << " ";
		os << SqurePos.x << " " << SqurePos.y << " ";
		os << WorldPos.x << " " << WorldPos.y << " ";
		os << IsWalkable << " " << IsInteractable << " ";

		// Chest 状态
		os << isOpen << " ";
		os << hasGivenReward << " ";
		os << expReward << " ";
		os << goldReward << " ";

		// 物品
		os << items.size() << " ";
		for (const auto& item : items)
		{
			if (item)
			{
				os << 1 << " ";
				os << static_cast<int>(item->getType()) << " ";
				os << static_cast<int>(item->getID()) << " ";
				MapWriteString(os, item->getName());
			}
			else { os << 0 << " "; }
		}
	}
	void Deserialize(istream& is)
	{
		int typeInt, stateInt;
		is >> typeInt >> stateInt;
		type = static_cast<ObjType>(typeInt);
		state = static_cast<ObjState>(stateInt);

		is >> SqurePos.x >> SqurePos.y;
		is >> WorldPos.x >> WorldPos.y;
		is >> IsWalkable >> IsInteractable;
		is >> isOpen >> hasGivenReward;
		is >> expReward >> goldReward;

		// 恢复物品
		size_t itemCount;
		is >> itemCount;
		items.clear();

		for (size_t i = 0; i < itemCount; ++i)
		{
			int hasItem;
			is >> hasItem;
			if (hasItem)
			{
				int typeInt, idInt;
				is >> typeInt >> idInt;
				string name = MapReadString(is);
				auto item = ItemDB::getItem(static_cast<ItemID>(idInt));
				if (item)
					items.push_back(move(item));
			}
		}

		// 纹理（运行时加载）
		if (!textures[0].loadFromFile("Resources/Chest111.png")) {}
		if (!textures[1].loadFromFile("Resources/Chest222.png")) {}
		if (!textures[2].loadFromFile("Resources/Chest333.png")) {}

		sprite.setTexture(textures[isOpen ? 2 : 0]);
		sprite.setPosition(WorldPos.x, WorldPos.y);
		sf::Vector2u texSize = textures[0].getSize();
		sprite.setScale(64.f / texSize.x, 64.f / texSize.y);

		// 强制重置状态
		if (!isOpen)
		{
			state = ObjState::Active;
			SetInteractable(true);
		}
		else
		{
			state = ObjState::Disable;
			SetInteractable(false);
		}
	}
};

class Block : public Obj
{
private:
	sf::Texture textureBlock;   // 存储图片纹理
	sf::Sprite sprite;          // 用于绘制的精灵
	string requiredCondition;   // 需要什么钥匙才能开
	bool isDestroyed;           // 记录是否已经被破坏（钥匙使用后）

public:
	Block(const string condition, string Name = "Block"):
		Obj(ObjType::Block, Name), isDestroyed(false), requiredCondition(condition)
	{
		SetWalkable(false);      // 初始不可走
		SetInteractable(false);  // 初始不需要交互，钥匙直接生效
		// 加载路障贴图
		if (!textureBlock.loadFromFile("Resources/Key.png"))
		{
			cerr << "加载路障图片失败！" << endl;
		}
		sprite.setTexture(textureBlock);
		// 获取原始纹理尺寸
		sf::Vector2u texSize = textureBlock.getSize();
		// 计算缩放比例，将图片缩放到 64×64
		float scaleX = 64.0f / texSize.x;
		float scaleY = 64.0f / texSize.y;
		sprite.setScale(scaleX, scaleY);
	}
	// Update
	void Update(float deltaTime) override
	{
		// 如果已经被破坏，不再阻挡玩家
		if (isDestroyed && state != ObjState::Disable)
		{
			state = ObjState::Disable;
			SetWalkable(true);  // 变得可以通行
			// 改为绘制地砖
			if (!textureBlock.loadFromFile("Resources/Squre.png"))
			{
				cerr << "加载地砖图片失败！" << endl;
			}
			sprite.setTexture(textureBlock);
			// 获取原始纹理尺寸
			sf::Vector2u texSize = textureBlock.getSize();
			// 计算缩放比例，将图片缩放到 64×64
			float scaleX = 64.0f / texSize.x;
			float scaleY = 64.0f / texSize.y;
			sprite.setScale(scaleX, scaleY);
		}
	}
	// Draw
	void Draw(sf::RenderWindow& window) override
	{
		sprite.setPosition(WorldPos.x, WorldPos.y);
		window.draw(sprite);
	}
	// 检查钥匙是否匹配
	bool MatchCondition(const string& key) const
	{
		return requiredCondition == key;
	}
	// 核心接口：使用钥匙后调用此函数
	void UseKey()
	{
		if (state == ObjState::Active)
		{
			isDestroyed = true;      // 标记为已破坏
			SetWalkable(true);       // 变为可走
			SetInteractable(false);  // 变为不可交互
		}
	}
	// 序列化和反序列化
	void Serialize(ostream& os) const
	{
		os << static_cast<int>(type) << " ";
		os << static_cast<int>(state) << " ";
		os << SqurePos.x << " " << SqurePos.y << " ";
		os << WorldPos.x << " " << WorldPos.y << " ";
		os << IsWalkable << " " << IsInteractable << " ";
		MapWriteString(os, requiredCondition);
		os << isDestroyed << " ";
	}
	void Deserialize(istream& is)
	{
		int typeInt, stateInt;
		is >> typeInt >> stateInt;
		type = static_cast<ObjType>(typeInt);
		state = static_cast<ObjState>(stateInt);
		is >> SqurePos.x >> SqurePos.y;
		is >> WorldPos.x >> WorldPos.y;
		is >> IsWalkable >> IsInteractable;
		requiredCondition = MapReadString(is);
		is >> isDestroyed;

		// 纹理运行时加载
		if (isDestroyed)
			textureBlock.loadFromFile("Resources/Squre.png");
		else
			textureBlock.loadFromFile("Resources/Key.png");

		sprite.setTexture(textureBlock);
		sprite.setPosition(WorldPos.x, WorldPos.y);
		sf::Vector2u texSize = textureBlock.getSize();
		sprite.setScale(64.f / texSize.x, 64.f / texSize.y);
	}
};