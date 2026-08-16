#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "ItemType.h"
#include "ItemDB.h"
#include "Player.h"

class Shop
{
private:
	std::string shopName;
	std::map<ItemID, int> goods;

	//商品池
	const std::map<std::string, std::vector<std::pair<ItemID, int>>> unlockPools =
		{
			{"stage1",
			 {{ItemID::BasicAttackPotion, 100},
			  {ItemID::BasicDefencePotion, 100},
			  {ItemID::BasicHealthPotion, 100},
			  {ItemID::BasicMagicAttackPotion, 100},
			  {ItemID::BasicMagicDefencePotion, 100},
			  {ItemID::CopperSword, 200},
			  {ItemID::CopperWand, 200},
			  {ItemID::LeatherArmour, 100}}},
			{"stage2",
			 {{ItemID::NormalAttackPotion, 200},
			  {ItemID::NormalDefencePotion, 200},
			  {ItemID::NormalHealthPotion, 200},
			  {ItemID::NormalMagicAttackPotion, 200},
			  {ItemID::NormalMagicDefencePotion, 200},
			  {ItemID::IronSword, 400},
			  {ItemID::IronWand, 400},
			  {ItemID::CopperArmour, 200},
			  {ItemID::IronNecklace, 200},
			  {ItemID::GoldenNecklace, 200},
			  {ItemID::IronRing, 200},
			  {ItemID::GoldenRing, 200},
			  {ItemID::QuartzRing, 200}}},
			{"stage3",
			 {{ItemID::AdvancedAttackPotion, 300},
			  {ItemID::AdvancedDefencePotion, 300},
			  {ItemID::AdvancedHealthPotion, 300},
			  {ItemID::AdvancedMagicAttackPotion, 300},
			  {ItemID::AdvancedMagicDefencePotion, 300},
			  {ItemID::EnergyPotion, 200},
			  {ItemID::PurificationPotion, 250},
			  {ItemID::SteelSword, 600},
			  {ItemID::CrystalWand, 600},
			  {ItemID::IronArmour, 400},
			  {ItemID::RubyNecklace, 500},
			  {ItemID::SapphireNecklace, 500},
			  {ItemID::EmeraldNecklace, 500},
			  {ItemID::RubyRing, 500},
			  {ItemID::SapphireRing, 500},
			  {ItemID::EmeraldRing, 500}}},
			{"stage4",
			 {{ItemID::BloodragePotion, 450},
			  {ItemID::StonehidePotion, 450},
			  {ItemID::SacredElixir, 450},
			  {ItemID::ThiefPotion, 450},
			  {ItemID::ArcanePotion, 450},
			  {ItemID::GiantPotion, 450},
			  {ItemID::VoidPotion, 450},
			  {ItemID::DiamondSword, 1000},
			  {ItemID::DiamondWand, 1000},
			  {ItemID::SteelArmour, 600},
			  {ItemID::CrystalArmour, 600},
			  {ItemID::DiamondNecklace, 800},
			  {ItemID::DiamondRing, 800}}},
			{"stage5",
			 {{ItemID::DiamondArmour, 1000}}},
			{"stage6",
			 {{ItemID::DragonBoneSword, 1500},
			  {ItemID::DragonBoneWand, 1500},
			  {ItemID::DragonScaleArmour, 1500},
			  {ItemID::DragonBoneNecklace, 1200},
			  {ItemID::DragonBoneRing, 1200}}}};

	void addPool(const std::string &poolName)
	{
		auto it = unlockPools.find(poolName);
		if (it != unlockPools.end())
		{
			for (const auto &itemPair : it->second)
			{
				goods[itemPair.first] = itemPair.second;
			}
		}
	}

public:
	Shop(const std::string &name) : shopName(name) {}

	//供ShopUI使用的公开接口
	const auto &getUnlockPools() const { return unlockPools; }
	const auto &getGoods() const { return goods; }
	static const std::vector<std::string> &getStageOrder()
	{
		static const std::vector<std::string> stages = {
			"stage1", "stage2", "stage3", "stage4", "stage5", "stage6"};
		return stages;
	}

	//根据玩家进度刷新商品
	void refreshGoods(const Player &player)
	{
		goods.clear();

		addPool("stage1");

		for (const auto &stageName : player.reachedStage)
		{
			if (unlockPools.find(stageName) != unlockPools.end())
			{
				addPool(stageName);
			}
		}
	}

	//购买商品
	bool buy(Player &player, ItemID id)
	{
		auto it = goods.find(id);
		if (it == goods.end())
		{
			return false; //商品未解锁
		}
		int cost = it->second;
		if (!player.spendGold(cost))
		{
			return false; //金币不足
		}
		player.bag.addItem(ItemDB::getItem(id));
		return true;
	}

	bool sell(Player &player, ItemID id)
	{
		if (player.bag.getItemCount(id) <= 0)
			return false;
		auto itemTpl = ItemDB::getItem(id);
		int sellPrice = itemTpl->getPrice() / 2;
		if (player.bag.removeItem(id, 1))
		{
			player.earnGold(sellPrice);
			return true;
		}
		return false;
	}
};
