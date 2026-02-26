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

    int level{1};
    int currentXp{0};
    int unspentStatPoints{0};

    Stats baseStats;
    int maxHp{0};
    int currentHp{0};

public:
    Character(std::string charName, Gender charGender, JobClass charClass);

    // Getters dengan [[nodiscard]] agar compiler memberi warning jika return value diabaikan
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] int getLevel() const;
    [[nodiscard]] int getHp() const;
    [[nodiscard]] int getMaxHp() const;
    
    void calculateBaseStats(); 
    void addXp(int amount);
    void levelUp();
    
    [[nodiscard]] int getAttackDamage() const;
    [[nodiscard]] int getDefense() const;
};