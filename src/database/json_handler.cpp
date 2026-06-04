#include "database.h"
#include "config.h"
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// ---- merged from src/database/json_and_paths.cpp


using namespace std;

namespace database_detail
{
    int asInt(const json &j, int fallback)
    {
        if (j.is_number_integer())
            return j.get<int>();
        if (j.is_number_float())
            return static_cast<int>(j.get<double>());
        if (j.is_string())
        {
            try
            {
                return stoi(j.get<string>());
            }
            catch (...)
            {
                return fallback;
            }
        }
        if (j.is_boolean())
            return j.get<bool>() ? 1 : 0;
        return fallback;
    }

    string asString(const json &j, const string &fallback)
    {
        if (j.is_string())
            return j.get<string>();
        if (j.is_number_integer())
            return to_string(j.get<int>());
        if (j.is_number_float())
        {
            ostringstream oss;
            oss << j.get<double>();
            return oss.str();
        }
        if (j.is_boolean())
            return j.get<bool>() ? "true" : "false";
        return fallback;
    }

    bool fileExists(const string &path)
    {
        ifstream file(path);
        return file.good();
    }

    string resolveGameDataPath(const GameContext &ctx)
    {
        if (!ctx.gameDataPath.empty())
            return ctx.gameDataPath;
        return game_paths::kGameDataPath;
    }

    string resolveSavePath(const GameContext &ctx)
    {
        if (!ctx.savePath.empty())
            return ctx.savePath;
        return game_paths::kSaveFilePath;
    }

    string sanitizeIdPart(const string &value)
    {
        string out;
        for (char ch : value)
        {
            unsigned char uch = static_cast<unsigned char>(ch);
            if (isalnum(uch))
                out.push_back(static_cast<char>(tolower(uch)));
            else if (isspace(uch) || ch == '-' || ch == '_')
                out.push_back('_');
        }

        if (out.empty())
            out = "hero";

        string compact;
        bool prevUnderscore = false;
        for (char ch : out)
        {
            if (ch == '_')
            {
                if (!prevUnderscore)
                    compact.push_back(ch);
                prevUnderscore = true;
                continue;
            }

            compact.push_back(ch);
            prevUnderscore = false;
        }

        if (!compact.empty() && compact.front() == '_')
            compact.erase(compact.begin());
        if (!compact.empty() && compact.back() == '_')
            compact.pop_back();
        return compact.empty() ? "hero" : compact;
    }

    int equippedItemBonus(const GameContext &ctx, const Player &player, const string &bonusKey)
    {
        int total = 0;
        for (const auto &entry : player.inventory)
        {
            if (!entry.equipped)
                continue;

            const auto *item = getItemById(ctx, entry.item_id);
            if (!item || !item->contains("stats") || !(*item)["stats"].is_object())
                continue;

            total += asInt((*item)["stats"].value(bonusKey, json(0)), 0);
        }
        return total;
    }

    void addStarterItem(Player &player, const string &itemId, int quantity, bool equipped, const string &slot)
    {
        if (itemId.empty() || quantity <= 0)
            return;

        InventoryEntry entry;
        entry.item_id = itemId;
        entry.quantity = quantity;
        entry.equipped = equipped;
        entry.slot = slot;
        player.inventory.push_back(entry);
    }

    void giveStarterKit(GameContext &ctx)
    {
        auto giveIfExists = [&](const string &itemId, int quantity, bool equipped = false)
        {
            const json *item = getItemById(ctx, itemId);
            if (item == nullptr)
                return;

            string slot;
            if (equipped)
                slot = slotFromCategory(item->value("category", string()));

            addStarterItem(ctx.player, itemId, quantity, equipped, slot);
        };

        giveIfExists("healing_potion", 3, false);
        giveIfExists("mana_elixir", 2, false);

        if (ctx.player.class_id == "knight")
        {
            giveIfExists("leather_coif", 1, true);
            giveIfExists("padded_tunic", 1, true);
            giveIfExists("rusty_greatsword", 1, true);
            giveIfExists("traveler_s_boots", 1, true);
        }
        else if (ctx.player.class_id == "sorcerer")
        {
            giveIfExists("apprentice_wand", 1, true);
            giveIfExists("novice_robe", 1, true);
        }
        else if (ctx.player.class_id == "wanderer")
        {
            giveIfExists("short_bow", 1, true);
            giveIfExists("leather_cap", 1, true);
            giveIfExists("rough_tunic", 1, true);
        }
    }
} // namespace database_detail

// ---- merged from src/database/player_serialization.cpp

using namespace std;
using namespace database_detail;

namespace database_detail
{
    json playerToJson(const Player &player)
    {
        json out = json::object();
        out["id"] = player.id;
        out["name"] = player.name;
        out["class_id"] = player.class_id;
        out["class_tier_id"] = player.class_tier_id;
        out["class_tier_name"] = player.class_tier_name;
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
        {
            out["progress"]["dungeon_progress"] = json::object();
            for (const auto &entry : player.progress.dungeon_progress)
            {
                out["progress"]["dungeon_progress"][entry.dungeon_id] = {
                    {"unlocked_depth", entry.unlocked_depth},
                    {"highest_cleared_depth", entry.highest_cleared_depth},
                    {"completed", entry.completed}};
            }
        }
        return out;
    }

    void fillPlayerCore(Player &player, const json &source)
    {
        player.id = asString(source.value("id", json("")));
        player.name = asString(source.value("name", json("Hero")));
        player.class_id = asString(source.value("class_id", json("knight")));
        player.class_tier_id = asString(source.value("class_tier_id", json("")));
        player.class_tier_name = asString(source.value("class_tier_name", json("")));
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

    DungeonProgressEntry dungeonProgressFromJson(const string &dungeonId, const json &source)
    {
        DungeonProgressEntry entry;
        entry.dungeon_id = dungeonId;
        entry.unlocked_depth = asInt(source.value("unlocked_depth", json(1)), 1);
        entry.highest_cleared_depth = asInt(source.value("highest_cleared_depth", json(0)), 0);
        entry.completed = source.value("completed", false);
        return entry;
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

        player.progress.dungeon_progress.clear();
        if (progress.contains("dungeon_progress") && progress["dungeon_progress"].is_object())
        {
            for (auto it = progress["dungeon_progress"].begin(); it != progress["dungeon_progress"].end(); ++it)
                player.progress.dungeon_progress.push_back(dungeonProgressFromJson(it.key(), it.value()));
        }
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

    void upsertCharacter(PlayerList &characters, const Player &player)
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
} // namespace database_detail

// ---- merged from src/database/save_system.cpp


using namespace std;
using namespace database_detail;

bool loadGameData(GameContext &ctx)
{
    const string path = resolveGameDataPath(ctx);
    ifstream file(path);
    if (!file.is_open())
    {
        cerr << "Failed to open game data: " << path << '\n';
        return false;
    }

    try
    {
        file >> ctx.gameData;
        ctx.gameDataPath = path;
        return true;
    }
    catch (const exception &e)
    {
        cerr << "Failed to parse game data: " << e.what() << '\n';
        return false;
    }
}

bool loadSave(GameContext &ctx)
{
    ctx.characters.clear();

    const string path = resolveSavePath(ctx);
    if (!fileExists(path))
        return false;

    ifstream file(path);
    if (!file.is_open())
        return false;

    try
    {
        json saveRoot;
        file >> saveRoot;

        if (saveRoot.contains("characters") && saveRoot["characters"].is_array())
        {
            for (const auto &row : saveRoot["characters"])
                ctx.characters.push_back(playerFromJson(row));
        }
        else if (saveRoot.contains("character"))
        {
            ctx.characters.push_back(playerFromLegacyJson(saveRoot));
        }

        if (ctx.characters.empty())
            return false;

        for (auto &character : ctx.characters)
            syncPlayerClassTier(ctx, character);

        ctx.player = ctx.characters.front();
        ctx.activeCharacterIndex = 0;
        normalizePlayerResources(ctx);
        return true;
    }
    catch (const exception &e)
    {
        cerr << "Failed to parse save file: " << e.what() << '\n';
        return false;
    }
}

bool loadCharacter(GameContext &ctx, int index)
{
    if (index < 0 || index >= static_cast<int>(ctx.characters.size()))
        return false;

    ctx.player = ctx.characters[static_cast<size_t>(index)];
    ctx.activeCharacterIndex = index;
    syncPlayerClassTier(ctx, ctx.player);
    normalizePlayerResources(ctx);
    return true;
}

bool saveGame(const GameContext &ctx)
{
    if (ctx.player.id.empty())
        return true;

    PlayerList characters = ctx.characters;
    Player current = ctx.player;
    for (auto &character : characters)
        syncPlayerClassTier(ctx, character);
    syncPlayerClassTier(ctx, current);
    current.max_hp = effectiveMaxHp(ctx, current);
    current.max_mp = effectiveMaxMp(ctx, current);
    current.hp = clampInt(current.hp, 0, current.max_hp);
    current.mp = clampInt(current.mp, 0, current.max_mp);
    upsertCharacter(characters, current);

    json saveRoot = json::object();
    saveRoot["characters"] = json::array();
    for (const auto &character : characters)
        saveRoot["characters"].push_back(playerToJson(character));

    const string path = resolveSavePath(ctx);
    try
    {
        const filesystem::path parent = filesystem::path(path).parent_path();
        if (!parent.empty())
            filesystem::create_directories(parent);
    }
    catch (...)
    {
    }

    ofstream file(path);
    if (!file.is_open())
    {
        cerr << "Failed to write save file: " << path << '\n';
        return false;
    }

    file << saveRoot.dump(2) << '\n';
    return true;
}

void createNewGame(GameContext &ctx, const string &playerName, const string &classId)
{
    ctx.player = Player{};
    ctx.player.name = playerName.empty() ? "Hero" : playerName;
    ctx.player.class_id = classId.empty() ? "knight" : classId;
    ctx.player.level = 1;
    ctx.player.current_exp = 0;
    ctx.player.gold = STARTING_GOLD;
    ctx.player.stat_points = STARTING_STAT_POINTS;
    syncPlayerClassTier(ctx, ctx.player);

    ctx.player.id = sanitizeIdPart(ctx.player.name);
    bool duplicateId = true;
    int suffix = 1;
    while (duplicateId)
    {
        duplicateId = false;
        for (const auto &character : ctx.characters)
        {
            if (character.id == ctx.player.id)
            {
                duplicateId = true;
                ++suffix;
                ctx.player.id = sanitizeIdPart(ctx.player.name) + "_" + to_string(suffix);
                break;
            }
        }
    }

    const string primary = classPrimaryStat(ctx, ctx.player.class_id);
    if (primary == "INT")
        ctx.player.stats.intl += player_balance::kPrimaryStatBonusOnNewGame;
    else if (primary == "AGI")
        ctx.player.stats.agi += player_balance::kPrimaryStatBonusOnNewGame;
    else if (primary == "VIT")
        ctx.player.stats.vit += player_balance::kPrimaryStatBonusOnNewGame;
    else
        ctx.player.stats.str += player_balance::kPrimaryStatBonusOnNewGame;

    ctx.player.max_hp = baseMaxHp(ctx.player);
    ctx.player.max_mp = baseMaxMp(ctx.player);
    ctx.player.hp = ctx.player.max_hp;
    ctx.player.mp = ctx.player.max_mp;
    ctx.player.progress = Progress{};
    ctx.player.progress.dungeon_progress.clear();
    giveStarterKit(ctx);
    ctx.activeCharacterIndex = -1;
}


