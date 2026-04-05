#pragma once

#include <string>

#include "globals.h"

namespace state_helpers
{
    void printHubHeader(const GameContext &ctx);
    void printStateHeader(const GameContext &ctx, const std::string &title);
    void showItemDetails(const GameContext &ctx, const json &item, int ownedCount = -1, int equippedCount = -1);
    std::string unlockedTierName(const GameContext &ctx);
    void printClassDetails(const GameContext &ctx);
    std::string itemDisplayName(const GameContext &ctx, const std::string &itemId);
    void showCharacterOverview(const GameContext &ctx);
} // namespace state_helpers
