#pragma once

namespace Core {
    struct Stats {
        int hp;
        int maxHp;
        int mp;
        int maxMp;
        int attack;
        int defense;
        float critRate;   
        float critDamage;
    };

    enum class CharacterClass {
        Knight,
        Sorcerer,
        Wanderer
    };
}