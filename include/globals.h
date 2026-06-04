#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "config/game_paths.h"
#include "config/game_rules.h"
#include "../lib/nlohmann/json.hpp"

using json = nlohmann::json;

// State utama game.
enum class GameState
{
    MainMenu,
    Hub,
    Battle,
    Quit
};

// Stat dasar character.
struct Stats
{
    int str = 10;
    int intl = 10;
    int agi = 10;
    int vit = 10;
};

// Satu baris inventory.
struct InventoryEntry
{
    std::string item_id;
    int quantity = 1;
    bool equipped = false;
    std::string slot;
};

// Menyimpan skill yang sedang cooldown.
struct CooldownEntry
{
    std::string skill_id;
    int remaining_turns = 0;
};

// Progress dungeon per character.
struct Progress
{
    std::string current_dungeon;
    int current_depth = 1;
    int max_depth_unlocked = 0;
    json dungeon_progress = json::object();
};

struct Quest
{
    std::string id;
    std::string target_enemy_id;
    std::string target_enemy_name;
    int target_amount = 0;
    int current_amount = 0;
    int reward_exp = 0;
    int reward_gold = 0;
};

struct QuestsState
{
    std::vector<Quest> board_quests;
    bool has_active_quest = false;
    Quest active_quest;
};

// Data character aktif.
struct Player
{
    std::string id;
    std::string name;
    std::string class_id;
    int level = 1;
    int current_exp = 0;
    int gold = 500;
    Stats stats;
    int hp = 100;
    int max_hp = 100;
    int mp = 50;
    int max_mp = 50;
    int stat_points = 0;
    std::vector<InventoryEntry> inventory;
    std::vector<CooldownEntry> cooldowns;
    Progress progress;
    QuestsState quests;
};

// Context utama game yang dibawa ke semua state.
struct GameContext
{
    json gameData;
    Player player;
    std::vector<Player> characters;
    int activeCharacterIndex = -1;
    std::vector<GameState> stateStack{GameState::MainMenu};
    std::string gameDataPath = game_paths::kGameDataPath;
    std::string savePath = game_paths::kSaveFilePath;
    bool running = true;
};

// Alias lama tetap dipertahankan supaya fitur lama tidak rusak,
// tetapi sumber angka utamanya sekarang ada di folder include/config/.
inline constexpr int STARTING_GOLD = game_rules::kStartingGold;
inline constexpr int STARTING_HP = game_rules::kStartingHp;
inline constexpr int STARTING_MP = game_rules::kStartingMp;
inline constexpr int STARTING_STAT_POINTS = game_rules::kStartingStatPoints;
inline constexpr int STAT_POINTS_PER_LEVEL = game_rules::kStatPointsPerLevel;
inline constexpr int PAGE_SIZE = game_rules::kDefaultPageSize;
inline constexpr const char *MENU_SEPARATOR = game_rules::kMenuSeparator;
inline constexpr const char *TITLE_SEPARATOR = game_rules::kTitleSeparator;

inline int clampInt(int value, int low, int high)
{
    return std::max(low, std::min(value, high));
}
