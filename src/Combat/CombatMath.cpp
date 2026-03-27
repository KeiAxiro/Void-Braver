#include "Combat/CombatMath.h"
#include <random>
#include <algorithm>

namespace CombatMath {

    int CalculateBaseDamage(int attack, int defense) {
        return std::max(1, attack - defense);
    }

    bool IsCriticalHit(float critRate) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        
        return dis(gen) <= critRate;
    }

    int CalculateFinalDamage(int baseDamage, bool isCrit, float critDamage) {
        if (isCrit) {
            return static_cast<int>(baseDamage * critDamage);
        }
        return baseDamage;
    }

}