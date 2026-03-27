#pragma once
#include <string>
#include "Core/Stats.h"

namespace Entities {
    class Entity {
    protected:
        std::string name;
        Core::Stats stats;

    public:
        Entity(const std::string& name, const Core::Stats& stats);
        virtual ~Entity() = default;

        virtual void TakeDamage(int damage);
        bool IsDead() const;

        const std::string& GetName() const;
        const Core::Stats& GetStats() const;
        Core::Stats& GetStatsMut();
    };
}