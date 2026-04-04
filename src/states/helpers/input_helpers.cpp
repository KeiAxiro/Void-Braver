#include "state_helpers.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>

using namespace std;

namespace state_helpers
{
    mt19937 &rng()
    {
        static mt19937 gen{random_device{}()};
        return gen;
    }

    int randInt(int minValue, int maxValue)
    {
        if (maxValue < minValue)
            swap(minValue, maxValue);
        uniform_int_distribution<int> dist(minValue, maxValue);
        return dist(rng());
    }

    double randUnit()
    {
        uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng());
    }

    string trim(const string &value)
    {
        size_t start = 0;
        size_t end = value.size();
        while (start < end && isspace(static_cast<unsigned char>(value[start])))
            ++start;
        while (end > start && isspace(static_cast<unsigned char>(value[end - 1])))
            --end;
        return value.substr(start, end - start);
    }

    string toLower(string value)
    {
        for (char &ch : value)
            ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        return value;
    }

    string readLine()
    {
        string input;
        getline(cin >> ws, input);
        return trim(input);
    }

    bool tryParseInt(const string &input, int &value)
    {
        try
        {
            size_t processed = 0;
            const int parsed = stoi(input, &processed);
            if (processed != input.size())
                return false;
            value = parsed;
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    int getPrimaryStatValue(const Player &player, const string &primaryStat)
    {
        if (primaryStat == "INT")
            return player.stats.intl;
        if (primaryStat == "AGI")
            return player.stats.agi;
        if (primaryStat == "VIT")
            return player.stats.vit;
        return player.stats.str;
    }
} // namespace state_helpers
