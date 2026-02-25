#pragma once

struct Stats {
    int str;
    int vit;
    int intl; // Intelligence
    int agi;

    Stats(int s = 0, int v = 0, int i = 0, int a = 0) 
        : str(s), vit(v), intl(i), agi(a) {}

    Stats operator+(const Stats& other) const {
        return Stats(str + other.str, vit + other.vit, intl + other.intl, agi + other.agi);
    }
};