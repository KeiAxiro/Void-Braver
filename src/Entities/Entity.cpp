#include "Entities/Entity.h"
#include <algorithm>

namespace Entities {
    Entity::Entity(const std::string& name, const Core::Stats& stats)
        : name(name), stats(stats) {}

    void Entity::TakeDamage(int damage) {
        stats.hp = std::max(0, stats.hp - damage);
    }

    bool Entity::IsDead() const {
        return stats.hp <= 0;
    }

    const std::string& Entity::GetName() const {
        return name;
    }

    const Core::Stats& Entity::GetStats() const {
        return stats;
    }

    Core::Stats& Entity::GetStatsMut() {
        return stats;
    }
}