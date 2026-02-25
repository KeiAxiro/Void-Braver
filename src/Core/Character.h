#pragma once
#include <string>
#include "Stats.h"

enum class Gender { Male, Female };
enum class JobClass { Knight, Sorcerer, Wanderer };

class Character {
private:
    std::string name;
    Gender gender;
    JobClass jobClass;

    int level;
    int currentXp;
    int unspentStatPoints;

    Stats baseStats;
    int maxHp;
    int currentHp;

public:
    Character(std::string name, Gender gender, JobClass jobClass);

    // Getters
    std::string getName() const;
    int getLevel() const;
    int getHp() const;
    int getMaxHp() const;
    
    // Core Mechanics
    void calculateBaseStats(); // Menghitung buff gender & class awal
    void addXp(int amount);
    void levelUp();
    
    // Battle Stats
    int getAttackDamage() const;
    int getDefense() const;
};