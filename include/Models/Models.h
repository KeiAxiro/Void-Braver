#pragma once
#include <string>
#include <vector>
#include "lib/json.hpp"

// ==========================================
// 1. Definisikan Struct-nya
// ==========================================
struct Account
{
    std::string account_id;
    std::string username;
    std::string email;
    std::string password;
    std::string created_at;
    std::string last_login;
};

// ==========================================
// 2. Sihir nlohmann::json (Wajib di luar struct)
// ==========================================
// Macro ini mengajari C++ cara mengubah struct Account menjadi JSON dan sebaliknya.
// Pastikan nama variabel di struct SAMA PERSIS dengan key di file JSON kamu.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Account, account_id, username, email, password, created_at, last_login)

// Nanti kamu bisa tambahin struct lain ke bawahnya, contoh:
/*
struct Enemy {
    std::string id;
    std::string name;
    int hp;
    int damage;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Enemy, id, name, hp, damage)
*/