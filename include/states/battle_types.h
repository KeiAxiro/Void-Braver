#pragma once

#include <string>
#include <utility>
#include <vector>

#include "globals.h"

namespace state_helpers
{
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
        std::vector<std::pair<std::string, int>> drops;
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
} // namespace state_helpers
