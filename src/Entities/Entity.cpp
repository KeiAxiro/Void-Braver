#include "Entities/Entity.h"
#include <algorithm>

using namespace std;
namespace Entities {
    Entity::Entity(const string& name, const Core::Stats& stats)
        : name(name), stats(stats) {}

    void Entity::TakeDamage(int damage) {
        stats.hp = max(0, stats.hp - damage);
    }

    bool Entity::IsDead() const {
        return stats.hp <= 0;
    }

    const string& Entity::GetName() const {
        return name;
    }

    const Core::Stats& Entity::GetStats() const {
        return stats;
    }

    Core::Stats& Entity::GetStatsMut() {
        return stats;
    }
}