#pragma once
#include "AccountManager.h"
#include "Character.h"
#include <memory>
#include <string>

// Struct ini murni untuk menampung data global/sesi pemain
struct GameContext {
    AccountManager accountManager;
    std::unique_ptr<Character> player = nullptr; 
    std::string currentUsername = "";
};