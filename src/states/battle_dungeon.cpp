#include "states.h"
#include "database.h"
#include "config.h"
#include "utils/console_ui.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>


// ---- merged from src/states/math_helpers.cpp


namespace state_helpers
{
    double normalizedRatio(int value, int minValue, int maxValue)
    {
        if (maxValue <= minValue)
            return 0.0;

        return static_cast<double>(value - minValue) /
               static_cast<double>(maxValue - minValue);
    }

    int interpolateStat(int level, int levelMin, int levelMax, int statMin, int statMax)
    {
        const double ratio = normalizedRatio(level, levelMin, levelMax);
        const double value = statMin + (statMax - statMin) * ratio;
        return std::max(1, static_cast<int>(std::round(value)));
    }
} // namespace state_helpers

// ---- merged from src/states/dungeon_menu.cpp



using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void printDungeonLore(const json &depthRow)
    {
        if (!depthRow.contains("tale") || !depthRow["tale"].is_object())
            return;

        const auto &tale = depthRow["tale"];
        cout << colorText("Chapter", Color::Magenta, true) << ": " << tale.value("chapter_name", std::string("-")) << '\n';
        cout << colorText("Title", Color::Cyan, true) << ": " << tale.value("depth_title", std::string("-")) << '\n';
        cout << colorText("Lore", Color::Yellow, true) << ": " << tale.value("lore_content", std::string("-")) << '\n';
    }

    bool selectDungeonDepth(GameContext &ctx, bool enterBattleState)
    {
        while (true)
        {
            clearScreen();
            printStateHeader(ctx, "ENTER DUNGEON");

            int index = 1;
            for (const auto &dungeon : ctx.gameData["dungeons"])
            {
                const bool locked = isFinalDungeonLocked(ctx, dungeon);
                const int unlockedDepth = getDungeonUnlockedDepth(ctx.player, dungeon);
                const int maxDepth = dungeonMaxDepth(dungeon);

                cout << colorText(to_string(index++) + ". " + dungeon.value("name", std::string()), Color::Yellow, true) << '\n';
                cout << "   Theme : " << dungeon.value("theme", std::string("-")) << '\n';
                cout << "   Boss  : " << dungeon.value("boss_name", std::string("-"))
                     << " | Orb: " << dungeon.value("reward_orb", std::string("-")) << '\n';
                cout << "   Depth : " << unlockedDepth << "/" << maxDepth;
                if (isDungeonCompleted(ctx.player, dungeon))
                    cout << " [Completed]";
                if (locked)
                    cout << " [Locked]";
                cout << "\n\n";
            }

            cout << "Pilih dungeon dengan nomor, atau c untuk kembali: ";
            const string input = toLower(readLine());
            if (input == "c")
                return false;

            int selected = 0;
            if (!tryParseInt(input, selected) || selected < 1 || selected > static_cast<int>(ctx.gameData["dungeons"].size()))
            {
                cout << "Pilihan dungeon tidak valid.\n";
                continue;
            }

            const json &dungeon = ctx.gameData["dungeons"][static_cast<size_t>(selected - 1)];
            if (isFinalDungeonLocked(ctx, dungeon))
            {
                cout << "Tahta Kehampaan masih terkunci. Selesaikan dungeon lain dulu.\n";
                waitForEnter();
                continue;
            }

            const int unlockedDepth = getDungeonUnlockedDepth(ctx.player, dungeon);
            const int maxDepth = dungeonMaxDepth(dungeon);

            clearScreen();
            printStateHeader(ctx, dungeon.value("name", std::string()));
            cout << "Pilih depth 1 sampai " << unlockedDepth << ".\n\n";

            for (int depth = 1; depth <= unlockedDepth && depth <= maxDepth; ++depth)
            {
                const json *depthRow = findDungeonDepth(dungeon, depth);
                if (!depthRow)
                    continue;

                const int minLevel = (*depthRow)["level_range"].value("min", 1);
                const int maxLevelRow = (*depthRow)["level_range"].value("max", 999);

                cout << colorText(to_string(depth) + ". Depth " + to_string(depth), Color::Cyan, true)
                     << " | Recommended Lv " << minLevel << "-" << maxLevelRow;
                if (depth == maxDepth)
                    cout << " [Boss]";
                cout << '\n';

                printDungeonLore(*depthRow);
                cout << '\n';
            }

            cout << "Depth pilihan, atau c untuk cancel: ";
            const string depthInput = toLower(readLine());
            if (depthInput == "c")
                continue;

            int depthChoice = 0;
            if (!tryParseInt(depthInput, depthChoice) || depthChoice < 1 || depthChoice > unlockedDepth || depthChoice > maxDepth)
            {
                cout << "Depth tidak valid.\n";
                continue;
            }

            const json *depthRow = findDungeonDepth(dungeon, depthChoice);
            if (!depthRow)
            {
                cout << "Data depth tidak ditemukan.\n";
                waitForEnter();
                continue;
            }

            if (!isDepthLevelAllowed(*depthRow, ctx.player.level))
            {
                cout << "Level character belum mencapai minimum depth ini.\n";
                waitForEnter();
                continue;
            }

            ctx.player.progress.current_dungeon = dungeon.value("id", std::string());
            ctx.player.progress.current_depth = depthChoice;
            ctx.player.progress.max_depth_unlocked = unlockedDepth;

            if (enterBattleState)
                ctx.stateStack.push_back(GameState::Battle);

            return true;
        }
    }

    void chooseDungeon(GameContext &ctx)
    {
        selectDungeonDepth(ctx, true);
    }
} // namespace state_helpers

// ---- merged from src/states/enemy_factory.cpp



using namespace std;

namespace state_helpers
{
    bool isDepthLevelAllowed(const json &depthRow, int level)
    {
        if (!depthRow.contains("level_range") || !depthRow["level_range"].is_object())
            return true;

        const int minLevel = depthRow["level_range"].value("min", 1);
        return level >= minLevel;
    }

    EnemyInstance spawnEnemyForDepth(const GameContext &ctx, const json &dungeon, int depth)
    {
        JsonPointerList pool;
        const bool wantsBoss = depth == dungeonMaxDepth(dungeon);

        if (ctx.gameData.contains("enemies") && ctx.gameData["enemies"].is_array())
        {
            for (const auto &enemy : ctx.gameData["enemies"])
            {
                if (!enemy.contains("spawn_depth") || !enemy["spawn_depth"].is_object())
                    continue;

                const int start = enemy["spawn_depth"].value("start", 1);
                const int end = enemy["spawn_depth"].value("end", 1);
                const bool matchesDepth = depth >= start && depth <= end;
                const bool isBoss = enemy.value("is_boss", false);

                if (matchesDepth && isBoss == wantsBoss)
                    pool.push_back(&enemy);
            }

            if (pool.empty())
            {
                for (const auto &enemy : ctx.gameData["enemies"])
                {
                    if (!enemy.contains("spawn_depth") || !enemy["spawn_depth"].is_object())
                        continue;

                    const int start = enemy["spawn_depth"].value("start", 1);
                    const int end = enemy["spawn_depth"].value("end", 1);
                    if (depth >= start && depth <= end)
                        pool.push_back(&enemy);
                }
            }
        }

        if (pool.empty())
        {
            EnemyInstance fallback;
            fallback.id = "training_slime";
            fallback.name = wantsBoss ? dungeon.value("boss_name", string("Apex Slime")) : "Training Slime";
            fallback.type = "Slime";
            fallback.level = depth;
            fallback.max_hp = fallback.hp = enemy_balance::kFallbackHpBase + depth * enemy_balance::kFallbackHpPerDepth;
            fallback.max_mp = fallback.mp = enemy_balance::kFallbackMpBase + depth * enemy_balance::kFallbackMpPerDepth;
            fallback.atk = enemy_balance::kFallbackAtkBase + depth * enemy_balance::kFallbackAtkPerDepth;
            fallback.def = enemy_balance::kFallbackDefBase + depth * enemy_balance::kFallbackDefPerDepth;
            fallback.crit_rate = 0.05;
            fallback.is_boss = wantsBoss;
            fallback.exp_drop = enemy_balance::kFallbackExpBase + depth * enemy_balance::kFallbackExpPerDepth;
            return fallback;
        }

        const json &templateEnemy = *pool[static_cast<size_t>(randInt(0, static_cast<int>(pool.size()) - 1))];
        EnemyInstance enemy;
        enemy.id = templateEnemy.value("id", string("enemy"));
        enemy.name = templateEnemy.value("name", string("Enemy"));
        enemy.type = templateEnemy.value("type", string("Unknown"));
        enemy.is_boss = wantsBoss || templateEnemy.value("is_boss", false);

        const int enemyLevelMin = templateEnemy["level_range"].value("min", 1);
        const int enemyLevelMax = templateEnemy["level_range"].value("max", enemyLevelMin);

        int levelMin = enemyLevelMin;
        int levelMax = enemyLevelMax;

        const json *depthData = findDungeonDepth(dungeon, depth);
        if (depthData != nullptr && depthData->contains("level_range"))
        {
            levelMin = max(levelMin, (*depthData)["level_range"].value("min", levelMin));
            levelMax = min(levelMax, (*depthData)["level_range"].value("max", levelMax));
            if (levelMin > levelMax)
                levelMax = levelMin;
        }

        enemy.level = randInt(levelMin, levelMax);
        enemy.max_hp = interpolateStat(enemy.level, levelMin, enemyLevelMax,
                                       templateEnemy["stats_range"]["hp"].value("min", 10),
                                       templateEnemy["stats_range"]["hp"].value("max", 20));
        enemy.max_mp = interpolateStat(enemy.level, levelMin, enemyLevelMax,
                                       templateEnemy["stats_range"]["mp"].value("min", 0),
                                       templateEnemy["stats_range"]["mp"].value("max", 0));
        enemy.atk = interpolateStat(enemy.level, levelMin, enemyLevelMax,
                                    templateEnemy["stats_range"]["atk"].value("min", 1),
                                    templateEnemy["stats_range"]["atk"].value("max", 2));
        enemy.def = interpolateStat(enemy.level, levelMin, enemyLevelMax,
                                    templateEnemy["stats_range"]["def"].value("min", 0),
                                    templateEnemy["stats_range"]["def"].value("max", 1));

        if (wantsBoss)
        {
            enemy.name = dungeon.value("boss_name", enemy.name);
            enemy.max_hp = static_cast<int>(enemy.max_hp * enemy_balance::kBossHpMultiplier);
            enemy.atk = static_cast<int>(enemy.atk * enemy_balance::kBossAtkMultiplier);
            enemy.def = static_cast<int>(enemy.def * enemy_balance::kBossDefMultiplier);
            enemy.crit_rate = max(templateEnemy.value("crit_rate", 0.05), enemy_balance::kBossMinimumCritRate);
        }
        else
        {
            enemy.crit_rate = templateEnemy.value("crit_rate", 0.05);
        }

        enemy.hp = enemy.max_hp;
        enemy.mp = enemy.max_mp;
        enemy.exp_drop = enemy.level * enemy_balance::kExpDropPerEnemyLevel +
                         depth * enemy_balance::kExpDropPerDepth +
                         (enemy.is_boss ? enemy_balance::kBossExpBonus : 0);

        if (templateEnemy.contains("drops") && templateEnemy["drops"].is_array())
        {
            for (const auto &drop : templateEnemy["drops"])
                enemy.drops.push_back({drop.value("item_id", string()), drop.value("drop_rate", 100)});
        }

        return enemy;
    }
} // namespace state_helpers

// ---- merged from src/states/battle_helpers.cpp



using namespace std;
using namespace consoleui;

namespace state_helpers
{
    int calculateEnemySpeed(const EnemyInstance &enemy)
    {
        return max(1, enemy.level * enemy_balance::kEnemySpeedPerLevel +
                          (enemy.atk / enemy_balance::kEnemySpeedAttackDivisor));
    }

    bool enemyGetsFirstTurn(const GameContext &ctx, const EnemyInstance &enemy, bool ambush)
    {
        int playerSpeed = calculatePlayerSpeed(ctx);
        int enemySpeed = calculateEnemySpeed(enemy);
        if (ambush)
            enemySpeed += enemy_balance::kEnemyOpeningAmbushSpeedBonus;
        return enemySpeed > playerSpeed;
    }

    EncounterQueue buildEncounterStepsForMove()
    {
        EncounterQueue steps;
        const int encounterCount = randInt(game_rules::kMinEncountersPerMove, game_rules::kMaxEncountersPerMove);
        bool selectedSteps[Config::Rules::MOVE_STEPS_PER_DEPTH + 1] = {};
        int selectedCount = 0;

        while (selectedCount < encounterCount)
        {
            const int step = randInt(1, game_rules::kMoveStepsPerDepth);
            if (!selectedSteps[step])
            {
                selectedSteps[step] = true;
                ++selectedCount;
            }
        }

        for (int step = 1; step <= game_rules::kMoveStepsPerDepth; ++step)
        {
            if (selectedSteps[step])
                steps.enqueue(step);
        }

        return steps;
    }

    EncounterResult runEncounterBattle(GameContext &ctx, const json &dungeon, const json &depthData, EnemyInstance enemy, bool enemyStartsFirst)
    {
        BattleState battle;

        auto showBattlePanel = [&](const string &title, bool showActions) {
            clearScreen();
            printStateHeader(ctx, title);
            cout << colorText("Dungeon", Color::Yellow, true) << " : " << dungeon.value("name", string()) << '\n';
            cout << colorText("Depth", Color::Yellow, true) << "   : " << ctx.player.progress.current_depth << '\n';
            cout << colorText("Enemy", Color::Yellow, true) << "   : " << enemy.name;
            if (enemy.is_boss)
                cout << " [Boss]";
            cout << '\n';
            cout << colorText("Type", Color::Yellow, true) << "    : " << enemy.type
                 << " | Level: " << enemy.level << '\n';
            cout << colorText("Enemy HP", Color::Red, true) << " : " << enemy.hp << "/" << enemy.max_hp
                 << " | DEF: " << calculateEnemyDefense(enemy, battle) << '\n';
            cout << colorText("Your HP", Color::Green, true) << "  : " << ctx.player.hp << "/" << ctx.player.max_hp
                 << " | " << colorText("MP", Color::Cyan, true) << ": " << ctx.player.mp << "/" << ctx.player.max_mp << '\n';
            cout << colorText("ATK / DEF", Color::Cyan, true) << " : "
                 << calculatePlayerAttack(ctx, &battle) << " / " << calculatePlayerDefense(ctx, &battle)
                 << " | Speed: " << calculatePlayerSpeed(ctx, &battle) << '\n';

            if (battle.playerShield > 0)
                cout << colorText("Shield", Color::Blue, true) << "  : " << battle.playerShield << '\n';

            printLine('=');

            if (!showActions)
                return;

            cout << colorText("1. Attack", Color::Green) << '\n';
            cout << colorText("2. Skill", Color::Cyan) << '\n';
            cout << colorText("3. Consumable", Color::Magenta) << '\n';
            cout << colorText("4. Run From Battle", Color::Yellow) << '\n';
            printLine('-');
        };

        auto enemyTurn = [&](bool openingAttack = false) {
            if (battle.enemyStunTurns > 0 && !openingAttack)
            {
                cout << enemy.name << " terkena stun dan gagal bergerak.\n";
                return;
            }

            int enemyDamage = calculateEnemyDamage(enemy, battle) - calculatePlayerDefense(ctx, &battle);
            enemyDamage = max(1, enemyDamage + randInt(enemy_balance::kEnemyDamageJitterMin,
                                                       enemy_balance::kEnemyDamageJitterMax));

            bool criticalHit = randUnit() <= enemy.crit_rate;
            if (criticalHit)
                enemyDamage = static_cast<int>(enemyDamage * enemy_balance::kEnemyCriticalDamageMultiplier);

            if (battle.playerInvincibleTurns > 0)
            {
                cout << "Karaktermu invincible dan menahan semua damage.\n";
                enemyDamage = 0;
            }
            else if (battle.playerEvadeTurns > 0)
            {
                cout << "Karaktermu menghindari serangan musuh.\n";
                enemyDamage = 0;
            }

            if (enemyDamage > 0 && battle.playerShield > 0)
            {
                int absorbedDamage = min(battle.playerShield, enemyDamage);
                battle.playerShield -= absorbedDamage;
                enemyDamage -= absorbedDamage;
                cout << "Shield menyerap " << absorbedDamage << " damage.\n";
            }

            if (enemyDamage > 0)
            {
                ctx.player.hp = max(0, ctx.player.hp - enemyDamage);
                if (openingAttack)
                    cout << colorText("Musuh bergerak lebih dulu!", Color::Red, true) << '\n';
                cout << colorText(enemy.name, Color::Red, true)
                     << " menyerang dan memberi " << enemyDamage << " damage";
                if (criticalHit)
                    cout << " [Critical]";
                cout << ".\n";
            }
        };

        showBattlePanel("BATTLE START", false);
        cout << colorText("Encounter dimulai. Siapkan tindakanmu.", Color::Yellow) << '\n';
        waitForEnter();

        if (enemyStartsFirst)
        {
            showBattlePanel("ENEMY OPENING TURN", false);
            enemyTurn(true);
            waitForEnter();
        }

        while (ctx.player.hp > 0 && enemy.hp > 0)
        {
            showBattlePanel("YOUR TURN", true);
            cout << "Choose: ";

            string choice = readLine();
            bool turnConsumed = false;

            if (choice == "1")
            {
                showBattlePanel("ACTION RESULT", false);

                int damage = calculatePlayerAttack(ctx, &battle) - calculateEnemyDefense(enemy, battle);
                damage = max(1, damage + randInt(player_balance::kAttackJitterMin,
                                                 player_balance::kAttackJitterMax));

                bool criticalHit = randUnit() <= calculatePlayerCritRate(ctx, &battle);
                if (criticalHit)
                    damage = static_cast<int>(damage * skill_balance::kPlayerCriticalDamageMultiplier);

                enemy.hp = max(0, enemy.hp - damage);
                cout << "Kamu menyerang " << enemy.name << " dan memberi " << damage << " damage";
                if (criticalHit)
                    cout << " [Critical]";
                cout << ".\n";

                battle.nextAttackGuaranteedCrit = false;
                turnConsumed = true;
            }
            else if (choice == "2")
            {
                const json *job = getClassById(ctx, ctx.player.class_id);
                if (job == nullptr || !job->contains("skills") || !(*job)["skills"].is_array())
                {
                    cout << "Skill tidak tersedia.\n";
                    waitForEnter();
                    continue;
                }

                showBattleSkills(ctx);
                cout << "Pilih nomor skill atau c untuk batal: ";

                string skillInput = toLower(readLine());
                if (skillInput == "c")
                    continue;

                int skillIndex = 0;
                if (!tryParseInt(skillInput, skillIndex) || skillIndex < 1 || skillIndex > static_cast<int>((*job)["skills"].size()))
                {
                    cout << "Pilihan skill tidak valid.\n";
                    waitForEnter();
                    continue;
                }

                if (!isSkillUnlockedByLevel(ctx.player, skillIndex))
                {
                    cout << "Skill ini baru terbuka pada level " << skillUnlockLevelForIndex(skillIndex) << ".\n";
                    waitForEnter();
                    continue;
                }

                SkillTreeNode *skillRoot = buildSkillTree((*job)["skills"], 1, static_cast<int>((*job)["skills"].size()));
                const json *selectedSkill = findSkillInTree(skillRoot, skillIndex);
                if (selectedSkill == nullptr)
                {
                    clearSkillTree(skillRoot);
                    cout << "Skill tidak ditemukan di tree.\n";
                    waitForEnter();
                    continue;
                }

                showBattlePanel("SKILL RESULT", false);
                resolveSkillUse(ctx, enemy, battle, *selectedSkill, turnConsumed);
                clearSkillTree(skillRoot);
                if (!turnConsumed)
                {
                    waitForEnter();
                    continue;
                }
            }
            else if (choice == "3")
            {
                turnConsumed = useBattleConsumable(ctx);
                if (!turnConsumed)
                    continue;
            }
            else if (choice == "4")
            {
                showBattlePanel("RETREAT", false);
                cout << "Kamu berhasil menjaga jarak dan mundur dari encounter ini.\n";
                waitForEnter();
                return EncounterResult::Retreat;
            }
            else
            {
                cout << "Pilihan tidak valid.\n";
                waitForEnter();
                continue;
            }

            if (enemy.hp <= 0)
                break;

            enemyTurn(false);
            tickBattleEffects(ctx, enemy, battle);
            waitForEnter();
        }

        showBattlePanel(ctx.player.hp <= 0 ? "DEFEAT" : "VICTORY", false);

        if (ctx.player.hp <= 0)
        {
            ctx.player.hp = max(1, ctx.player.max_hp / player_balance::kDefeatRecoveryDivisor);
            ctx.player.mp = max(1, ctx.player.max_mp / player_balance::kDefeatRecoveryDivisor);
            cout << "Kamu kalah. HP dan MP dipulihkan ke sebagian maksimum.\n";
            saveGame(ctx);
            waitForEnter();
            return EncounterResult::Defeat;
        }

        cout << colorText(enemy.name + " dikalahkan!", Color::Green, true) << '\n';
        applyBattleRewards(ctx, dungeon, depthData, enemy);
        waitForEnter();
        return EncounterResult::Victory;
    }
} // namespace state_helpers

// ---- merged from src/states/battle_effects.cpp



using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void grantEnemyDrops(GameContext &ctx, const EnemyInstance &enemy)
    {
        if (enemy.drops.empty())
            return;

        cout << "Drop:\n";
        for (const auto &drop : enemy.drops)
        {
            if (randInt(1, 100) <= drop.drop_rate)
            {
                addItem(ctx.player, drop.item_id, 1, false, "");
                cout << " - " << itemDisplayName(ctx, drop.item_id) << '\n';
            }
        }
    }

    void advanceDungeonProgress(GameContext &ctx, const json &dungeon, int clearedDepth)
    {
        DungeonProgressEntry &progress = ensureDungeonProgress(ctx.player, dungeon);
        const int maxDepth = dungeonMaxDepth(dungeon);
        progress.highest_cleared_depth = max(progress.highest_cleared_depth, clearedDepth);

        if (clearedDepth >= maxDepth)
        {
            progress.unlocked_depth = maxDepth;
            progress.completed = true;
            cout << "Dungeon selesai! Reward orb: " << dungeon.value("reward_orb", std::string("-")) << '\n';
        }
        else
        {
            progress.unlocked_depth = max(progress.unlocked_depth, clearedDepth + 1);
        }

        ctx.player.progress.current_dungeon = dungeon.value("id", std::string());
        ctx.player.progress.current_depth = min(progress.unlocked_depth, maxDepth);
        ctx.player.progress.max_depth_unlocked = progress.unlocked_depth;
    }

    int calculateEnemyDamage(const EnemyInstance &enemy, const BattleState &battle)
    {
        int attack = enemy.atk;
        if (!enemy.is_boss)
            attack = static_cast<int>(attack * enemy_balance::kRegularEnemyAttackMultiplier);
        if (battle.enemyAtkDebuffTurns > 0)
            attack = static_cast<int>(attack * enemy_balance::kEnemyAttackDebuffMultiplier);
        if (enemy.is_boss)
            attack = static_cast<int>(attack * enemy_balance::kBossEnemyAttackMultiplier);
        return max(1, attack);
    }

    int calculateEnemyDefense(const EnemyInstance &enemy, const BattleState &battle)
    {
        int defense = enemy.def;
        if (battle.enemyDefDebuffTurns > 0)
            defense = static_cast<int>(defense * enemy_balance::kEnemyDefenseDebuffMultiplier);
        return max(0, defense);
    }

    void tickBattleEffects(GameContext &ctx, EnemyInstance &enemy, BattleState &battle)
    {
        if (battle.enemyDotTurns > 0)
        {
            enemy.hp = max(0, enemy.hp - battle.enemyDotDamage);
            cout << enemy.name << " terkena " << battle.enemyDotLabel
                 << " dan kehilangan " << battle.enemyDotDamage << " HP.\n";
            --battle.enemyDotTurns;
        }

        if (battle.playerAtkBuffTurns > 0)
            --battle.playerAtkBuffTurns;
        if (battle.playerDefBuffTurns > 0)
            --battle.playerDefBuffTurns;
        if (battle.playerAgiBuffTurns > 0)
            --battle.playerAgiBuffTurns;
        if (battle.playerInvincibleTurns > 0)
            --battle.playerInvincibleTurns;
        if (battle.playerEvadeTurns > 0)
            --battle.playerEvadeTurns;
        if (battle.enemyAtkDebuffTurns > 0)
            --battle.enemyAtkDebuffTurns;
        if (battle.enemyDefDebuffTurns > 0)
            --battle.enemyDefDebuffTurns;
        if (battle.enemySilenceTurns > 0)
            --battle.enemySilenceTurns;
        if (battle.enemyStunTurns > 0)
            --battle.enemyStunTurns;

        battle.nextAttackGuaranteedCrit = false;
        reduceCooldowns(ctx.player);
    }

    void applyBattleRewards(GameContext &ctx, const json &dungeon, const json &depthRow, const EnemyInstance &enemy)
    {
        (void)dungeon;
        (void)depthRow;

        const int expReward = enemy.exp_drop;
        const int goldReward = max(enemy_balance::kBattleGoldMinimum,
                                   enemy.level * enemy_balance::kBattleGoldPerEnemyLevel +
                                       enemy.atk / enemy_balance::kBattleGoldAtkDivisor);

        ctx.player.current_exp += expReward;
        ctx.player.gold += goldReward;

        cout << "Reward: +" << expReward << " EXP, +" << goldReward << " Gold\n";

        grantEnemyDrops(ctx, enemy);
        levelUpIfNeeded(ctx);
        saveGame(ctx);
    }

    void applyDepthCompletionRewards(GameContext &ctx, const json &dungeon, const json &depthRow)
    {
        const int baseDepthExp = depthRow.value("exp_gain", 0);
        const int clearExpReward = max(enemy_balance::kDepthClearExpMinimum,
                                       (baseDepthExp * enemy_balance::kDepthClearExpNumerator) /
                                           enemy_balance::kDepthClearExpDenominator);
        const int clearGoldReward = max(enemy_balance::kDepthClearGoldMinimum,
                                        baseDepthExp / enemy_balance::kDepthClearGoldDivisor);

        ctx.player.current_exp += clearExpReward;
        ctx.player.gold += clearGoldReward;

        cout << "\n";
        printLine('=');
        cout << colorText("DEPTH CLEAR", Color::Green, true) << '\n';
        cout << "Bonus clear: +" << clearExpReward << " EXP, +" << clearGoldReward << " Gold\n";

        advanceDungeonProgress(ctx, dungeon, depthRow.value("depth", 1));
        levelUpIfNeeded(ctx);
        saveGame(ctx);
    }
} // namespace state_helpers

// ---- merged from src/states/battle_skills.cpp



using namespace std;
using namespace consoleui;

namespace state_helpers
{
    bool useBattleConsumable(GameContext &ctx)
    {
        IndexList consumableIndexes;
        for (size_t i = 0; i < ctx.player.inventory.size(); ++i)
        {
            const auto *item = getItemById(ctx, ctx.player.inventory[i].item_id);
            if (item && item->value("category", std::string()) == "consumable")
                consumableIndexes.push_back(i);
        }

        if (consumableIndexes.empty())
        {
            clearScreen();
            printStateHeader(ctx, "CONSUMABLE");
            cout << "Tidak ada consumable di inventory.\n";
            waitForEnter();
            return false;
        }

        clearScreen();
        printStateHeader(ctx, "CONSUMABLE");
        cout << "Consumables:\n";
        for (size_t i = 0; i < consumableIndexes.size(); ++i)
        {
            const auto &entry = ctx.player.inventory[consumableIndexes[i]];
            cout << (i + 1) << ". " << itemDisplayName(ctx, entry.item_id) << " x" << entry.quantity << '\n';
        }
        cout << "Pilih nomor item atau c untuk batal: ";

        const string input = toLower(readLine());
        if (input == "c")
            return false;

        int selected = 0;
        if (!tryParseInt(input, selected) || selected < 1 || selected > static_cast<int>(consumableIndexes.size()))
        {
            cout << "Pilihan tidak valid.\n";
            waitForEnter();
            return false;
        }

        clearScreen();
        printStateHeader(ctx, "ITEM USED");
        useConsumable(ctx, consumableIndexes[static_cast<size_t>(selected - 1)]);
        return true;
    }

    void resolveSkillUse(GameContext &ctx, EnemyInstance &enemy, BattleState &battle, const json &skill, bool &turnConsumed)
    {
        const string skillId = skill.value("id", std::string());
        const string skillName = skill.value("name", skillId);
        const int manaCost = skill.value("mana_cost", 0);
        const int cooldown = skill.value("cooldown_turns", 0);
        const int currentCooldown = getCooldown(ctx.player, skillId);

        if (currentCooldown > 0)
        {
            cout << "Skill masih cooldown " << currentCooldown << " turn.\n";
            turnConsumed = false;
            return;
        }

        if (ctx.player.mp < manaCost)
        {
            cout << "MP tidak cukup.\n";
            turnConsumed = false;
            return;
        }

        ctx.player.mp -= manaCost;
        setCooldown(ctx.player, skillId, cooldown);
        turnConsumed = true;

        const int baseAttack = calculatePlayerAttack(ctx, &battle);
        const int primaryStat = getPrimaryStatValue(ctx.player, classPrimaryStat(ctx, ctx.player.class_id));
        int damage = max(1, baseAttack - calculateEnemyDefense(enemy, battle) / 2);
        bool ignoreDefense = false;

        if (skillId == "pommel_strike")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kPommelStrikeMultiplier) - calculateEnemyDefense(enemy, battle);
            battle.enemyStunTurns = skill_balance::kPommelStrikeStunTurns;
        }
        else if (skillId == "iron_cleave" || skillId == "mana_bolt")
        {
            const double multiplier = (skillId == "mana_bolt")
                                          ? skill_balance::kManaBoltMultiplier
                                          : skill_balance::kIronCleaveMultiplier;
            damage = static_cast<int>(baseAttack * multiplier) - calculateEnemyDefense(enemy, battle);
        }
        else if (skillId == "challenger_s_roar")
        {
            battle.playerDefBuffTurns = skill_balance::kChallengerRoarDefenseBuffTurns;
            damage = 0;
        }
        else if (skillId == "impact_crater")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kImpactCraterMultiplier) - calculateEnemyDefense(enemy, battle);
        }
        else if (skillId == "blood_and_iron")
        {
            const int sacrifice = max(1, static_cast<int>(ctx.player.max_hp * skill_balance::kBloodAndIronSelfDamageRatio));
            ctx.player.hp = max(1, ctx.player.hp - sacrifice);
            battle.playerAtkBuffTurns = skill_balance::kBloodAndIronAttackBuffTurns;
            battle.playerDefBuffTurns = skill_balance::kBloodAndIronDefenseBuffTurns;
            damage = 0;
        }
        else if (skillId == "shattering_onslaught")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kShatteringOnslaughtMultiplier) - calculateEnemyDefense(enemy, battle);
            battle.enemyDefDebuffTurns = skill_balance::kShatteringOnslaughtDefenseDebuffTurns;
        }
        else if (skillId == "calamity_end")
        {
            if (!enemy.is_boss && enemy.hp <= static_cast<int>(enemy.max_hp * skill_balance::kCalamityEndExecuteThreshold))
                damage = enemy.hp;
            else
                damage = static_cast<int>(baseAttack * skill_balance::kCalamityEndMultiplier) - calculateEnemyDefense(enemy, battle);
        }
        else if (skillId == "dominion_aura")
        {
            battle.playerDefBuffTurns = skill_balance::kDominionAuraDefenseBuffTurns;
            battle.enemyAtkDebuffTurns = skill_balance::kDominionAuraEnemyAttackDebuffTurns;
            damage = 0;
        }
        else if (skillId == "undying_juggernaut")
        {
            battle.playerInvincibleTurns = skill_balance::kUndyingJuggernautInvincibleTurns;
            battle.playerAtkBuffTurns = skill_balance::kUndyingJuggernautAttackBuffTurns;
            damage = 0;
        }
        else if (skillId == "ignite")
        {
            damage = static_cast<int>(primaryStat * skill_balance::kIgnitePrimaryMultiplier) +
                     itemBonusFromEquipped(ctx, ctx.player, "atk") - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemyDotTurns = skill_balance::kIgniteDotTurns;
            battle.enemyDotDamage = max(1, static_cast<int>(primaryStat * skill_balance::kIgniteDotMultiplier));
            battle.enemyDotLabel = "Burn";
        }
        else if (skillId == "arcane_shield")
        {
            battle.playerShield += max(skill_balance::kArcaneShieldMinimum,
                                       static_cast<int>(primaryStat * skill_balance::kArcaneShieldPrimaryMultiplier));
            damage = 0;
        }
        else if (skillId == "chain_lightning")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kChainLightningMultiplier) - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemySilenceTurns = skill_balance::kChainLightningSilenceTurns;
        }
        else if (skillId == "frost_nova")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kFrostNovaMultiplier) - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemyStunTurns = skill_balance::kFrostNovaStunTurns;
        }
        else if (skillId == "meteor_fall")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kMeteorFallMultiplier) - calculateEnemyDefense(enemy, battle);
        }
        else if (skillId == "abyssal_singularity")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kAbyssalSingularityMultiplier) - calculateEnemyDefense(enemy, battle) / 2;
            const int drain = max(skill_balance::kAbyssalSingularityMinimumDrain,
                                  damage / skill_balance::kAbyssalSingularityDrainDivisor);
            ctx.player.hp = min(ctx.player.max_hp, ctx.player.hp + drain);
        }
        else if (skillId == "astral_rebirth")
        {
            ctx.player.mp = min(ctx.player.max_mp,
                                ctx.player.mp + max(skill_balance::kAstralRebirthMinimumManaRestore,
                                                    ctx.player.max_mp / skill_balance::kAstralRebirthManaRestoreDivisor));
            battle.playerAtkBuffTurns = skill_balance::kAstralRebirthAttackBuffTurns;
            damage = 0;
        }
        else if (skillId == "void_apocalypse")
        {
            ignoreDefense = true;
            damage = static_cast<int>(baseAttack * skill_balance::kVoidApocalypseMultiplier);
        }
        else if (skillId == "quick_draw")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kQuickDrawMultiplier) - calculateEnemyDefense(enemy, battle) / 2;
        }
        else if (skillId == "toxic_arrow")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kToxicArrowMultiplier) - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemyDotTurns = skill_balance::kToxicArrowDotTurns;
            battle.enemyDotDamage = max(1, static_cast<int>(ctx.player.stats.agi * skill_balance::kToxicArrowDotAgiMultiplier));
            battle.enemyDotLabel = "Poison";
        }
        else if (skillId == "agility_boost")
        {
            battle.playerAgiBuffTurns = skill_balance::kAgilityBoostTurns;
            damage = 0;
        }
        else if (skillId == "shadow_strike")
        {
            battle.nextAttackGuaranteedCrit = true;
            damage = static_cast<int>(baseAttack * skill_balance::kShadowStrikeMultiplier) - calculateEnemyDefense(enemy, battle) / 2;
        }
        else if (skillId == "phantom_veil")
        {
            battle.playerEvadeTurns = skill_balance::kPhantomVeilEvadeTurns;
            damage = 0;
        }
        else if (skillId == "blade_dance")
        {
            int hits = randInt(skill_balance::kBladeDanceMinHits, skill_balance::kBladeDanceMaxHits);
            damage = 0;
            for (int i = 0; i < hits; ++i)
                damage += max(1, static_cast<int>(baseAttack * skill_balance::kBladeDanceHitMultiplier) - calculateEnemyDefense(enemy, battle) / 3);
        }
        else if (skillId == "oblivion_edge")
        {
            if (!enemy.is_boss && randInt(1, 100) <= skill_balance::kOblivionEdgeExecuteChance)
                damage = enemy.hp;
            else
                damage = static_cast<int>(baseAttack * skill_balance::kOblivionEdgeMultiplier) - calculateEnemyDefense(enemy, battle) / 2;
        }
        else if (skillId == "eternal_silence")
        {
            damage = static_cast<int>(baseAttack * skill_balance::kEternalSilenceMultiplier) - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemySilenceTurns = skill_balance::kEternalSilenceTurns;
        }
        else if (skillId == "shadow_meld")
        {
            battle.playerEvadeTurns = skill_balance::kShadowMeldEvadeTurns;
            battle.nextAttackGuaranteedCrit = true;
            damage = 0;
        }

        if (damage > 0)
        {
            bool critical = randUnit() <= calculatePlayerCritRate(ctx, &battle);
            if (critical)
                damage = static_cast<int>(damage * skill_balance::kSkillCriticalDamageMultiplier);
            if (!ignoreDefense)
                damage = max(1, damage);
            enemy.hp = max(0, enemy.hp - damage);
            cout << "Kamu menggunakan " << skillName << " dan memberi " << damage << " damage";
            if (critical)
                cout << " [Critical]";
            cout << ".\n";
        }
        else
        {
            cout << "Kamu menggunakan " << skillName << ".\n";
        }
    }

    void showBattleSkills(const GameContext &ctx)
    {
        const auto *cls = getClassById(ctx, ctx.player.class_id);
        if (!cls || !cls->contains("skills") || !(*cls)["skills"].is_array())
        {
            cout << "Tidak ada skill.\n";
            return;
        }

        clearScreen();
        printStateHeader(ctx, "BATTLE SKILL LIST");
        cout << colorText("Class", Color::Yellow, true) << " : " << cls->value("name", std::string(ctx.player.class_id)) << '\n';
        cout << colorText("Formula Dasar", Color::Yellow, true) << " : " << cls->value("base_damage_formula", std::string("-")) << "\n\n";

        SkillTreeNode *root = buildSkillTree((*cls)["skills"], 1, static_cast<int>((*cls)["skills"].size()));
        cout << "Binary Skill Tree Traversal: InOrder\n\n";
        printSkillTreeInOrder(ctx.player, root);
        clearSkillTree(root);

        printLine('=');
    }
} // namespace state_helpers

// ---- merged from src/states/battle_state.cpp


using namespace std;
using namespace consoleui;
using namespace state_helpers;

// Flow dungeon:
// 1. pilih aksi di depth aktif
// 2. jika move forward, pemain menempuh beberapa langkah tetap
// 3. di dalam langkah itu bisa muncul encounter acak
// 4. jika seluruh langkah selesai, depth dianggap clear
void runBattle(GameContext &ctx)
{
    auto leaveBattleState = [&]() {
        if (!ctx.stateStack.empty() && ctx.stateStack.back() == GameState::Battle)
            ctx.stateStack.pop_back();
    };

    while (ctx.running && !ctx.stateStack.empty() && ctx.stateStack.back() == GameState::Battle)
    {
        const json *dungeon = getDungeonById(ctx, ctx.player.progress.current_dungeon);
        if (dungeon == nullptr)
        {
            cout << "Dungeon tidak ditemukan. Kembali ke hub.\n";
            waitForEnter();
            leaveBattleState();
            return;
        }

        const json *depthData = findDungeonDepth(*dungeon, ctx.player.progress.current_depth);
        if (depthData == nullptr)
        {
            cout << "Depth tidak valid. Kembali ke hub.\n";
            waitForEnter();
            leaveBattleState();
            return;
        }

        clearScreen();
        refreshPlayerResources(ctx);

        printStateHeader(ctx, "EKSPLORASI DUNGEON");
        cout << colorText(dungeon->value("name", string()), Color::Yellow, true) << '\n';
        cout << colorText("Depth Aktif", Color::Cyan, true) << " : " << ctx.player.progress.current_depth << '\n';
        cout << colorText("Level Range", Color::Cyan, true) << " : "
             << (*depthData)["level_range"].value("min", 1)
             << "-" << (*depthData)["level_range"].value("max", 999) << '\n';
        cout << colorText("ATK / DEF", Color::Cyan, true) << "   : "
             << calculatePlayerAttack(ctx) << " / " << calculatePlayerDefense(ctx) << '\n';
        cout << colorText("HP / MP", Color::Cyan, true) << "     : "
             << ctx.player.hp << "/" << ctx.player.max_hp
             << " | " << ctx.player.mp << "/" << ctx.player.max_mp << "\n\n";

        printDungeonLore(*depthData);
        cout << "\n";
        printLine('=');
        cout << colorText("1. Move Forward (" + to_string(game_rules::kMoveStepsPerDepth) + " langkah)", Color::Green) << '\n';
        cout << colorText("2. Ganti Dungeon / Depth", Color::Cyan) << '\n';
        cout << colorText("3. Kembali ke Hub", Color::Magenta) << '\n';
        printLine('=');
        cout << "Choose: ";

        const string explorationChoice = readLine();
        if (explorationChoice == "2")
        {
            selectDungeonDepth(ctx, false);
            continue;
        }
        if (explorationChoice == "3")
        {
            leaveBattleState();
            return;
        }
        if (explorationChoice != "1")
        {
            cout << "Pilihan tidak valid.\n";
            waitForEnter();
            continue;
        }

        EncounterQueue encounterSteps = buildEncounterStepsForMove();
        const int plannedEncounterCount = encounterSteps.size();
        int nextEncounterStep = 0;
        bool hasQueuedEncounter = encounterSteps.dequeue(nextEncounterStep);
        bool journeyCancelled = false;

        for (int step = 1; step <= game_rules::kMoveStepsPerDepth; ++step)
        {
            clearScreen();
            printStateHeader(ctx, "MENYUSURI KORIDOR");
            cout << colorText(dungeon->value("name", string()), Color::Yellow, true)
                 << " | Depth " << ctx.player.progress.current_depth << "\n\n";
            cout << colorText("Langkah", Color::Cyan, true) << " : " << step << "/" << game_rules::kMoveStepsPerDepth << "\n";
            cout << colorText("Encounter dalam perjalanan ini", Color::Cyan, true)
                 << " : " << plannedEncounterCount << "\n";
            printLine('-');

            const bool encounterNow = hasQueuedEncounter && nextEncounterStep == step;
            if (!encounterNow)
            {
                cout << colorText("Lorong masih sepi, tapi hawa dungeon terasa berat.", Color::Blue) << '\n';
                if (step < game_rules::kMoveStepsPerDepth)
                    waitForEnter();
                continue;
            }
            hasQueuedEncounter = encounterSteps.dequeue(nextEncounterStep);

            cout << colorText("Ada langkah kaki lain yang mendekat. Musuh langsung menyerang!", Color::Red, true) << "\n\n";

            EnemyInstance enemy = spawnEnemyForDepth(ctx, *dungeon, ctx.player.progress.current_depth);
            const bool enemyStartsFirst = enemyGetsFirstTurn(ctx, enemy, false);

            waitForEnter();

            const EncounterResult result = runEncounterBattle(ctx, *dungeon, *depthData, enemy, enemyStartsFirst);
            if (result == EncounterResult::Defeat)
            {
                leaveBattleState();
                return;
            }
            if (result == EncounterResult::Retreat)
            {
                journeyCancelled = true;
                break;
            }

            if (step < game_rules::kMoveStepsPerDepth)
            {
                clearScreen();
                printStateHeader(ctx, "LANJUT MENYUSURI DEPTH");
                cout << colorText("Kamu menang dan kembali melangkah ke depan.", Color::Green) << '\n';
                cout << "Sisa langkah: " << (game_rules::kMoveStepsPerDepth - step) << '\n';
                waitForEnter();
            }
        }

        if (journeyCancelled)
            continue;

        clearScreen();
        printStateHeader(ctx, "UJUNG DEPTH TERCAPAI");
        cout << colorText("Kamu berhasil menyelesaikan perjalanan depth ini.", Color::Green, true) << '\n';
        applyDepthCompletionRewards(ctx, *dungeon, *depthData);
        waitForEnter();
    }
}


