#pragma once

#include <string>

#include "globals.h"

namespace state_helpers
{
    int randInt(int minValue, int maxValue);
    double randUnit();
    std::string trim(const std::string &value);
    std::string toLower(std::string value);
    std::string readLine();
    bool tryParseInt(const std::string &input, int &value);
    int getPrimaryStatValue(const Player &player, const std::string &primaryStat);
} // namespace state_helpers
