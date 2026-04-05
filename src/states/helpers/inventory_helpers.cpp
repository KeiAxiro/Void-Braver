#include "states/inventory_helpers.h"

#include "database.h"

#include <algorithm>

using namespace std;

namespace state_helpers
{
    int inventoryCount(const Player &player, const string &itemId)
    {
        int total = 0;
        for (const auto &entry : player.inventory)
        {
            if (entry.item_id == itemId)
                total += entry.quantity;
        }
        return total;
    }

    void mergeInventory(Player &player)
    {
        vector<InventoryEntry> merged;
        for (const auto &entry : player.inventory)
        {
            if (entry.quantity <= 0)
                continue;

            bool handled = false;
            for (auto &slot : merged)
            {
                if (slot.item_id == entry.item_id && slot.equipped == entry.equipped && slot.slot == entry.slot)
                {
                    slot.quantity += entry.quantity;
                    handled = true;
                    break;
                }
            }

            if (!handled)
                merged.push_back(entry);
        }
        player.inventory = merged;
    }

    void addItem(Player &player, const string &itemId, int quantity, bool equipped, const string &slot)
    {
        if (quantity <= 0)
            return;

        for (auto &entry : player.inventory)
        {
            if (entry.item_id == itemId && entry.equipped == equipped && entry.slot == slot)
            {
                entry.quantity += quantity;
                return;
            }
        }

        InventoryEntry entry;
        entry.item_id = itemId;
        entry.quantity = quantity;
        entry.equipped = equipped;
        entry.slot = slot;
        player.inventory.push_back(entry);
    }

    bool removeItem(Player &player, const string &itemId, int quantity)
    {
        if (quantity <= 0)
            return true;
        if (inventoryCount(player, itemId) < quantity)
            return false;

        for (auto it = player.inventory.begin(); it != player.inventory.end() && quantity > 0;)
        {
            if (it->item_id != itemId)
            {
                ++it;
                continue;
            }

            const int taken = min(quantity, it->quantity);
            it->quantity -= taken;
            quantity -= taken;
            if (it->quantity <= 0)
                it = player.inventory.erase(it);
            else
                ++it;
        }

        mergeInventory(player);
        return true;
    }

    int itemBonusFromEquipped(const GameContext &ctx, const Player &player, const string &category)
    {
        int total = 0;
        for (const auto &entry : player.inventory)
        {
            if (!entry.equipped)
                continue;
            if (category == "atk")
                total += itemAtkBonus(ctx, entry.item_id);
            else if (category == "def")
                total += itemDefBonus(ctx, entry.item_id);
            else if (category == "hp")
                total += itemHpBonus(ctx, entry.item_id);
            else if (category == "mp")
                total += itemMpBonus(ctx, entry.item_id);
        }
        return total;
    }

    int getCooldown(const Player &player, const string &skillId)
    {
        for (const auto &entry : player.cooldowns)
        {
            if (entry.skill_id == skillId)
                return entry.remaining_turns;
        }
        return 0;
    }

    void setCooldown(Player &player, const string &skillId, int turns)
    {
        for (auto &entry : player.cooldowns)
        {
            if (entry.skill_id == skillId)
            {
                entry.remaining_turns = turns;
                return;
            }
        }
        player.cooldowns.push_back({skillId, turns});
    }

    void reduceCooldowns(Player &player)
    {
        for (auto &entry : player.cooldowns)
            entry.remaining_turns = max(0, entry.remaining_turns - 1);
    }
} // namespace state_helpers
