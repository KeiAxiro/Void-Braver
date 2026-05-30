#pragma once

#include <cstddef>
#include <string>

#include "globals.h"

// Titik masuk utama untuk setiap state dalam game.
// Stack state di main loop akan memanggil fungsi-fungsi ini.
void runMainMenu(GameContext &ctx);
void runHub(GameContext &ctx);
void runBattle(GameContext &ctx);

namespace state_helpers
{
    struct DropEntry
    {
        std::string item_id;
        int drop_rate = 100;
    };

    using DropList = ManualList<DropEntry>;

    class EncounterQueue
    {
        struct Node
        {
            int step = 0;
            Node *next = nullptr;
            explicit Node(int value) : step(value) {}
        };

        Node *front_ = nullptr;
        Node *rear_ = nullptr;
        int count_ = 0;

    public:
        EncounterQueue() = default;
        EncounterQueue(const EncounterQueue &) = delete;
        EncounterQueue &operator=(const EncounterQueue &) = delete;
        EncounterQueue(EncounterQueue &&other) noexcept
            : front_(other.front_), rear_(other.rear_), count_(other.count_)
        {
            other.front_ = nullptr;
            other.rear_ = nullptr;
            other.count_ = 0;
        }
        EncounterQueue &operator=(EncounterQueue &&other) noexcept
        {
            if (this == &other)
                return *this;
            clear();
            front_ = other.front_;
            rear_ = other.rear_;
            count_ = other.count_;
            other.front_ = nullptr;
            other.rear_ = nullptr;
            other.count_ = 0;
            return *this;
        }
        ~EncounterQueue() { clear(); }

        bool empty() const { return front_ == nullptr; }
        int size() const { return count_; }

        bool contains(int value) const
        {
            for (Node *current = front_; current != nullptr; current = current->next)
            {
                if (current->step == value)
                    return true;
            }
            return false;
        }

        void enqueue(int value)
        {
            Node *node = new Node(value);
            if (rear_ != nullptr)
                rear_->next = node;
            else
                front_ = node;
            rear_ = node;
            ++count_;
        }

        bool dequeue(int &value)
        {
            if (front_ == nullptr)
                return false;
            Node *old = front_;
            value = old->step;
            front_ = front_->next;
            if (front_ == nullptr)
                rear_ = nullptr;
            delete old;
            --count_;
            return true;
        }

        void clear()
        {
            int ignored = 0;
            while (dequeue(ignored))
            {
            }
        }
    };

    struct SkillTreeNode
    {
        const json *skill = nullptr;
        int list_index = 0;
        int unlock_level = 1;
        SkillTreeNode *left = nullptr;
        SkillTreeNode *right = nullptr;
    };

    struct EnemyInstance
    {
        std::string id;
        std::string name;
        std::string type;
        int level = 1;
        int hp = 1;
        int max_hp = 1;
        int mp = 0;
        int max_mp = 0;
        int atk = 1;
        int def = 0;
        double crit_rate = 0.05;
        bool is_boss = false;
        int exp_drop = 10;
        DropList drops;
    };

    struct BattleState
    {
        int playerShield = 0;
        int playerAtkBuffTurns = 0;
        int playerDefBuffTurns = 0;
        int playerAgiBuffTurns = 0;
        int playerInvincibleTurns = 0;
        int playerEvadeTurns = 0;
        bool nextAttackGuaranteedCrit = false;
        int enemyAtkDebuffTurns = 0;
        int enemyDefDebuffTurns = 0;
        int enemyStunTurns = 0;
        int enemySilenceTurns = 0;
        int enemyDotTurns = 0;
        int enemyDotDamage = 0;
        std::string enemyDotLabel;
    };

    enum class EncounterResult
    {
        Victory,
        Retreat,
        Defeat
    };

    int randInt(int minValue, int maxValue);
    double randUnit();
    std::string trim(const std::string &value);
    std::string toLower(std::string value);
    std::string readLine();
    bool tryParseInt(const std::string &text, int &out);
    int getPrimaryStatValue(const Player &player, const std::string &primary);

    int inventoryCount(const Player &player, const std::string &itemId);
    void mergeInventory(Player &player);
    void addItem(Player &player, const std::string &itemId, int quantity, bool equipped = false, const std::string &slot = "");
    bool removeItem(Player &player, const std::string &itemId, int quantity);
    int itemBonusFromEquipped(const GameContext &ctx, const Player &player, const std::string &category);
    int getCooldown(const Player &player, const std::string &skillId);
    void setCooldown(Player &player, const std::string &skillId, int turns);
    void reduceCooldowns(Player &player);

    double normalizedRatio(int value, int minValue, int maxValue);
    int interpolateStat(int level, int minLevel, int maxLevel, int minValue, int maxValue);

    DungeonProgressEntry &ensureDungeonProgress(Player &player, const json &dungeon);
    int getDungeonUnlockedDepth(Player &player, const json &dungeon);
    bool isDungeonCompleted(Player &player, const json &dungeon);
    int dungeonMaxDepth(const json &dungeon);
    bool isFinalDungeonLocked(GameContext &ctx, const json &dungeon);

    int calculatePlayerAttack(const GameContext &ctx, const BattleState *battle = nullptr);
    int calculatePlayerDefense(const GameContext &ctx, const BattleState *battle = nullptr);
    int calculatePlayerSpeed(const GameContext &ctx, const BattleState *battle = nullptr);
    double calculatePlayerCritRate(const GameContext &ctx, const BattleState *battle = nullptr);
    void refreshPlayerResources(GameContext &ctx);
    void levelUpIfNeeded(GameContext &ctx);

    void printHubHeader(const GameContext &ctx);
    void printStateHeader(const GameContext &ctx, const std::string &title);
    void showItemDetails(const GameContext &ctx, const json &item, int ownedCount, int equippedCount);
    std::string unlockedTierName(const GameContext &ctx);
    void printClassDetails(const GameContext &ctx);
    std::string itemDisplayName(const GameContext &ctx, const std::string &itemId);
    void showCharacterOverview(const GameContext &ctx);

    void allocateStats(GameContext &ctx);
    bool equipInventoryEntry(GameContext &ctx, std::size_t index);
    bool unequipInventoryEntry(GameContext &ctx, std::size_t index);
    void useConsumable(GameContext &ctx, std::size_t index);
    void inventoryMenu(GameContext &ctx);
    void skillMenu(const GameContext &ctx);
    void characterMenu(GameContext &ctx);
    int skillUnlockLevelForIndex(int listIndex);
    bool isSkillUnlockedByLevel(const Player &player, int listIndex);
    SkillTreeNode *buildSkillTree(const json &skills, int leftIndex, int rightIndex);
    const json *findSkillInTree(SkillTreeNode *root, int listIndex);
    void clearSkillTree(SkillTreeNode *root);
    void printSkillTreeInOrder(const Player &player, SkillTreeNode *root);

    EnemyInstance spawnEnemyForDepth(const GameContext &ctx, const json &dungeon, int depth);
    bool isDepthLevelAllowed(const json &depthRow, int level);
    void printDungeonLore(const json &depthRow);
    bool selectDungeonDepth(GameContext &ctx, bool enterBattleState);
    void chooseDungeon(GameContext &ctx);

    bool canCraftItem(const GameContext &ctx, const json &item);
    int ownedIngredientCount(const GameContext &ctx, const json &item);
    int totalIngredientNeed(const json &item);
    StringList collectCraftingCategories(const GameContext &ctx);
    StringList categoriesForShopMode(const GameContext &ctx, const std::string &mode);
    void showRecipeDetail(const GameContext &ctx, const json &item);
    void craftingMenu(GameContext &ctx);
    void shopMenu(GameContext &ctx);

    int calculateEnemySpeed(const EnemyInstance &enemy);
    bool enemyGetsFirstTurn(const GameContext &ctx, const EnemyInstance &enemy, bool ambush);
    EncounterQueue buildEncounterStepsForMove();
    EncounterResult runEncounterBattle(GameContext &ctx, const json &dungeon, const json &depthData, EnemyInstance enemy, bool enemyStartsFirst);
    void grantEnemyDrops(GameContext &ctx, const EnemyInstance &enemy);
    void advanceDungeonProgress(GameContext &ctx, const json &dungeon, int clearedDepth);
    int calculateEnemyDamage(const EnemyInstance &enemy, const BattleState &battle);
    int calculateEnemyDefense(const EnemyInstance &enemy, const BattleState &battle);
    void tickBattleEffects(GameContext &ctx, EnemyInstance &enemy, BattleState &battle);
    bool useBattleConsumable(GameContext &ctx);
    void resolveSkillUse(GameContext &ctx, EnemyInstance &enemy, BattleState &battle, const json &skill, bool &turnConsumed);
    void showBattleSkills(const GameContext &ctx);
    void applyBattleRewards(GameContext &ctx, const json &dungeon, const json &depthRow, const EnemyInstance &enemy);
    void applyDepthCompletionRewards(GameContext &ctx, const json &dungeon, const json &depthRow);
}
