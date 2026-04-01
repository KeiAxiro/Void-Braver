#include "Combat/CombatMath.h"
#include <random>
#include <algorithm>

using namespace std;
namespace CombatMath {

    int CalculateBaseDamage(int attack, int defense) {
        return max(1, attack - defense);
    }

    bool IsCriticalHit(float critRate) {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<float> dis(0.0f, 1.0f);
        
        return dis(gen) <= critRate;
    }

    int CalculateFinalDamage(int baseDamage, bool isCrit, float critDamage) {
        if (isCrit) {
            return static_cast<int>(baseDamage * critDamage);
        }
        return baseDamage;
    }

}