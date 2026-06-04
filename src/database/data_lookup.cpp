#include "database.h"
#include "config.h"
#include <algorithm>
#include <string>

// ---- merged from src/database/data_lookup.cpp

using namespace std;
using namespace database_detail;

const json *findInArrayById(const json &arr, const string &id)
{
    if (!arr.is_array())
        return nullptr;

    for (const auto &item : arr)
    {
        if (item.contains("id") && item["id"].is_string() && item["id"].get<string>() == id)
            return &item;
    }
    return nullptr;
}

const json *getClassById(const GameContext &ctx, const string &classId)
{
    if (!ctx.gameData.contains("classes") || !ctx.gameData["classes"].is_array())
        return nullptr;
    return findInArrayById(ctx.gameData["classes"], classId);
}

const json *getItemById(const GameContext &ctx, const string &itemId)
{
    if (!ctx.gameData.contains("items") || !ctx.gameData["items"].is_array())
        return nullptr;
    return findInArrayById(ctx.gameData["items"], itemId);
}

const json *getSkillById(const GameContext &ctx, const string &skillId)
{
    if (!ctx.gameData.contains("classes") || !ctx.gameData["classes"].is_array())
        return nullptr;

    for (const auto &cls : ctx.gameData["classes"])
    {
        if (!cls.contains("skills") || !cls["skills"].is_array())
            continue;

        for (const auto &skill : cls["skills"])
        {
            if (skill.contains("id") && skill["id"].is_string() && skill["id"].get<string>() == skillId)
                return &skill;
        }
    }

    return nullptr;
}

const json *getDungeonById(const GameContext &ctx, const string &dungeonId)
{
    if (!ctx.gameData.contains("dungeons") || !ctx.gameData["dungeons"].is_array())
        return nullptr;
    return findInArrayById(ctx.gameData["dungeons"], dungeonId);
}

const json *getEnemyById(const GameContext &ctx, const string &enemyId)
{
    if (!ctx.gameData.contains("enemies") || !ctx.gameData["enemies"].is_array())
        return nullptr;
    return findInArrayById(ctx.gameData["enemies"], enemyId);
}

const json *findDungeonDepth(const json &dungeon, int depth)
{
    if (!dungeon.contains("depths") || !dungeon["depths"].is_array())
        return nullptr;

    for (const auto &row : dungeon["depths"])
    {
        if (row.contains("depth") && asInt(row["depth"], 0) == depth)
            return &row;
    }

    return nullptr;
}

const json *getDepthDialog(const json &dungeon, int depth, const string &event)
{
    const json *depthRow = findDungeonDepth(dungeon, depth);
    if (!depthRow)
        return nullptr;
    if (!depthRow->contains("dialog") || !(*depthRow)["dialog"].is_object())
        return nullptr;
    const auto &dialog = (*depthRow)["dialog"];
    if (!dialog.contains(event) || !dialog[event].is_array())
        return nullptr;
    return &dialog[event];
}

string itemCategory(const GameContext &ctx, const string &itemId)
{
    const auto *item = getItemById(ctx, itemId);
    if (!item)
        return {};
    return asString(item->value("category", json("")));
}

string classPrimaryStat(const GameContext &ctx, const string &classId)
{
    const auto *cls = getClassById(ctx, classId);
    if (!cls)
        return "STR";
    return asString(cls->value("primary_stat", json("STR")), "STR");
}

string itemRequiredClass(const GameContext &ctx, const string &itemId)
{
    const auto *item = getItemById(ctx, itemId);
    if (!item)
        return {};
    return asString(item->value("required_class_id", json("")));
}

string slotFromCategory(const string &category)
{
    if (category == "weapon")
        return "Weapon";
    if (category == "helmet")
        return "Helmet";
    if (category == "armor")
        return "Armor";
    if (category == "boots")
        return "Boots";
    if (category == "accessory")
        return "Accessory";
    return {};
}

bool isEquipmentCategory(const string &category)
{
    return !slotFromCategory(category).empty();
}


// ---- merged from src/database/resource_balance.cpp

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

void syncPlayerClassTier(const GameContext &ctx, Player &player)
{
    const auto *cls = getClassById(ctx, player.class_id);
    if (!cls)
    {
        player.class_tier_id = player.class_id;
        player.class_tier_name = player.class_id.empty() ? "-" : player.class_id;
        return;
    }

    string highestId = asString(cls->value("id", json(player.class_id)), player.class_id);
    string highestName = asString(cls->value("name", json(highestId)), highestId);

    if (cls->contains("tiers") && (*cls)["tiers"].is_array())
    {
        for (const auto &tier : (*cls)["tiers"])
        {
            if (player.level >= asInt(tier.value("level", json(999)), 999))
            {
                highestId = asString(tier.value("id", json(highestId)), highestId);
                highestName = asString(tier.value("name", json(highestName)), highestName);
            }
        }
    }

    player.class_tier_id = highestId;
    player.class_tier_name = highestName;
}

void normalizePlayerResources(GameContext &ctx)
{
    syncPlayerClassTier(ctx, ctx.player);
    ctx.player.max_hp = effectiveMaxHp(ctx, ctx.player);
    ctx.player.max_mp = effectiveMaxMp(ctx, ctx.player);
    ctx.player.hp = clampInt(ctx.player.hp, 0, ctx.player.max_hp);
    ctx.player.mp = clampInt(ctx.player.mp, 0, ctx.player.max_mp);

    if (ctx.activeCharacterIndex >= 0 && ctx.activeCharacterIndex < static_cast<int>(ctx.characters.size()))
        ctx.characters[static_cast<size_t>(ctx.activeCharacterIndex)] = ctx.player;
}
