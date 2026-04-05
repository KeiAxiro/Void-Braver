#include "states.h"
#include "database.h"
#include "state_helpers.h"
#include "utils/console_ui.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    int calculateEnemySpeed(const EnemyInstance &enemy)
    {
        return max(1, enemy.level * 2 + (enemy.atk / 6));
    }

    bool enemyGetsFirstTurn(const GameContext &ctx, const EnemyInstance &enemy, bool ambush)
    {
        int playerSpeed = calculatePlayerSpeed(ctx);
        int enemySpeed = calculateEnemySpeed(enemy);
        if (ambush)
            enemySpeed += 8;
        return enemySpeed > playerSpeed;
    }

    void tickBattleEffects(GameContext &ctx, EnemyInstance &enemy, BattleState &battle);
    bool useBattleConsumable(GameContext &ctx);
    void resolveSkillUse(GameContext &ctx, EnemyInstance &enemy, BattleState &battle, const json &skill, bool &turnConsumed);
    void showBattleSkills(const GameContext &ctx);
    int calculateEnemyDamage(const EnemyInstance &enemy, const BattleState &battle);
    int calculateEnemyDefense(const EnemyInstance &enemy, const BattleState &battle);
    void applyBattleRewards(GameContext &ctx, const json &dungeon, const json &depthRow, const EnemyInstance &enemy);

    std::vector<int> buildEncounterStepsForMove()
    {
        std::vector<int> steps;
        const int encounterCount = randInt(1, 3);

        while (static_cast<int>(steps.size()) < encounterCount)
        {
            const int step = randInt(1, 5);
            if (std::find(steps.begin(), steps.end(), step) == steps.end())
                steps.push_back(step);
        }

        std::sort(steps.begin(), steps.end());
        return steps;
    }

    EncounterResult runEncounterBattle(GameContext &ctx, const json &dungeon, const json &depthData, EnemyInstance enemy, bool enemyStartsFirst)
    {
        BattleState battle;

        auto showBattlePanel = [&](const string &title, bool showActions)
        {
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

        auto enemyTurn = [&](bool openingAttack = false)
        {
            if (battle.enemyStunTurns > 0 && !openingAttack)
            {
                cout << enemy.name << " terkena stun dan gagal bergerak.\n";
                return;
            }

            int enemyDamage = calculateEnemyDamage(enemy, battle) - calculatePlayerDefense(ctx, &battle);
            enemyDamage = max(1, enemyDamage + randInt(-3, 2));

            bool criticalHit = randUnit() <= enemy.crit_rate;
            if (criticalHit)
                enemyDamage = static_cast<int>(enemyDamage * 1.5);

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
                damage = max(1, damage + randInt(-2, 4));

                bool criticalHit = randUnit() <= calculatePlayerCritRate(ctx, &battle);
                if (criticalHit)
                    damage = static_cast<int>(damage * 1.5);

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
            ctx.player.hp = max(1, ctx.player.max_hp / 2);
            ctx.player.mp = max(1, ctx.player.max_mp / 2);
            cout << colorText("Kamu kalah. HP dan MP dipulihkan ke setengah maksimum.", Color::Red, true) << '\n';
            saveGame(ctx);
            waitForEnter();
            return EncounterResult::Defeat;
        }

        cout << colorText(enemy.name + " dikalahkan!", Color::Green, true) << '\n';
        applyBattleRewards(ctx, dungeon, depthData, enemy);
        waitForEnter();
        return EncounterResult::Victory;
    }

    void grantEnemyDrops(GameContext &ctx, const EnemyInstance &enemy)
    {
        if (enemy.drops.empty())
            return;

        cout << "Drop:\n";
        for (const auto &[itemId, dropRate] : enemy.drops)
        {
            if (randInt(1, 100) <= dropRate)
            {
                addItem(ctx.player, itemId, 1, false, "");
                cout << " - " << itemDisplayName(ctx, itemId) << '\n';
            }
        }
    }

    void advanceDungeonProgress(GameContext &ctx, const json &dungeon, int clearedDepth)
    {
        json &progress = ensureDungeonProgress(ctx.player, dungeon);
        const int maxDepth = dungeonMaxDepth(dungeon);
        progress["highest_cleared_depth"] = std::max(progress.value("highest_cleared_depth", 0), clearedDepth);

        if (clearedDepth >= maxDepth)
        {
            progress["unlocked_depth"] = maxDepth;
            progress["completed"] = true;
            cout << "Dungeon selesai! Reward orb: " << dungeon.value("reward_orb", std::string("-")) << '\n';
        }
        else
        {
            progress["unlocked_depth"] = std::max(progress.value("unlocked_depth", 1), clearedDepth + 1);
        }

        ctx.player.progress.current_dungeon = dungeon.value("id", std::string());
        ctx.player.progress.current_depth = std::min(progress.value("unlocked_depth", 1), maxDepth);
        ctx.player.progress.max_depth_unlocked = progress.value("unlocked_depth", 1);
    }

    int calculateEnemyDamage(const EnemyInstance &enemy, const BattleState &battle)
    {
        int attack = enemy.atk;
        if (!enemy.is_boss)
            attack = static_cast<int>(attack * 0.92);
        if (battle.enemyAtkDebuffTurns > 0)
            attack = static_cast<int>(attack * 0.85);
        if (enemy.is_boss)
            attack = static_cast<int>(attack * 1.10);
        return std::max(1, attack);
    }

    int calculateEnemyDefense(const EnemyInstance &enemy, const BattleState &battle)
    {
        int defense = enemy.def;
        if (battle.enemyDefDebuffTurns > 0)
            defense = static_cast<int>(defense * 0.30);
        return std::max(0, defense);
    }

    void tickBattleEffects(GameContext &ctx, EnemyInstance &enemy, BattleState &battle)
    {
        if (battle.enemyDotTurns > 0)
        {
            enemy.hp = std::max(0, enemy.hp - battle.enemyDotDamage);
            std::cout << enemy.name << " terkena " << battle.enemyDotLabel
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

    bool useBattleConsumable(GameContext &ctx)
    {
        std::vector<std::size_t> consumableIndexes;
        for (std::size_t i = 0; i < ctx.player.inventory.size(); ++i)
        {
            const auto *item = getItemById(ctx, ctx.player.inventory[i].item_id);
            if (item && item->value("category", std::string()) == "consumable")
                consumableIndexes.push_back(i);
        }

        if (consumableIndexes.empty())
        {
            clearScreen();
            printStateHeader(ctx, "CONSUMABLE");
            std::cout << "Tidak ada consumable di inventory.\n";
            waitForEnter();
            return false;
        }

        clearScreen();
        printStateHeader(ctx, "CONSUMABLE");
        std::cout << "Consumables:\n";
        for (std::size_t i = 0; i < consumableIndexes.size(); ++i)
        {
            const auto &entry = ctx.player.inventory[consumableIndexes[i]];
            std::cout << (i + 1) << ". " << itemDisplayName(ctx, entry.item_id) << " x" << entry.quantity << '\n';
        }
        std::cout << "Pilih nomor item atau c untuk batal: ";

        const std::string input = toLower(readLine());
        if (input == "c")
            return false;

        int selected = 0;
        if (!tryParseInt(input, selected) || selected < 1 || selected > static_cast<int>(consumableIndexes.size()))
        {
            std::cout << "Pilihan tidak valid.\n";
            waitForEnter();
            return false;
        }

        clearScreen();
        printStateHeader(ctx, "ITEM USED");
        useConsumable(ctx, consumableIndexes[static_cast<std::size_t>(selected - 1)]);
        return true;
    }

    void resolveSkillUse(GameContext &ctx, EnemyInstance &enemy, BattleState &battle, const json &skill, bool &turnConsumed)
    {
        const std::string skillId = skill.value("id", std::string());
        const std::string skillName = skill.value("name", skillId);
        const int manaCost = skill.value("mana_cost", 0);
        const int cooldown = skill.value("cooldown_turns", 0);
        const int currentCooldown = getCooldown(ctx.player, skillId);

        if (currentCooldown > 0)
        {
            std::cout << "Skill masih cooldown " << currentCooldown << " turn.\n";
            turnConsumed = false;
            return;
        }

        if (ctx.player.mp < manaCost)
        {
            std::cout << "MP tidak cukup.\n";
            turnConsumed = false;
            return;
        }

        ctx.player.mp -= manaCost;
        setCooldown(ctx.player, skillId, cooldown);
        turnConsumed = true;

        const int baseAttack = calculatePlayerAttack(ctx, &battle);
        const int primaryStat = getPrimaryStatValue(ctx.player, classPrimaryStat(ctx, ctx.player.class_id));
        int damage = std::max(1, baseAttack - calculateEnemyDefense(enemy, battle) / 2);
        bool ignoreDefense = false;

        if (skillId == "pommel_strike")
        {
            damage = static_cast<int>(baseAttack * 1.50) - calculateEnemyDefense(enemy, battle);
            battle.enemyStunTurns = 1;
        }
        else if (skillId == "iron_cleave" || skillId == "mana_bolt")
        {
            damage = static_cast<int>(baseAttack * 1.75) - calculateEnemyDefense(enemy, battle);
        }
        else if (skillId == "challenger_s_roar")
        {
            battle.playerDefBuffTurns = 2;
            damage = 0;
        }
        else if (skillId == "impact_crater")
        {
            damage = static_cast<int>(baseAttack * 1.90) - calculateEnemyDefense(enemy, battle);
        }
        else if (skillId == "blood_and_iron")
        {
            const int sacrifice = std::max(1, static_cast<int>(ctx.player.max_hp * 0.05));
            ctx.player.hp = std::max(1, ctx.player.hp - sacrifice);
            battle.playerAtkBuffTurns = 3;
            battle.playerDefBuffTurns = 3;
            damage = 0;
        }
        else if (skillId == "shattering_onslaught")
        {
            damage = static_cast<int>(baseAttack * 1.35) - calculateEnemyDefense(enemy, battle);
            battle.enemyDefDebuffTurns = 3;
        }
        else if (skillId == "calamity_end")
        {
            if (!enemy.is_boss && enemy.hp <= enemy.max_hp / 5)
                damage = enemy.hp;
            else
                damage = static_cast<int>(baseAttack * 2.60) - calculateEnemyDefense(enemy, battle);
        }
        else if (skillId == "dominion_aura")
        {
            battle.playerDefBuffTurns = 3;
            battle.enemyAtkDebuffTurns = 3;
            damage = 0;
        }
        else if (skillId == "undying_juggernaut")
        {
            battle.playerInvincibleTurns = 1;
            battle.playerAtkBuffTurns = 2;
            damage = 0;
        }
        else if (skillId == "ignite")
        {
            damage = static_cast<int>(primaryStat * 0.80) + itemBonusFromEquipped(ctx, ctx.player, "atk") - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemyDotTurns = 3;
            battle.enemyDotDamage = std::max(1, static_cast<int>(primaryStat * 0.40));
            battle.enemyDotLabel = "Burn";
        }
        else if (skillId == "arcane_shield")
        {
            battle.playerShield += std::max(30, static_cast<int>(primaryStat * 2.5));
            damage = 0;
        }
        else if (skillId == "chain_lightning")
        {
            damage = static_cast<int>(baseAttack * 1.55) - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemySilenceTurns = 1;
        }
        else if (skillId == "frost_nova")
        {
            damage = static_cast<int>(baseAttack * 1.35) - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemyStunTurns = 1;
        }
        else if (skillId == "meteor_fall")
        {
            damage = static_cast<int>(baseAttack * 2.80) - calculateEnemyDefense(enemy, battle);
        }
        else if (skillId == "abyssal_singularity")
        {
            damage = static_cast<int>(baseAttack * 2.10) - calculateEnemyDefense(enemy, battle) / 2;
            const int drain = std::max(10, damage / 4);
            ctx.player.hp = std::min(ctx.player.max_hp, ctx.player.hp + drain);
        }
        else if (skillId == "astral_rebirth")
        {
            ctx.player.mp = std::min(ctx.player.max_mp, ctx.player.mp + std::max(25, ctx.player.max_mp / 2));
            battle.playerAtkBuffTurns = 2;
            damage = 0;
        }
        else if (skillId == "void_apocalypse")
        {
            ignoreDefense = true;
            damage = static_cast<int>(baseAttack * 3.50);
        }
        else if (skillId == "quick_draw")
        {
            damage = static_cast<int>(baseAttack * 1.25) - calculateEnemyDefense(enemy, battle) / 2;
        }
        else if (skillId == "toxic_arrow")
        {
            damage = static_cast<int>(baseAttack * 1.10) - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemyDotTurns = 3;
            battle.enemyDotDamage = std::max(1, static_cast<int>(ctx.player.stats.agi * 0.5));
            battle.enemyDotLabel = "Poison";
        }
        else if (skillId == "agility_boost")
        {
            battle.playerAgiBuffTurns = 3;
            damage = 0;
        }
        else if (skillId == "shadow_strike")
        {
            battle.nextAttackGuaranteedCrit = true;
            damage = static_cast<int>(baseAttack * 1.75) - calculateEnemyDefense(enemy, battle) / 2;
        }
        else if (skillId == "phantom_veil")
        {
            battle.playerEvadeTurns = 1;
            damage = 0;
        }
        else if (skillId == "blade_dance")
        {
            int hits = randInt(3, 5);
            damage = 0;
            for (int i = 0; i < hits; ++i)
                damage += std::max(1, static_cast<int>(baseAttack * 0.45) - calculateEnemyDefense(enemy, battle) / 3);
        }
        else if (skillId == "oblivion_edge")
        {
            if (!enemy.is_boss && randInt(1, 100) <= 10)
                damage = enemy.hp;
            else
                damage = static_cast<int>(baseAttack * 2.30) - calculateEnemyDefense(enemy, battle) / 2;
        }
        else if (skillId == "eternal_silence")
        {
            damage = static_cast<int>(baseAttack * 1.70) - calculateEnemyDefense(enemy, battle) / 2;
            battle.enemySilenceTurns = 2;
        }
        else if (skillId == "shadow_meld")
        {
            battle.playerEvadeTurns = 1;
            battle.nextAttackGuaranteedCrit = true;
            damage = 0;
        }

        if (damage > 0)
        {
            bool critical = randUnit() <= calculatePlayerCritRate(ctx, &battle);
            if (critical)
                damage = static_cast<int>(damage * 1.6);
            if (!ignoreDefense)
                damage = std::max(1, damage);
            enemy.hp = std::max(0, enemy.hp - damage);
            std::cout << "Kamu menggunakan " << skillName << " dan memberi " << damage << " damage";
            if (critical)
                std::cout << " [Critical]";
            std::cout << ".\n";
        }
        else
        {
            std::cout << "Kamu menggunakan " << skillName << ".\n";
        }
    }

    void showBattleSkills(const GameContext &ctx)
    {
        const auto *cls = getClassById(ctx, ctx.player.class_id);
        if (!cls || !cls->contains("skills") || !(*cls)["skills"].is_array())
        {
            std::cout << "Tidak ada skill.\n";
            return;
        }

        clearScreen();
        printStateHeader(ctx, "BATTLE SKILL LIST");
        std::cout << colorText("Class", Color::Yellow, true) << " : " << cls->value("name", std::string(ctx.player.class_id)) << '\n';
        std::cout << colorText("Formula Dasar", Color::Yellow, true) << " : " << cls->value("base_damage_formula", std::string("-")) << "\n\n";

        for (std::size_t i = 0; i < (*cls)["skills"].size(); ++i)
        {
            const auto &skill = (*cls)["skills"][i];
            const int currentCd = getCooldown(ctx.player, skill.value("id", std::string()));
            std::cout << colorText(std::to_string(i + 1) + ". " + skill.value("name", std::string()), Color::Cyan, true) << '\n';
            std::cout << "   MP Cost      : " << skill.value("mana_cost", 0) << '\n';
            std::cout << "   Cooldown     : " << skill.value("cooldown_turns", 0)
                      << " | Current CD: " << currentCd << '\n';
            std::cout << "   Damage Form  : " << skill.value("damage_formula", std::string("-")) << '\n';
            std::cout << "   Description  : " << skill.value("mechanic_description", std::string("-")) << '\n';
            std::cout << "   Status       : " << (currentCd > 0 ? "Masih Cooldown" : "Siap Dipakai") << "\n\n";
        }

        printLine('=');
    }

    void applyBattleRewards(GameContext &ctx, const json &dungeon, const json &depthRow, const EnemyInstance &enemy)
    {
        (void)dungeon;
        (void)depthRow;

        const int expReward = enemy.exp_drop;
        const int goldReward = std::max(18, enemy.level * 8 + enemy.atk / 2);
        ctx.player.current_exp += expReward;
        ctx.player.gold += goldReward;
        std::cout << "Reward: +" << expReward << " EXP, +" << goldReward << " Gold\n";

        grantEnemyDrops(ctx, enemy);
        levelUpIfNeeded(ctx);
        saveGame(ctx);
    }

    void applyDepthCompletionRewards(GameContext &ctx, const json &dungeon, const json &depthRow)
    {
        const int baseDepthExp = depthRow.value("exp_gain", 0);
        const int clearExpReward = std::max(20, (baseDepthExp * 2) / 5);
        const int clearGoldReward = std::max(14, baseDepthExp / 5);

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
