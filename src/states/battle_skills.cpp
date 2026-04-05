#include "states/battle_features.h"

#include "database.h"
#include "states/character_features.h"
#include "states/input_helpers.h"
#include "states/inventory_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"
#include "config/skill_balance.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    bool useBattleConsumable(GameContext &ctx)
    {
        vector<size_t> consumableIndexes;
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

        for (size_t i = 0; i < (*cls)["skills"].size(); ++i)
        {
            const auto &skill = (*cls)["skills"][i];
            const int currentCd = getCooldown(ctx.player, skill.value("id", std::string()));
            cout << colorText(to_string(i + 1) + ". " + skill.value("name", std::string()), Color::Cyan, true) << '\n';
            cout << "   MP Cost      : " << skill.value("mana_cost", 0) << '\n';
            cout << "   Cooldown     : " << skill.value("cooldown_turns", 0)
                 << " | Current CD: " << currentCd << '\n';
            cout << "   Damage Form  : " << skill.value("damage_formula", std::string("-")) << '\n';
            cout << "   Description  : " << skill.value("mechanic_description", std::string("-")) << '\n';
            cout << "   Status       : " << (currentCd > 0 ? "Masih Cooldown" : "Siap Dipakai") << "\n\n";
        }

        printLine('=');
    }
} // namespace state_helpers
