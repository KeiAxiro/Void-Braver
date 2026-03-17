#include "Core/Character.h"
#include <algorithm> // Untuk std::max, std::min jika diperlukan nanti

Character::Character(std::string charName, Gender charGender, JobClass charClass)
    : name(std::move(charName)), gender(charGender), jobClass(charClass)
{
    calculateBaseStats();
    currentHp = maxHp;
}

void Character::calculateBaseStats()
{
    baseStats = Stats(5, 5, 5, 5);

    if (gender == Gender::Male)
    {
        baseStats.vit += 2;
    }
    else if (gender == Gender::Female)
    {
        baseStats.str += 2;
        baseStats.intl += 2;
        baseStats.agi += 2;
    }

    maxHp = 100 + (baseStats.vit * 10);

    if (gender == Gender::Male)
    {
        maxHp += (maxHp * 10 / 100);
    }
}

std::string Character::getName() const { return name; }
int Character::getLevel() const { return level; }
int Character::getHp() const { return currentHp; }
int Character::getMaxHp() const { return maxHp; }