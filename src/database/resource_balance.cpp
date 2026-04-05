#include "database.h"
#include "database/database_detail.h"
#include "config/player_balance.h"

using namespace std;
using namespace database_detail;

int expRequiredForNextLevel(const GameContext &ctx, int nextLevel)
{
    if (!ctx.gameData.contains("balance") || !ctx.gameData["balance"].is_object())
        return nextLevel * 100;
    if (!ctx.gameData["balance"].contains("level_exp") || !ctx.gameData["balance"]["level_exp"].is_array())
        return nextLevel * 100;

    for (const auto &row : ctx.gameData["balance"]["level_exp"])
    {
        if (asInt(row.value("level", json(-1)), -1) == nextLevel)
        {
            const int baseExp = asInt(row.value("exp_required", json(nextLevel * 100)), nextLevel * 100);

            double multiplier = player_balance::kLevelUpEarlyMultiplier;
            if (nextLevel >= 6)
                multiplier = player_balance::kLevelUpMidMultiplier;
            if (nextLevel >= 11)
                multiplier = player_balance::kLevelUpLateMultiplier;
            if (nextLevel >= 21)
                multiplier = player_balance::kLevelUpEndgameMultiplier;

            return max(player_balance::kMinimumExpRequirement, static_cast<int>(baseExp * multiplier));
        }
    }

    return max(player_balance::kMinimumExpRequirement, static_cast<int>(nextLevel * nextLevel * 22));
}

int itemAtkBonus(const GameContext &ctx, const string &itemId)
{
    const auto *item = getItemById(ctx, itemId);
    if (!item || !item->contains("stats") || !(*item)["stats"].is_object())
        return 0;
    return asInt((*item)["stats"].value("atk_bonus", json(0)), 0);
}

int itemDefBonus(const GameContext &ctx, const string &itemId)
{
    const auto *item = getItemById(ctx, itemId);
    if (!item || !item->contains("stats") || !(*item)["stats"].is_object())
        return 0;
    return asInt((*item)["stats"].value("def_bonus", json(0)), 0);
}

int itemHpBonus(const GameContext &ctx, const string &itemId)
{
    const auto *item = getItemById(ctx, itemId);
    if (!item || !item->contains("stats") || !(*item)["stats"].is_object())
        return 0;
    return asInt((*item)["stats"].value("hp_bonus", json(0)), 0);
}

int itemMpBonus(const GameContext &ctx, const string &itemId)
{
    const auto *item = getItemById(ctx, itemId);
    if (!item || !item->contains("stats") || !(*item)["stats"].is_object())
        return 0;
    if ((*item)["stats"].contains("mp_bonus"))
        return asInt((*item)["stats"]["mp_bonus"], 0);
    return 0;
}

int baseMaxHp(const Player &player)
{
    return player_balance::kBaseHpStart +
           (player.level - 1) * player_balance::kBaseHpPerLevel +
           player.stats.vit * player_balance::kBaseHpPerVit +
           player.stats.str * player_balance::kBaseHpPerStr;
}

int baseMaxMp(const Player &player)
{
    return player_balance::kBaseMpStart +
           (player.level - 1) * player_balance::kBaseMpPerLevel +
           player.stats.intl * player_balance::kBaseMpPerInt +
           (player.stats.agi / 2) * player_balance::kBaseMpPerTwoAgi;
}

int effectiveMaxHp(const GameContext &ctx, const Player &player)
{
    return baseMaxHp(player) + equippedItemBonus(ctx, player, "hp_bonus");
}

int effectiveMaxMp(const GameContext &ctx, const Player &player)
{
    return baseMaxMp(player) + equippedItemBonus(ctx, player, "mp_bonus");
}

void normalizePlayerResources(GameContext &ctx)
{
    ctx.player.max_hp = effectiveMaxHp(ctx, ctx.player);
    ctx.player.max_mp = effectiveMaxMp(ctx, ctx.player);
    ctx.player.hp = clampInt(ctx.player.hp, 0, ctx.player.max_hp);
    ctx.player.mp = clampInt(ctx.player.mp, 0, ctx.player.max_mp);

    if (ctx.activeCharacterIndex >= 0 && ctx.activeCharacterIndex < static_cast<int>(ctx.characters.size()))
        ctx.characters[static_cast<size_t>(ctx.activeCharacterIndex)] = ctx.player;
}
