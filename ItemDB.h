#pragma once
#include <SFML/Graphics.hpp>
#include "codemon.h"
#include "ItemType.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

//通用效果类，映射到Codemon::MoveEffect
class Effect
{
public:
	monbase::MoveEffect type;
	int amount;

	Effect(monbase::MoveEffect t, int a) : type(t), amount(a) {}

	void apply(monbase::Codemon* target)
	{
		if (!target) return;
		target->applyTemporaryBoost(type, amount);
	}
};

class Item
{
protected:
	ItemID id;               //道具ID
	std::string name;        //道具名
	std::string description; //描述
	int price;               //价格
	ItemType type;           //道具类型
	EquipSlot slot;          //装备栏位
	std::vector<std::unique_ptr<Effect>> effects; //智能指针管理效果

	sf::Texture texture;
	sf::Sprite sprite;
	bool hasTexture = false;

public:
	Item(ItemID i, std::string n, std::string desc, int p, ItemType t, EquipSlot s)
		: id(i), name(n), description(desc), price(p), type(t), slot(s) {}

	//克隆函数
	std::unique_ptr<Item> clone() const
	{
		std::unique_ptr<Item> newItem = std::make_unique<Item>(id, name, description, price, type, slot);
		for (const auto& eff : effects)
		{
			newItem->addEffect(std::make_unique<Effect>(eff->type, eff->amount));
		}
		return newItem;
	}

	//添加效果
	void addEffect(std::unique_ptr<Effect> effect)
	{
		effects.push_back(std::move(effect));
	}

	//使用道具
	virtual bool use(monbase::Codemon* target)
	{
		if (!target) return false;
		for (const auto& eff : effects)
		{
			eff->apply(target);
		}
		return true;
	}

	//装备穿戴（永久加成）
	bool equip(monbase::Codemon* target)
	{
		if (!target || type != ItemType::EQUIPMENT) return false;
		for (auto& eff : effects)
		{
			target->applyEquipmentBoost(eff->type, eff->amount, id, slot);
		}
		return true;
	}

	//装备卸下
	bool unequip(monbase::Codemon* target)
	{
		if (!target || type != ItemType::EQUIPMENT) return false;
		for (auto& eff : effects)
		{
			target->removeEquipmentBoost(eff->type, eff->amount, slot);
		}
		return true;
	}

	//纹理相关方法
	bool loadTexture(const std::string& texturePath)
	{
		if (texture.loadFromFile(texturePath)) {
			sprite.setTexture(texture);
			hasTexture = true;
			return true;
		}
		std::cerr << "警告: 无法加载纹理文件: " << texturePath << std::endl;
		return false;
	}

	void setTexture(const sf::Texture& tex)
	{
		texture = tex;
		sprite.setTexture(texture);
		hasTexture = true;
	}

	const sf::Texture& getTexture() const { return texture; }
	sf::Sprite& getSprite() { return sprite; }
	bool hasValidTexture() const { return hasTexture; }

	//设置精灵位置和缩放
	void setSpritePosition(float x, float y)
	{ 
		sprite.setPosition(x, y);
	}

	void setSpriteScale(float scaleX, float scaleY)
	{ 
		sprite.setScale(scaleX, scaleY);
	}

	//Getter
	EquipSlot getSlot() const { return slot; }
	ItemID getID() const { return id; }
	ItemType getType() const { return type; }
	std::string getName() const { return name; }
	std::string getDescription() const { return description; }
	int getPrice() const { return price; }

};