#pragma once

#include <string>

#include "globals.h"

// Detail implementasi database/save system.
// File lain cukup include database.h kecuali sedang mengerjakan src/database.
namespace database_detail
{
    int asInt(const json &value, int fallback = 0);
    std::string asString(const json &value, const std::string &fallback = "");
    bool fileExists(const std::string &path);
    std::string resolveGameDataPath(const GameContext &ctx);
    std::string resolveSavePath(const GameContext &ctx);
    std::string sanitizeIdPart(const std::string &value);
    int equippedItemBonus(const GameContext &ctx, const Player &player, const std::string &statKey);
    void addStarterItem(Player &player, const std::string &itemId, int quantity, bool equipped, const std::string &slot);
    void giveStarterKit(GameContext &ctx);
    json playerToJson(const Player &player);
    Player playerFromJson(const json &source);
    Player playerFromLegacyJson(const json &root);
    void upsertCharacter(PlayerList &characters, const Player &player);
}
