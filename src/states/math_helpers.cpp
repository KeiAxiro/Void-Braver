#include "states/math_helpers.h"

#include <algorithm>
#include <cmath>

namespace state_helpers
{
    double normalizedRatio(int value, int minValue, int maxValue)
    {
        if (maxValue <= minValue)
            return 0.0;

        return static_cast<double>(value - minValue) /
               static_cast<double>(maxValue - minValue);
    }

    int interpolateStat(int level, int levelMin, int levelMax, int statMin, int statMax)
    {
        const double ratio = normalizedRatio(level, levelMin, levelMax);
        const double value = statMin + (statMax - statMin) * ratio;
        return std::max(1, static_cast<int>(std::round(value)));
    }
} // namespace state_helpers
