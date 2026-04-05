#pragma once

#include <cstddef>

#include "globals.h"

namespace state_helpers
{
    void allocateStats(GameContext &ctx);
    bool equipInventoryEntry(GameContext &ctx, std::size_t index);
    bool unequipInventoryEntry(GameContext &ctx, std::size_t index);
    void useConsumable(GameContext &ctx, std::size_t index);
    void inventoryMenu(GameContext &ctx);
    void skillMenu(const GameContext &ctx);
    void characterMenu(GameContext &ctx);
} // namespace state_helpers
