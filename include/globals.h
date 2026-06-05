#pragma once

#include <algorithm>
#include <cstddef>
#include <string>

#include "config.h"
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

// Double linked list manual berbasis raw pointer.
// Dipakai sebagai pengganti container STL untuk data runtime yang dinilai SDA.
template <typename T>
class ManualList
{
    struct Node
    {
        T data;
        Node *prev = nullptr;
        Node *next = nullptr;

        explicit Node(const T &value) : data(value) {}
    };

    Node *head_ = nullptr;
    Node *tail_ = nullptr;
    std::size_t count_ = 0;

    Node *nodeAt(std::size_t index) const
    {
        Node *current = head_;
        for (std::size_t i = 0; current != nullptr && i < index; ++i)
            current = current->next;
        return current;
    }

public:
    class iterator
    {
        friend class ManualList<T>;
        Node *node_ = nullptr;

    public:
        explicit iterator(Node *node = nullptr) : node_(node) {}
        T &operator*() const { return node_->data; }
        T *operator->() const { return &node_->data; }
        iterator &operator++()
        {
            if (node_ != nullptr)
                node_ = node_->next;
            return *this;
        }
        bool operator!=(const iterator &other) const { return node_ != other.node_; }
        bool operator==(const iterator &other) const { return node_ == other.node_; }
    };

    class const_iterator
    {
        friend class ManualList<T>;
        const Node *node_ = nullptr;

    public:
        explicit const_iterator(const Node *node = nullptr) : node_(node) {}
        const T &operator*() const { return node_->data; }
        const T *operator->() const { return &node_->data; }
        const_iterator &operator++()
        {
            if (node_ != nullptr)
                node_ = node_->next;
            return *this;
        }
        bool operator!=(const const_iterator &other) const { return node_ != other.node_; }
        bool operator==(const const_iterator &other) const { return node_ == other.node_; }
    };

    ManualList() = default;

    ManualList(const ManualList &other)
    {
        for (const auto &entry : other)
            push_back(entry);
    }

    ManualList &operator=(const ManualList &other)
    {
        if (this == &other)
            return *this;
        clear();
        for (const auto &entry : other)
            push_back(entry);
        return *this;
    }

    ManualList(ManualList &&other) noexcept
        : head_(other.head_), tail_(other.tail_), count_(other.count_)
    {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.count_ = 0;
    }

    ManualList &operator=(ManualList &&other) noexcept
    {
        if (this == &other)
            return *this;
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        count_ = other.count_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.count_ = 0;
        return *this;
    }

    ~ManualList()
    {
        clear();
    }

    bool empty() const { return count_ == 0; }
    std::size_t size() const { return count_; }

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head_); }
    const_iterator end() const { return const_iterator(nullptr); }

    T &front() { return head_->data; }
    const T &front() const { return head_->data; }

    T &operator[](std::size_t index) { return nodeAt(index)->data; }
    const T &operator[](std::size_t index) const { return nodeAt(index)->data; }

    void push_back(const T &value)
    {
        Node *node = new Node(value);
        node->prev = tail_;
        if (tail_ != nullptr)
            tail_->next = node;
        else
            head_ = node;
        tail_ = node;
        ++count_;
    }

    iterator erase(iterator it)
    {
        Node *node = it.node_;
        if (node == nullptr)
            return end();

        Node *next = node->next;
        if (node->prev != nullptr)
            node->prev->next = node->next;
        else
            head_ = node->next;

        if (node->next != nullptr)
            node->next->prev = node->prev;
        else
            tail_ = node->prev;

        delete node;
        --count_;
        return iterator(next);
    }

    void clear()
    {
        Node *current = head_;
        while (current != nullptr)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        count_ = 0;
    }

    void swapData(std::size_t leftIndex, std::size_t rightIndex)
    {
        if (leftIndex == rightIndex)
            return;
        Node *left = nodeAt(leftIndex);
        Node *right = nodeAt(rightIndex);
        if (left == nullptr || right == nullptr)
            return;
        T temp = left->data;
        left->data = right->data;
        right->data = temp;
    }
};

template <typename T, typename Compare>
inline void manualSort(ManualList<T> &list, Compare compare)
{
    const std::size_t n = list.size();
    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j + 1 < n - i; ++j)
        {
            if (compare(list[j + 1], list[j]))
                list.swapData(j, j + 1);
        }
    }
}

class StateStack
{
    struct Node
    {
        GameState state;
        Node *next = nullptr;
        explicit Node(GameState value) : state(value) {}
    };

    Node *top_ = nullptr;
    int count_ = 0;

public:
    StateStack()
    {
        push_back(GameState::MainMenu);
    }

    StateStack(const StateStack &) = delete;
    StateStack &operator=(const StateStack &) = delete;

    ~StateStack()
    {
        clear();
    }

    bool empty() const { return top_ == nullptr; }

    GameState back() const
    {
        return top_ == nullptr ? GameState::Quit : top_->state;
    }

    void push_back(GameState state)
    {
        Node *node = new Node(state);
        node->next = top_;
        top_ = node;
        ++count_;
    }

    void pop_back()
    {
        if (top_ == nullptr)
            return;
        Node *old = top_;
        top_ = top_->next;
        delete old;
        --count_;
    }

    void clear()
    {
        while (top_ != nullptr)
            pop_back();
    }
};

// Stat dasar character.
struct Stats
{
    int str = Config::Defaults::PLAYER_STAT_STR;
    int intl = Config::Defaults::PLAYER_STAT_INT;
    int agi = Config::Defaults::PLAYER_STAT_AGI;
    int vit = Config::Defaults::PLAYER_STAT_VIT;
};

// Satu baris inventory.
struct InventoryEntry
{
    std::string item_id;
    int quantity = Config::Defaults::INVENTORY_QUANTITY;
    bool equipped = false;
    std::string slot;
};

// Menyimpan skill yang sedang cooldown.
struct CooldownEntry
{
    std::string skill_id;
    int remaining_turns = Config::Defaults::COOLDOWN_TURNS;
};

struct DungeonProgressEntry
{
    std::string dungeon_id;
    int unlocked_depth = Config::Progress::START_DEPTH;
    int highest_cleared_depth = Config::Progress::NO_DEPTH_CLEARED;
    bool completed = false;
};

// Progress dungeon per character.
struct Progress
{
    std::string current_dungeon;
    int current_depth = Config::Progress::START_DEPTH;
    int max_depth_unlocked = Config::Progress::NO_DEPTH_UNLOCKED;
    ManualList<DungeonProgressEntry> dungeon_progress;
};

// Data character aktif.
struct Player
{
    std::string id;
    std::string name;
    std::string class_id;
    std::string class_tier_id;
    std::string class_tier_name;
    int level = Config::Defaults::PLAYER_LEVEL;
    int current_exp = Config::Defaults::PLAYER_EXP;
    int gold = Config::Defaults::PLAYER_GOLD;
    Stats stats;
    int hp = Config::Defaults::PLAYER_HP;
    int max_hp = Config::Defaults::PLAYER_HP;
    int mp = Config::Defaults::PLAYER_MP;
    int max_mp = Config::Defaults::PLAYER_MP;
    int stat_points = Config::Defaults::PLAYER_STAT_POINTS;
    ManualList<InventoryEntry> inventory;
    ManualList<CooldownEntry> cooldowns;
    Progress progress;
};

using PlayerList = ManualList<Player>;
using StringList = ManualList<std::string>;
using IndexList = ManualList<std::size_t>;
using JsonPointerList = ManualList<const json *>;

// Context utama game yang dibawa ke semua state.
struct GameContext
{
    json gameData;
    Player player;
    PlayerList characters;
    int activeCharacterIndex = Config::Defaults::ACTIVE_CHARACTER_NONE;
    StateStack stateStack;
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
