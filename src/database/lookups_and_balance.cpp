#include "database.h"

using namespace std;

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

            double multiplier = 0.95;
            if (nextLevel >= 6)
                multiplier = 0.80;
            if (nextLevel >= 11)
                multiplier = 0.68;
            if (nextLevel >= 21)
                multiplier = 0.58;

            return max(80, static_cast<int>(baseExp * multiplier));
        }
    }
    return max(80, static_cast<int>(nextLevel * nextLevel * 22));
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

int baseMaxHp(const Player &player)
{
    // Basis HP dinaikkan sedikit supaya early game tetap menantang,
    // tapi pemain pemula tidak langsung habis dalam satu depth.
    return 65 + (player.level - 1) * 7 + player.stats.vit * 4 + player.stats.str;
}

int baseMaxMp(const Player &player)
{
    return 40 + (player.level - 1) * 4 + player.stats.intl * 3 + (player.stats.agi / 2);
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
