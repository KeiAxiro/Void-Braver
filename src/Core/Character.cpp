#include "Character.h"
#include <iostream>

Character::Character(std::string name, Gender gender, JobClass jobClass)
    : name(name), gender(gender), jobClass(jobClass), 
      level(1), currentXp(0), unspentStatPoints(0) {
    
    calculateBaseStats();
    currentHp = maxHp; // Isi HP sampai penuh saat pertama dibuat
}

void Character::calculateBaseStats() {
    // Stat dasar umum
    baseStats = Stats(5, 5, 5, 5); 

    // Konsep No. 4: Buff Gender
    if (gender == Gender::Male) {
        // Male: +10% HP (Nanti dihitung saat kalkulasi MaxHP)
        // Kita beri base VIT ekstra sebagai contoh
        baseStats.vit += 2; 
    } else if (gender == Gender::Female) {
        // Female: +10% ATK
        baseStats.str += 2; 
        baseStats.intl += 2;
        baseStats.agi += 2;
    }

    // Kalkulasi Max HP (Konsep No. 8: VIT menambah total HP)
    maxHp = 100 + (baseStats.vit * 10);
    
    if (gender == Gender::Male) {
        maxHp += (maxHp * 10 / 100); // Ekstra +10% HP untuk Male
    }
}

std::string Character::getName() const { return name; }
int Character::getLevel() const { return level; }
int Character::getHp() const { return currentHp; }
int Character::getMaxHp() const { return maxHp; }