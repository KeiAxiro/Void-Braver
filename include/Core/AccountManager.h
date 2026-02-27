#pragma once
#include <string>
#include <vector>
#include "Models/Models.h"
#include "Core/JsonDbHelper.h"

class AccountManager
{
private:
    // Panggil helper-nya khusus untuk struct Account
    JsonDbHelper<Account> db;

public:
    // Path JSON-nya sekarang disesuaikan (karena exe kamu ada di folder build, perlu naik 1 folder dulu)
    explicit AccountManager(std::string filePath = "../data/databases/account.json");

    bool registerAccount(const std::string &username, const std::string &password);
    bool loginAccount(const std::string &username, const std::string &password);
    [[nodiscard]] std::vector<std::string> getCharacterList(const std::string &username);
    [[nodiscard]] bool isUsernameTaken(const std::string &username);
};