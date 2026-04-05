#pragma once

#include "globals.h"

namespace state_helpers
{
    json &ensureDungeonProgress(Player &player, const json &dungeon);
    int getDungeonUnlockedDepth(Player &player, const json &dungeon);
    bool isDungeonCompleted(Player &player, const json &dungeon);
    int dungeonMaxDepth(const json &dungeon);
    bool isFinalDungeonLocked(GameContext &ctx, const json &dungeon);
} // namespace state_helpers
