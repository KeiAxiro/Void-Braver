#include "database.h"
#include "database/database_detail.h"
#include "config/player_balance.h"

#include <filesystem>
#include <fstream>
#include <iostream>

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
    normalizePlayerResources(ctx);
    return true;
}

bool saveGame(const GameContext &ctx)
{
    if (ctx.player.id.empty())
        return true;

    vector<Player> characters = ctx.characters;
    Player current = ctx.player;
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
    ctx.player.progress.dungeon_progress = json::object();
    giveStarterKit(ctx);
    ctx.activeCharacterIndex = -1;
}
