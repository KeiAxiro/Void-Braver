#pragma once
#include "Entities/Entity.h"

namespace Combat {

    struct AttackResult {
        int damageDealt;
        bool isCritical;
        bool isDefenderDead;
    };

    class Commands {
    public:
        static AttackResult ExecuteAttack(Entities::Entity& attacker, Entities::Entity& defender);
    };

}