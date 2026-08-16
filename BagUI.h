#pragma once
#include <memory>
#include <map>
#include "Item.h"
#include "ItemDB.h"

class Bag
{
private:
	std::map<ItemID, std::pair<std::unique_ptr<Item>, int>> inventory;

public:
    //添加道具
	void addItem(std::unique_ptr<Item> item, int c = 1)
	{
		if (!item) return;
        ItemID id = item->getID();
		inventory[id].first = std::move(item);
		inventory[id].second += c;
	}

    //使用道具（使用消耗品/穿戴装备）
    bool use(ItemID id, monbase::Codemon* target)
    {
        auto it = inventory.find(id);
        if (it == inventory.end() || it->second.second <= 0) return false;

        Item* itemPtr = it->second.first.get();
        bool success = false;

        if (itemPtr->getType() == ItemType::CONSUMABLE)
        {
            success = itemPtr->use(target);
            if (success)
            {
                it->second.second--;
                if (it->second.second <= 0) inventory.erase(id);
            }
        }
        else if (itemPtr->getType() == ItemType::EQUIPMENT)
        {
            EquipSlot newSlot = itemPtr->getSlot();

            if (target->isSlotEquipped(newSlot))
            {
                ItemID oldId = target->getEquippedItemInSlot(newSlot);
                auto oldItem = ItemDB::getItem(oldId);

                if (oldItem)
                {
                    bool unequipSuccess = oldItem->unequip(target);
                    if (unequipSuccess)
                    {
                        addItem(std::move(oldItem));
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            success = itemPtr->equip(target);
            if (success)
            {
                inventory.erase(id);
            }
        }
        return success;
    }

    //卸下装备
    bool unequip(ItemID id, monbase::Codemon* target)
    {
        bool found = false;
        EquipSlot slot = EquipSlot::WEAPON;

        for (const auto& pair : target->getEquippedItems())
        {
            if (pair.second == id)
            {
                found = true;
                slot = pair.first;
                break;
            }
        }

        if (!found)
        {
            return false;
        }

        auto itemInstance = ItemDB::getItem(id);
        if (!itemInstance) return false;

        bool success = itemInstance->unequip(target);
        if (success)
        {
            addItem(std::move(itemInstance));
        }
        return success;
    }

    //移除道具
    bool removeItem(ItemID id, int count)
    {
        auto it = inventory.find(id);
        if (it != inventory.end())
        {
            if (it->second.second >= count)
            {
                it->second.second -= count;
                if (it->second.second <= 0)
                {
                    inventory.erase(it);
                }
                return true;
            }
        }
        return false;
    }

    //获取道具数量
	int getItemCount(ItemID id) const
	{
		auto it = inventory.find(id);
		return (it != inventory.end()) ? it->second.second : 0;
	}

    size_t getInventorySize() const
    {
        return inventory.size();
    }

    const auto& getInventory() const
    {
        return inventory;
    }
};
