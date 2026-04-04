#include "database.h"

using namespace std;

namespace
{
    json playerToJson(const Player &player)
    {
        json out = json::object();
        out["id"] = player.id;
        out["name"] = player.name;
        out["class_id"] = player.class_id;
        out["level"] = player.level;
        out["current_exp"] = player.current_exp;
        out["gold"] = player.gold;
        out["stats"] = {
            {"str", player.stats.str},
            {"int", player.stats.intl},
            {"agi", player.stats.agi},
            {"vit", player.stats.vit}};
        out["hp"] = player.hp;
        out["max_hp"] = player.max_hp;
        out["mp"] = player.mp;
        out["max_mp"] = player.max_mp;
        out["stat_points"] = player.stat_points;

        out["inventory"] = json::array();
        for (const auto &entry : player.inventory)
        {
            json row = json::object();
            row["item_id"] = entry.item_id;
            row["quantity"] = entry.quantity;
            row["equipped"] = entry.equipped;
            if (!entry.slot.empty())
                row["slot"] = entry.slot;
            out["inventory"].push_back(row);
        }

        out["cooldowns"] = json::array();
        for (const auto &entry : player.cooldowns)
        {
            out["cooldowns"].push_back({{"skill_id", entry.skill_id},
                                        {"remaining_turns", entry.remaining_turns}});
        }

        out["progress"] = json::object();
        out["progress"]["current_depth"] = player.progress.current_depth;
        out["progress"]["current_dungeon"] = player.progress.current_dungeon.empty() ? json(nullptr) : json(player.progress.current_dungeon);
        out["progress"]["max_depth_unlocked"] = player.progress.max_depth_unlocked;
        if (!player.progress.dungeon_progress.empty())
            out["progress"]["dungeon_progress"] = player.progress.dungeon_progress;
        return out;
    }

    void fillPlayerCore(Player &player, const json &source)
    {
        player.id = asString(source.value("id", json("")));
        player.name = asString(source.value("name", json("Hero")));
        player.class_id = asString(source.value("class_id", json("knight")));
        player.level = asInt(source.value("level", json(1)), 1);
        player.current_exp = asInt(source.value("current_exp", json(0)), 0);
        player.gold = asInt(source.value("gold", json(STARTING_GOLD)), STARTING_GOLD);

        if (source.contains("stats") && source["stats"].is_object())
        {
            const auto &stats = source["stats"];
            player.stats.str = asInt(stats.value("str", json(10)), 10);
            player.stats.intl = asInt(stats.value("int", json(10)), 10);
            player.stats.agi = asInt(stats.value("agi", json(10)), 10);
            player.stats.vit = asInt(stats.value("vit", json(10)), 10);
        }

        player.hp = asInt(source.value("hp", json(STARTING_HP)), STARTING_HP);
        player.max_hp = asInt(source.value("max_hp", json(baseMaxHp(player))), baseMaxHp(player));
        player.mp = asInt(source.value("mp", json(STARTING_MP)), STARTING_MP);
        player.max_mp = asInt(source.value("max_mp", json(baseMaxMp(player))), baseMaxMp(player));
        player.stat_points = asInt(source.value("stat_points", json(0)), 0);
    }

    void fillInventory(Player &player, const json &source)
    {
        player.inventory.clear();
        if (!source.contains("inventory") || !source["inventory"].is_array())
            return;

        for (const auto &row : source["inventory"])
        {
            InventoryEntry entry;
            entry.item_id = asString(row.value("item_id", json("")));
            entry.quantity = asInt(row.value("quantity", json(1)), 1);
            entry.equipped = row.contains("equipped") ? row["equipped"].get<bool>() : false;
            entry.slot = asString(row.value("slot", json("")));
            player.inventory.push_back(entry);
        }
    }

    void fillCooldowns(Player &player, const json &source)
    {
        player.cooldowns.clear();
        if (!source.contains("cooldowns") || !source["cooldowns"].is_array())
            return;

        for (const auto &row : source["cooldowns"])
        {
            CooldownEntry entry;
            entry.skill_id = asString(row.value("skill_id", json("")));
            entry.remaining_turns = asInt(row.value("remaining_turns", json(0)), 0);
            player.cooldowns.push_back(entry);
        }
    }

    void fillProgress(Player &player, const json &source)
    {
        player.progress = Progress{};
        if (!source.contains("progress") || !source["progress"].is_object())
            return;

        const auto &progress = source["progress"];
        player.progress.current_depth = asInt(progress.value("current_depth", json(1)), 1);
        if (progress.contains("current_dungeon") && !progress["current_dungeon"].is_null())
            player.progress.current_dungeon = asString(progress["current_dungeon"]);
        player.progress.max_depth_unlocked = asInt(progress.value("max_depth_unlocked", json(0)), 0);

        if (progress.contains("dungeon_progress") && progress["dungeon_progress"].is_object())
            player.progress.dungeon_progress = progress["dungeon_progress"];
        else
            player.progress.dungeon_progress = json::object();
    }

    Player playerFromJson(const json &source)
    {
        Player player;
        fillPlayerCore(player, source);
        fillInventory(player, source);
        fillCooldowns(player, source);
        fillProgress(player, source);
        return player;
    }

    Player playerFromLegacyJson(const json &root)
    {
        Player player;
        if (root.contains("character") && root["character"].is_object())
            fillPlayerCore(player, root["character"]);
        fillInventory(player, root);
        fillCooldowns(player, root);
        fillProgress(player, root);
        return player;
    }

    void upsertCharacter(vector<Player> &characters, const Player &player)
    {
        for (auto &entry : characters)
        {
            if (entry.id == player.id)
            {
                entry = player;
                return;
            }
        }
        characters.push_back(player);
    }
} // namespace
