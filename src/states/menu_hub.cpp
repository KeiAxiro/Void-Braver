#include "states.h"
#include "database.h"
#include "config.h"
#include "utils/console_ui.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <string>

// ---- merged from src/states/helpers/input_helpers.cpp

using namespace std;

namespace state_helpers
{
    void clsScreen()
    {
        system("cls||clear");
    }

    mt19937 &rng()
    {
        static mt19937 gen{random_device{}()};
        return gen;
    }

    int randInt(int minValue, int maxValue)
    {
        if (maxValue < minValue)
            swap(minValue, maxValue);
        uniform_int_distribution<int> dist(minValue, maxValue);
        return dist(rng());
    }

    double randUnit()
    {
        uniform_real_distribution<double> dist(Config::Math::UNIT_ROLL_MIN, Config::Math::UNIT_ROLL_MAX);
        return dist(rng());
    }

    string trim(const string &value)
    {
        size_t start = 0;
        size_t end = value.size();
        while (start < end && isspace(static_cast<unsigned char>(value[start])))
            ++start;
        while (end > start && isspace(static_cast<unsigned char>(value[end - 1])))
            --end;
        return value.substr(start, end - start);
    }

    string toLower(string value)
    {
        for (char &ch : value)
            ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        return value;
    }

    string readLine()
    {
        string input;
        getline(cin >> ws, input);
        return trim(input);
    }

    bool tryParseInt(const string &input, int &value)
    {
        try
        {
            size_t processed = 0;
            const int parsed = stoi(input, &processed);
            if (processed != input.size())
                return false;
            value = parsed;
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    int getPrimaryStatValue(const Player &player, const string &primaryStat)
    {
        if (primaryStat == "INT")
            return player.stats.intl;
        if (primaryStat == "AGI")
            return player.stats.agi;
        if (primaryStat == "VIT")
            return player.stats.vit;
        return player.stats.str;
    }
} // namespace state_helpers

// ---- merged from src/states/helpers/inventory_helpers.cpp

using namespace std;

namespace state_helpers
{
    int inventoryCount(const Player &player, const string &itemId)
    {
        int total = 0;
        for (const auto &entry : player.inventory)
        {
            if (entry.item_id == itemId)
                total += entry.quantity;
        }
        return total;
    }

    void mergeInventory(Player &player)
    {
        ManualList<InventoryEntry> merged;
        for (const auto &entry : player.inventory)
        {
            if (entry.quantity <= 0)
                continue;

            bool handled = false;
            for (auto &slot : merged)
            {
                if (slot.item_id == entry.item_id && slot.equipped == entry.equipped && slot.slot == entry.slot)
                {
                    slot.quantity += entry.quantity;
                    handled = true;
                    break;
                }
            }

            if (!handled)
                merged.push_back(entry);
        }
        player.inventory = merged;
    }

    void addItem(Player &player, const string &itemId, int quantity, bool equipped, const string &slot)
    {
        if (quantity <= 0)
            return;

        for (auto &entry : player.inventory)
        {
            if (entry.item_id == itemId && entry.equipped == equipped && entry.slot == slot)
            {
                entry.quantity += quantity;
                return;
            }
        }

        InventoryEntry entry;
        entry.item_id = itemId;
        entry.quantity = quantity;
        entry.equipped = equipped;
        entry.slot = slot;
        player.inventory.push_back(entry);
    }

    bool removeItem(Player &player, const string &itemId, int quantity)
    {
        if (quantity <= 0)
            return true;
        if (inventoryCount(player, itemId) < quantity)
            return false;

        for (auto it = player.inventory.begin(); it != player.inventory.end() && quantity > 0;)
        {
            if (it->item_id != itemId)
            {
                ++it;
                continue;
            }

            const int taken = min(quantity, it->quantity);
            it->quantity -= taken;
            quantity -= taken;
            if (it->quantity <= 0)
                it = player.inventory.erase(it);
            else
                ++it;
        }

        mergeInventory(player);
        return true;
    }

    int itemBonusFromEquipped(const GameContext &ctx, const Player &player, const string &category)
    {
        int total = 0;
        for (const auto &entry : player.inventory)
        {
            if (!entry.equipped)
                continue;
            if (category == "atk")
                total += itemAtkBonus(ctx, entry.item_id);
            else if (category == "def")
                total += itemDefBonus(ctx, entry.item_id);
            else if (category == "hp")
                total += itemHpBonus(ctx, entry.item_id);
            else if (category == "mp")
                total += itemMpBonus(ctx, entry.item_id);
        }
        return total;
    }

    int getCooldown(const Player &player, const string &skillId)
    {
        for (const auto &entry : player.cooldowns)
        {
            if (entry.skill_id == skillId)
                return entry.remaining_turns;
        }
        return 0;
    }

    void setCooldown(Player &player, const string &skillId, int turns)
    {
        for (auto &entry : player.cooldowns)
        {
            if (entry.skill_id == skillId)
            {
                entry.remaining_turns = turns;
                return;
            }
        }
        player.cooldowns.push_back({skillId, turns});
    }

    void reduceCooldowns(Player &player)
    {
        for (auto &entry : player.cooldowns)
            entry.remaining_turns = max(Config::Defaults::COOLDOWN_TURNS, entry.remaining_turns - Config::Math::ONE);
    }
} // namespace state_helpers

// ---- merged from src/states/helpers/progress_helpers.cpp

using namespace std;

namespace state_helpers
{
    DungeonProgressEntry &ensureDungeonProgress(Player &player, const json &dungeon)
    {
        const string dungeonId = dungeon.value("id", string());
        for (auto &entry : player.progress.dungeon_progress)
        {
            if (entry.dungeon_id == dungeonId)
                return entry;
        }

        DungeonProgressEntry entry;
        entry.dungeon_id = dungeonId;
        entry.unlocked_depth = Config::Progress::START_DEPTH;
        entry.highest_cleared_depth = Config::Progress::NO_DEPTH_CLEARED;
        entry.completed = false;

        if (player.progress.current_dungeon == dungeonId && player.progress.max_depth_unlocked > Config::Progress::NO_DEPTH_UNLOCKED)
        {
            entry.unlocked_depth = max(Config::Progress::START_DEPTH, player.progress.max_depth_unlocked);
            entry.highest_cleared_depth = max(Config::Progress::NO_DEPTH_CLEARED, entry.unlocked_depth - Config::Progress::DEPTH_UNLOCK_STEP);
        }

        player.progress.dungeon_progress.push_back(entry);
        return player.progress.dungeon_progress[player.progress.dungeon_progress.size() - 1];
    }

    int getDungeonUnlockedDepth(Player &player, const json &dungeon)
    {
        DungeonProgressEntry &progress = ensureDungeonProgress(player, dungeon);
        const string dungeonId = dungeon.value("id", string());
        int maxDepth = Config::Progress::START_DEPTH;
        int levelUnlockedDepth = Config::Progress::START_DEPTH;

        if (dungeon.contains("depths") && dungeon["depths"].is_array())
        {
            maxDepth = max(Config::Progress::START_DEPTH, static_cast<int>(dungeon["depths"].size()));
            int fallbackDepth = Config::Progress::START_DEPTH;
            for (const auto &depthRow : dungeon["depths"])
            {
                const int depth = depthRow.value("depth", fallbackDepth);
                int minLevel = Config::Progress::LEVEL_RANGE_MIN_FALLBACK;
                if (depthRow.contains("level_range") && depthRow["level_range"].is_object())
                    minLevel = depthRow["level_range"].value("min", Config::Progress::LEVEL_RANGE_MIN_FALLBACK);

                if (player.level >= minLevel)
                    levelUnlockedDepth = max(levelUnlockedDepth, depth);
                ++fallbackDepth;
            }
        }

        progress.unlocked_depth = clampInt(max(progress.unlocked_depth, levelUnlockedDepth), Config::Progress::START_DEPTH, maxDepth);
        progress.highest_cleared_depth = clampInt(progress.highest_cleared_depth, Config::Progress::NO_DEPTH_CLEARED, maxDepth);
        if (progress.highest_cleared_depth >= maxDepth)
            progress.completed = true;

        if (player.progress.current_dungeon == dungeonId)
            player.progress.max_depth_unlocked = progress.unlocked_depth;

        return progress.unlocked_depth;
    }

    bool isDungeonCompleted(Player &player, const json &dungeon)
    {
        DungeonProgressEntry &progress = ensureDungeonProgress(player, dungeon);
        return progress.completed;
    }

    int dungeonMaxDepth(const json &dungeon)
    {
        if (!dungeon.contains("depths") || !dungeon["depths"].is_array())
            return Config::Progress::START_DEPTH;
        return static_cast<int>(dungeon["depths"].size());
    }

    struct DungeonGraphEdge
    {
        std::string target_id;
        DungeonGraphEdge *next = nullptr;
    };

    struct DungeonGraphVertex
    {
        std::string id;
        bool completed = false;
        bool is_final = false;
        bool visited = false;
        DungeonGraphEdge *edges = nullptr;
        DungeonGraphVertex *next = nullptr;
    };

    DungeonGraphVertex *findDungeonVertex(DungeonGraphVertex *head, const string &id)
    {
        for (DungeonGraphVertex *current = head; current != nullptr; current = current->next)
        {
            if (current->id == id)
                return current;
        }
        return nullptr;
    }

    void addDungeonVertex(DungeonGraphVertex *&head, const string &id, bool completed, bool isFinal)
    {
        DungeonGraphVertex *node = new DungeonGraphVertex;
        node->id = id;
        node->completed = completed;
        node->is_final = isFinal;
        node->next = head;
        head = node;
    }

    void addDungeonEdge(DungeonGraphVertex *head, const string &fromId, const string &toId)
    {
        DungeonGraphVertex *from = findDungeonVertex(head, fromId);
        if (from == nullptr)
            return;

        DungeonGraphEdge *edge = new DungeonGraphEdge;
        edge->target_id = toId;
        edge->next = from->edges;
        from->edges = edge;
    }

    void dfsDungeonGraph(DungeonGraphVertex *head, DungeonGraphVertex *start)
    {
        if (start == nullptr || start->visited)
            return;

        start->visited = true;
        for (DungeonGraphEdge *edge = start->edges; edge != nullptr; edge = edge->next)
            dfsDungeonGraph(head, findDungeonVertex(head, edge->target_id));
    }

    bool graphHasLockedPrerequisite(DungeonGraphVertex *head)
    {
        for (DungeonGraphVertex *current = head; current != nullptr; current = current->next)
        {
            if (current->visited && !current->is_final && !current->completed)
                return true;
        }
        return false;
    }

    void clearDungeonGraph(DungeonGraphVertex *head)
    {
        while (head != nullptr)
        {
            DungeonGraphVertex *nextVertex = head->next;
            DungeonGraphEdge *edge = head->edges;
            while (edge != nullptr)
            {
                DungeonGraphEdge *nextEdge = edge->next;
                delete edge;
                edge = nextEdge;
            }
            delete head;
            head = nextVertex;
        }
    }

    struct DungeonGraphMatrix
    {
        static constexpr int MAX_NODES = Config::Progress::GRAPH_MAX_NODES;
        std::string ids[MAX_NODES];
        int weights[MAX_NODES][MAX_NODES] = {};
        int count = 0;
    };

    int findDungeonMatrixIndex(const DungeonGraphMatrix &matrix, const string &id)
    {
        for (int i = 0; i < matrix.count; ++i)
        {
            if (matrix.ids[i] == id)
                return i;
        }
        return -1;
    }

    void addDungeonMatrixVertex(DungeonGraphMatrix &matrix, const string &id)
    {
        if (matrix.count >= DungeonGraphMatrix::MAX_NODES)
            return;
        if (findDungeonMatrixIndex(matrix, id) >= 0)
            return;
        matrix.ids[matrix.count++] = id;
    }

    void addDungeonMatrixEdge(DungeonGraphMatrix &matrix, const string &fromId, const string &toId, int weight, bool directed)
    {
        const int from = findDungeonMatrixIndex(matrix, fromId);
        const int to = findDungeonMatrixIndex(matrix, toId);
        if (from < 0 || to < 0)
            return;

        matrix.weights[from][to] = max(Config::Progress::MIN_ROUTE_WEIGHT, weight);
        if (!directed)
            matrix.weights[to][from] = max(Config::Progress::MIN_ROUTE_WEIGHT, weight);
    }

    void buildDungeonGraphMatrix(GameContext &ctx, DungeonGraphMatrix &matrix)
    {
        const string finalId = Config::Progress::FINAL_DUNGEON_ID;
        for (const auto &row : ctx.gameData["dungeons"])
            addDungeonMatrixVertex(matrix, row.value("id", string()));

        for (const auto &row : ctx.gameData["dungeons"])
        {
            const string rowId = row.value("id", string());
            if (rowId == finalId)
                continue;

            const int routeWeight = dungeonMaxDepth(row);
            addDungeonMatrixEdge(matrix, rowId, finalId, routeWeight, false);
        }
    }

    int dijkstraDungeonRoute(const DungeonGraphMatrix &matrix, const string &startId, const string &targetId)
    {
        constexpr int INF = Config::Progress::ROUTE_INFINITY;
        int distance[DungeonGraphMatrix::MAX_NODES];
        bool visited[DungeonGraphMatrix::MAX_NODES] = {};

        for (int i = 0; i < DungeonGraphMatrix::MAX_NODES; ++i)
            distance[i] = INF;

        const int start = findDungeonMatrixIndex(matrix, startId);
        const int target = findDungeonMatrixIndex(matrix, targetId);
        if (start < 0 || target < 0)
            return INF;

        distance[start] = 0;
        for (int step = 0; step < matrix.count; ++step)
        {
            int current = -1;
            for (int i = 0; i < matrix.count; ++i)
            {
                if (!visited[i] && (current < 0 || distance[i] < distance[current]))
                    current = i;
            }

            if (current < 0 || distance[current] == INF)
                break;

            visited[current] = true;
            for (int next = 0; next < matrix.count; ++next)
            {
                const int weight = matrix.weights[current][next];
                if (weight > 0 && distance[current] + weight < distance[next])
                    distance[next] = distance[current] + weight;
            }
        }

        return distance[target];
    }

    struct MatrixEdge
    {
        int from = 0;
        int to = 0;
        int weight = 0;
    };

    int findParent(int parent[], int node)
    {
        while (parent[node] != node)
            node = parent[node];
        return node;
    }

    int kruskalDungeonRouteCost(const DungeonGraphMatrix &matrix)
    {
        MatrixEdge edges[DungeonGraphMatrix::MAX_NODES * DungeonGraphMatrix::MAX_NODES];
        int edgeCount = 0;

        for (int row = 0; row < matrix.count; ++row)
        {
            for (int col = row + 1; col < matrix.count; ++col)
            {
                if (matrix.weights[row][col] <= 0)
                    continue;
                edges[edgeCount++] = {row, col, matrix.weights[row][col]};
            }
        }

        for (int i = 0; i < edgeCount; ++i)
        {
            for (int j = 0; j + 1 < edgeCount - i; ++j)
            {
                if (edges[j + 1].weight < edges[j].weight)
                {
                    MatrixEdge temp = edges[j];
                    edges[j] = edges[j + 1];
                    edges[j + 1] = temp;
                }
            }
        }

        int parent[DungeonGraphMatrix::MAX_NODES];
        for (int i = 0; i < DungeonGraphMatrix::MAX_NODES; ++i)
            parent[i] = i;

        int totalCost = 0;
        int usedEdges = 0;
        for (int i = 0; i < edgeCount && usedEdges < matrix.count - 1; ++i)
        {
            const int fromRoot = findParent(parent, edges[i].from);
            const int toRoot = findParent(parent, edges[i].to);
            if (fromRoot == toRoot)
                continue;

            parent[toRoot] = fromRoot;
            totalCost += edges[i].weight;
            ++usedEdges;
        }

        return totalCost;
    }

    bool isFinalDungeonLocked(GameContext &ctx, const json &dungeon)
    {
        if (dungeon.value("id", string()) != Config::Progress::FINAL_DUNGEON_ID)
            return false;
        if (!ctx.gameData.contains("dungeons") || !ctx.gameData["dungeons"].is_array())
            return false;
        if (ctx.gameData["dungeons"].empty())
            return false;

        DungeonGraphVertex *graph = nullptr;
        const string finalId = Config::Progress::FINAL_DUNGEON_ID;

        for (const auto &row : ctx.gameData["dungeons"])
            getDungeonUnlockedDepth(ctx.player, row);

        for (const auto &row : ctx.gameData["dungeons"])
        {
            const string rowId = row.value("id", string());
            addDungeonVertex(graph, rowId, isDungeonCompleted(ctx.player, row), rowId == finalId);
        }

        for (const auto &row : ctx.gameData["dungeons"])
        {
            const string rowId = row.value("id", string());
            if (rowId == finalId)
                continue;
            addDungeonEdge(graph, finalId, rowId);
            addDungeonEdge(graph, rowId, finalId);
        }

        DungeonGraphVertex *finalVertex = findDungeonVertex(graph, finalId);
        dfsDungeonGraph(graph, finalVertex);
        const bool locked = graphHasLockedPrerequisite(graph);
        clearDungeonGraph(graph);

        DungeonGraphMatrix matrix;
        buildDungeonGraphMatrix(ctx, matrix);
        const string startId = ctx.player.progress.current_dungeon.empty()
                                   ? ctx.gameData["dungeons"].front().value("id", string())
                                   : ctx.player.progress.current_dungeon;
        const int shortestRouteCost = dijkstraDungeonRoute(matrix, startId, finalId);
        const int stableRouteCost = kruskalDungeonRouteCost(matrix);
        (void)shortestRouteCost;
        (void)stableRouteCost;
        return locked;
    }
} // namespace state_helpers

// ---- merged from src/states/helpers/player_stats_helpers.cpp

using namespace std;

namespace state_helpers
{
    int calculatePlayerAttack(const GameContext &ctx, const BattleState *battle)
    {
        const string primary = classPrimaryStat(ctx, ctx.player.class_id);
        const int primaryValue = getPrimaryStatValue(ctx.player, primary);

        int attack = ctx.player.level * player_balance::kAttackBasePerLevel;
        attack += primaryValue / player_balance::kAttackPrimaryDivisor;
        attack += ctx.player.stats.str / player_balance::kAttackStrDivisor;
        attack += ctx.player.stats.agi / player_balance::kAttackAgiDivisor;
        attack += static_cast<int>(itemBonusFromEquipped(ctx, ctx.player, "atk") * player_balance::kAttackEquipmentMultiplier);

        if (battle && battle->playerAtkBuffTurns > 0)
            attack = static_cast<int>(attack * player_balance::kAttackBuffMultiplier);
        return max(player_balance::kMinAttack, attack);
    }

    int calculatePlayerDefense(const GameContext &ctx, const BattleState *battle)
    {
        int defense = ctx.player.level * player_balance::kDefenseBasePerLevel;
        defense += ctx.player.stats.vit / player_balance::kDefenseVitDivisor;
        defense += ctx.player.stats.str / player_balance::kDefenseStrDivisor;
        defense += static_cast<int>(itemBonusFromEquipped(ctx, ctx.player, "def") * player_balance::kDefenseEquipmentMultiplier);

        if (battle && battle->playerDefBuffTurns > 0)
            defense = static_cast<int>(defense * player_balance::kDefenseBuffMultiplier);

        defense = max(player_balance::kMinDefense, defense);

        // Clamp supaya damage output musuh/pemain tidak menjadi 1 saat DEF terlalu tinggi.
        if (defense > player_balance::kMaxEffectiveDefenseForDamage)
            defense = player_balance::kMaxEffectiveDefenseForDamage;

        return defense;
    }

    int calculatePlayerSpeed(const GameContext &ctx, const BattleState *battle)
    {
        int speed = ctx.player.level * player_balance::kSpeedBasePerLevel +
                    ctx.player.stats.agi * player_balance::kSpeedPerAgi;
        if (battle && battle->playerAgiBuffTurns > 0)
            speed = static_cast<int>(speed * player_balance::kSpeedBuffMultiplier);
        return max(player_balance::kMinSpeed, speed);
    }

    double calculatePlayerCritRate(const GameContext &ctx, const BattleState *battle)
    {
        double rate = player_balance::kCritBaseRate + (ctx.player.stats.agi * player_balance::kCritPerAgi);
        if (battle && battle->nextAttackGuaranteedCrit)
            rate = player_balance::kCritCap;
        return min(player_balance::kCritCap, rate);
    }

    void refreshPlayerResources(GameContext &ctx)
    {
        normalizePlayerResources(ctx);
    }

    void levelUpIfNeeded(GameContext &ctx)
    {
        Player tierBefore = ctx.player;
        syncPlayerClassTier(ctx, tierBefore);
        const string previousTierId = tierBefore.class_tier_id;
        bool leveled = false;
        while (ctx.player.current_exp >= expRequiredForNextLevel(ctx, ctx.player.level + 1))
        {
            ++ctx.player.level;
            ctx.player.stat_points += STAT_POINTS_PER_LEVEL;
            leveled = true;
            cout << "Level up! Sekarang level kamu " << ctx.player.level
                 << " dan mendapat " << STAT_POINTS_PER_LEVEL << " stat points.\n";
        }

        if (leveled)
        {
            syncPlayerClassTier(ctx, ctx.player);
            if (!previousTierId.empty() && previousTierId != ctx.player.class_tier_id)
                cout << "Tier class terbuka: " << ctx.player.class_tier_name << ".\n";
            refreshPlayerResources(ctx);
            ctx.player.hp = ctx.player.max_hp;
            ctx.player.mp = ctx.player.max_mp;
        }
    }
} // namespace state_helpers

// ---- merged from src/states/helpers/ui_helpers.cpp

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void printHubHeader(const GameContext &ctx)
    {
        const string tierName = unlockedTierName(ctx);
        printLine('=', 64);
        cout << colorText("Character", Color::Yellow, true) << ": " << ctx.player.name
             << " | Class: " << ctx.player.class_id
             << " | Tier: " << tierName
             << " | Level: " << ctx.player.level << "\033[0m\n";
        cout << colorText("Gold", Color::Yellow, true) << ": " << ctx.player.gold
             << " | EXP: " << ctx.player.current_exp << " / "
             << expRequiredForNextLevel(ctx, ctx.player.level + 1) << "\033[0m\n";
        cout << colorText("HP", Color::Green, true) << ": " << ctx.player.hp << "/" << ctx.player.max_hp
             << " | " << colorText("MP", Color::Cyan, true) << ": " << ctx.player.mp << "/" << ctx.player.max_mp << "\033[0m\n";
        cout << "STR: " << ctx.player.stats.str << " | INT: " << ctx.player.stats.intl
             << " | AGI: " << ctx.player.stats.agi << " | VIT: " << ctx.player.stats.vit
             << " | Stat Points: " << ctx.player.stat_points << "\033[0m\n";
        cout << "ATK: " << calculatePlayerAttack(ctx)
             << " | DEF: " << calculatePlayerDefense(ctx)
             << " | Speed: " << calculatePlayerSpeed(ctx) << "\033[0m\n";
        printLine('=', 64);
    }

    void printStateHeader(const GameContext &ctx, const string &title)
    {
        printHubHeader(ctx);
        printTitleBox(title);
    }

    void showItemDetails(const GameContext &ctx, const json &item, int ownedCount, int equippedCount)
    {
        const string itemId = item.value("id", string("-"));
        const string specialEffect = item.value("special_effect", string("null"));

        cout << colorText(item.value("name", itemId), Color::Yellow, true) << '\n';
        cout << colorText("ID", Color::Cyan, true) << "          : " << itemId << '\n';
        cout << colorText("Category", Color::Cyan, true) << "    : " << item.value("category", string("-")) << '\n';
        cout << colorText("Description", Color::Cyan, true) << " : " << item.value("description", string("-")) << '\n';
        cout << colorText("Tier", Color::Cyan, true) << "        : " << item.value("tier_level", 0) << '\n';
        cout << colorText("Required", Color::Cyan, true) << "    : " << item.value("required_class_id", string("-")) << '\n';
        cout << colorText("Buy / Sell", Color::Cyan, true) << "  : " << item.value("buy_price", 0)
             << " / " << item.value("sell_price", 0) << '\n';

        if (ownedCount >= 0)
            cout << colorText("Owned", Color::Cyan, true) << "       : " << ownedCount << '\n';
        if (equippedCount >= 0)
            cout << colorText("Equipped", Color::Cyan, true) << "    : " << equippedCount << '\n';

        cout << colorText("Stats", Color::Cyan, true) << "       : "
             << "ATK +" << item["stats"].value("atk_bonus", 0)
             << " | DEF +" << item["stats"].value("def_bonus", 0)
             << " | HP +" << item["stats"].value("hp_bonus", 0)
             << " | MP +" << item["stats"].value("mp_bonus", 0) << '\n';

        if (!specialEffect.empty() && specialEffect != "null")
            cout << colorText("Effect", Color::Cyan, true) << "      : " << specialEffect << '\n';

        if (item.contains("crafting") && item["crafting"].contains("ingredients"))
        {
            cout << colorText("Crafting", Color::Cyan, true) << "    :\n";
            for (const auto &ingredient : item["crafting"]["ingredients"])
            {
                const string ingredientId = ingredient.value("item_id", string("-"));
                const int need = ingredient.value("quantity", 0);
                const int own = inventoryCount(ctx.player, ingredientId);
                cout << "  - " << itemDisplayName(ctx, ingredientId) << " : " << own << "/" << need << '\n';
            }
        }

        printLine('=');
    }

    string unlockedTierName(const GameContext &ctx)
    {
        Player tierView = ctx.player;
        syncPlayerClassTier(ctx, tierView);
        return tierView.class_tier_name.empty() ? "-" : tierView.class_tier_name;
    }

    void printClassDetails(const GameContext &ctx)
    {
        const auto *cls = getClassById(ctx, ctx.player.class_id);
        if (!cls)
            return;

        Player tierView = ctx.player;
        syncPlayerClassTier(ctx, tierView);
        cout << "Current Tier: " << tierView.class_tier_name
             << " (" << tierView.class_tier_id << ")\n";
        if (cls->contains("base_damage_formula"))
            cout << "Base Formula: " << (*cls)["base_damage_formula"].get<string>() << '\n';

        if (cls->contains("tiers") && (*cls)["tiers"].is_array())
        {
            cout << "Tier Progress:\n";
            for (const auto &tier : (*cls)["tiers"])
            {
                const bool unlocked = ctx.player.level >= tier.value("level", Config::Progress::LEVEL_RANGE_MAX_FALLBACK);
                cout << " - Lv " << tier.value("level", 0) << " -> " << tier.value("name", string())
                     << (unlocked ? " [Unlocked]" : "") << '\n';
            }
        }
    }

    string itemDisplayName(const GameContext &ctx, const string &itemId)
    {
        const auto *item = getItemById(ctx, itemId);
        if (!item)
            return itemId;
        return item->value("name", itemId);
    }

    void showCharacterOverview(const GameContext &ctx)
    {
        printStateHeader(ctx, "CHARACTER MENU");
        printClassDetails(ctx);
        cout << "Equipment Bonus -> ATK: " << itemBonusFromEquipped(ctx, ctx.player, "atk")
             << ", DEF: " << itemBonusFromEquipped(ctx, ctx.player, "def")
             << ", HP: " << itemBonusFromEquipped(ctx, ctx.player, "hp")
             << ", MP: " << itemBonusFromEquipped(ctx, ctx.player, "mp") << '\n';
        cout << "Stat Growth Preview:\n";
        cout << " - STR +1 => sekitar +0 sampai +1 ATK dan +1 Max HP\n";
        cout << " - INT +1 => menaikkan damage skill dan sekitar +3 Max MP\n";
        cout << " - AGI +1 => sekitar +2 Speed dan +0.2% Crit\n";
        cout << " - VIT +1 => sekitar +0 sampai +1 DEF dan +4 Max HP\n";
        cout << MENU_SEPARATOR << '\n';
    }
} // namespace state_helpers

// ---- merged from src/states/main_menu_state.cpp

using namespace std;
using namespace consoleui;
using namespace state_helpers;

// Main menu dibuat sesederhana mungkin supaya alurnya mudah diikuti:
// tampilkan menu, baca input, lalu pindahkan state jika valid.
void runMainMenu(GameContext &ctx)
{
    clearScreen();
    printLine('=');
    cout << colorText("              VOID BRAVER               ", Color::Cyan, true) << '\n';
    printLine('=');
    cout << "1. New Game\n";
    cout << "2. Load Game\n";
    cout << "3. Exit\n";
    printLine('=');
    cout << "Choose: ";

    string choice = readLine();

    if (choice == "1")
    {
        loadSave(ctx);

        cout << "Masukkan nama character: ";
        string name = readLine();
        if (name.empty())
            name = Config::Defaults::PLAYER_NAME;

        cout << MENU_SEPARATOR << '\n';
        cout << "Pilih class:\n";
        for (size_t i = 0; i < ctx.gameData["classes"].size(); ++i)
        {
            const auto &job = ctx.gameData["classes"][i];
            cout << (i + 1) << ". " << job.value("name", string())
                 << " | Primary Stat: " << job.value("primary_stat", string(Config::Defaults::PRIMARY_STAT)) << '\n';
        }
        cout << "Nomor class: ";

        string classChoice = readLine();
        int classIndex = Config::Math::ZERO;
        if (!tryParseInt(classChoice, classIndex) || classIndex < Config::Math::ONE || classIndex > static_cast<int>(ctx.gameData["classes"].size()))
        {
            cout << "Class tidak valid. Default ke Knight.\n";
            classIndex = Config::Math::ONE;
        }

        string classId = ctx.gameData["classes"][static_cast<size_t>(classIndex - Config::Math::INDEX_OFFSET)].value("id", string(Config::Defaults::CLASS_ID));
        createNewGame(ctx, name, classId);
        refreshPlayerResources(ctx);

        string newCharacterId = ctx.player.id;
        if (saveGame(ctx))
            loadSave(ctx);

        for (int i = 0; i < static_cast<int>(ctx.characters.size()); ++i)
        {
            if (ctx.characters[static_cast<size_t>(i)].id == newCharacterId)
            {
                loadCharacter(ctx, i);
                break;
            }
        }

        ctx.stateStack.clear();
        ctx.stateStack.push_back(GameState::Hub);
        return;
    }

    if (choice == "2")
    {
        if (!loadSave(ctx) || ctx.characters.empty())
        {
            cout << "Belum ada character yang tersimpan.\n";
            waitForEnter();
            return;
        }

        while (true)
        {
            clearScreen();
            cout << MENU_SEPARATOR << '\n';
            cout << colorText("LOAD GAME", Color::Yellow, true) << '\n';
            cout << MENU_SEPARATOR << '\n';

            for (size_t i = 0; i < ctx.characters.size(); ++i)
            {
                const auto &character = ctx.characters[i];
                cout << (i + 1) << ". " << character.name
                     << " | Class: " << character.class_id
                     << " | Level: " << character.level
                     << " | Gold: " << character.gold << '\n';
            }

            cout << "Pilih nomor character atau c untuk kembali: ";
            string input = toLower(readLine());
            if (input == "c")
                return;

            int selected = 0;
            if (!tryParseInt(input, selected) || selected < 1 || selected > static_cast<int>(ctx.characters.size()))
            {
                cout << "Pilihan character tidak valid.\n";
                continue;
            }

            loadCharacter(ctx, selected - 1);
            ctx.stateStack.clear();
            ctx.stateStack.push_back(GameState::Hub);
            return;
        }
    }

    if (choice == "3")
    {
        ctx.running = false;
        return;
    }

    cout << "Pilihan tidak valid.\n";
    waitForEnter();
}

// ---- merged from src/states/hub_state.cpp

using namespace std;
using namespace consoleui;
using namespace state_helpers;

// Hub adalah pusat navigasi pemain setelah character aktif masuk game.
void runHub(GameContext &ctx)
{
    clearScreen();
    refreshPlayerResources(ctx);
    printHubHeader(ctx);
    printTitleBox("MAIN HUB");

    cout << colorText("1. Character Menu", Color::Green) << '\n';
    cout << colorText("2. Inventory", Color::Cyan) << '\n';
    cout << colorText("3. Enter Dungeon", Color::Yellow) << '\n';
    cout << colorText("4. Crafting", Color::Blue) << '\n';
    cout << colorText("5. Shop", Color::Magenta) << '\n';
    cout << "6. Save\n";
    cout << "7. Kembali ke Main Menu\n";
    cout << "Choose: ";

    string choice = readLine();
    if (choice == "1")
    {
        characterMenu(ctx);
        return;
    }
    if (choice == "2")
    {
        inventoryMenu(ctx);
        return;
    }
    if (choice == "3")
    {
        chooseDungeon(ctx);
        return;
    }
    if (choice == "4")
    {
        craftingMenu(ctx);
        return;
    }
    if (choice == "5")
    {
        shopMenu(ctx);
        return;
    }
    if (choice == "6")
    {
        if (saveGame(ctx))
            cout << "Game berhasil disimpan.\n";
        waitForEnter();
        return;
    }
    if (choice == "7")
    {
        saveGame(ctx);
        ctx.stateStack.clear();
        ctx.stateStack.push_back(GameState::MainMenu);
        return;
    }

    cout << "Pilihan tidak valid.\n";
    waitForEnter();
}
