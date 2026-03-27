#pragma once
#include "Core/Stats.h"

namespace CombatMath {
    
    int CalculateBaseDamage(int attackerAtk, int defenderDef);

    bool IsCriticalHit(float critRate);

    int CalculateFinalDamage(int baseDamage, bool isCrit, float critDamageMultiplier);

}