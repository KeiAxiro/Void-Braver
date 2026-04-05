#include "database.h"
#include "database/database_detail.h"

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
