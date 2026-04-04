#pragma once

#include <algorithm>
#include <string>
#include <vector>

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
};

// Context utama game yang dibawa ke semua state.
struct GameContext
{
    json gameData;
    Player player;
    std::vector<Player> characters;
    int activeCharacterIndex = -1;
    std::vector<GameState> stateStack{GameState::MainMenu};
    std::string gameDataPath = "data/game_data.json";
    std::string savePath = "data/save.json";
    bool running = true;
};

inline constexpr int STARTING_GOLD = 650;
inline constexpr int STARTING_HP = 100;
inline constexpr int STARTING_MP = 50;
inline constexpr int STARTING_STAT_POINTS = 5;
inline constexpr int STAT_POINTS_PER_LEVEL = 5;
inline constexpr int PAGE_SIZE = 5;
inline constexpr const char *MENU_SEPARATOR = "-------------------------------------------------------";
inline constexpr const char *TITLE_SEPARATOR = "========================================";

inline int clampInt(int value, int low, int high)
{
    return std::max(low, std::min(value, high));
}
