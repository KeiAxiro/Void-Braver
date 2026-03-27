#include "Combat/Commands.h"
#include "Combat/CombatMath.h"

namespace Combat {

    AttackResult Commands::ExecuteAttack(Entities::Entity& attacker, Entities::Entity& defender) {
        const auto& atkStats = attacker.GetStats();
        const auto& defStats = defender.GetStats();

        int baseDamage = CombatMath::CalculateBaseDamage(atkStats.attack, defStats.defense);
        bool isCrit = CombatMath::IsCriticalHit(atkStats.critRate);
        int finalDamage = CombatMath::CalculateFinalDamage(baseDamage, isCrit, atkStats.critDamage);

        defender.TakeDamage(finalDamage);

        AttackResult result;
        result.damageDealt = finalDamage;
        result.isCritical = isCrit;
        result.isDefenderDead = defender.IsDead();

        return result;
    }

}