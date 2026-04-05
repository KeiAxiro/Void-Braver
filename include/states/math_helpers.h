#pragma once

namespace state_helpers
{
    double normalizedRatio(int value, int minValue, int maxValue);
    int interpolateStat(int level, int levelMin, int levelMax, int statMin, int statMax);
} // namespace state_helpers
