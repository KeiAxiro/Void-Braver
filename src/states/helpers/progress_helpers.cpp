#include "state_helpers.h"

#include <algorithm>

using namespace std;

namespace state_helpers
{
    json &ensureDungeonProgress(Player &player, const json &dungeon)
    {
        if (!player.progress.dungeon_progress.is_object())
            player.progress.dungeon_progress = json::object();

        const string dungeonId = dungeon.value("id", string());
        if (!player.progress.dungeon_progress.contains(dungeonId) || !player.progress.dungeon_progress[dungeonId].is_object())
        {
            int unlockedDepth = 1;
            int highestCleared = 0;

            if (player.progress.current_dungeon == dungeonId && player.progress.max_depth_unlocked > 0)
            {
                unlockedDepth = max(1, player.progress.max_depth_unlocked);
                highestCleared = max(0, unlockedDepth - 1);
            }

            player.progress.dungeon_progress[dungeonId] = {
                {"unlocked_depth", unlockedDepth},
                {"highest_cleared_depth", highestCleared},
                {"completed", false}};
        }
        return player.progress.dungeon_progress[dungeonId];
    }

    int getDungeonUnlockedDepth(Player &player, const json &dungeon)
    {
        json &progress = ensureDungeonProgress(player, dungeon);
        return max(1, progress.value("unlocked_depth", 1));
    }

    bool isDungeonCompleted(Player &player, const json &dungeon)
    {
        json &progress = ensureDungeonProgress(player, dungeon);
        return progress.value("completed", false);
    }

    int dungeonMaxDepth(const json &dungeon)
    {
        if (!dungeon.contains("depths") || !dungeon["depths"].is_array())
            return 1;
        return static_cast<int>(dungeon["depths"].size());
    }

    bool isFinalDungeonLocked(GameContext &ctx, const json &dungeon)
    {
        if (dungeon.value("id", string()) != "tahta_kehampaan")
            return false;
        if (!ctx.gameData.contains("dungeons") || !ctx.gameData["dungeons"].is_array())
            return false;

        for (const auto &row : ctx.gameData["dungeons"])
        {
            if (row.value("id", string()) == "tahta_kehampaan")
                continue;
            if (!isDungeonCompleted(ctx.player, row))
                return true;
        }
        return false;
    }
} // namespace state_helpers
