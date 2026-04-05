#include "states/battle_features.h"

#include "database.h"
#include "states/input_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/progress_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"
#include "config/enemy_balance.h"
#include "config/player_balance.h"
#include "config/game_rules.h"
#include "config/skill_balance.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

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

    vector<int> buildEncounterStepsForMove()
    {
        vector<int> steps;
        const int encounterCount = randInt(game_rules::kMinEncountersPerMove, game_rules::kMaxEncountersPerMove);

        while (static_cast<int>(steps.size()) < encounterCount)
        {
            const int step = randInt(1, game_rules::kMoveStepsPerDepth);
            if (find(steps.begin(), steps.end(), step) == steps.end())
                steps.push_back(step);
        }

        sort(steps.begin(), steps.end());
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

                showBattlePanel("SKILL RESULT", false);
                resolveSkillUse(ctx, enemy, battle, (*job)["skills"][static_cast<size_t>(skillIndex - 1)], turnConsumed);
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
