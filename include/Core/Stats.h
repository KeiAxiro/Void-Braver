#pragma once

struct Stats {
    int str = 0;
    int agi = 0;
    int vit = 0;
    int intl = 0;

    Stats() = default;
    Stats(int s, int a, int v, int i) : str(s), agi(a), vit(v), intl(i) {}
};