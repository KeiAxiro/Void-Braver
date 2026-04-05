#pragma once

#include <string>

#include "globals.h"

namespace state_helpers
{
    int inventoryCount(const Player &player, const std::string &itemId);
    void mergeInventory(Player &player);
    void addItem(Player &player, const std::string &itemId, int quantity, bool equipped = false, const std::string &slot = "");
    bool removeItem(Player &player, const std::string &itemId, int quantity);
    int itemBonusFromEquipped(const GameContext &ctx, const Player &player, const std::string &category);
    int getCooldown(const Player &player, const std::string &skillId);
    void setCooldown(Player &player, const std::string &skillId, int turns);
    void reduceCooldowns(Player &player);
} // namespace state_helpers
