#pragma once
#include <string>
#include <vector>
#include "lib/json.hpp" // Sesuaikan path

class AccountManager
{
private:
    std::string dbFilePath;
    nlohmann::json dbCache; // Cache JSON di memori

    void loadDatabase();
    void saveDatabase();

    struct Account
    {
        std::string account_id;
        std::string username;
        std::string email;
        std::string password;
        std::string created_at;
        std::string last_login;
    };

public:
    explicit AccountManager(std::string filePath = "../data/databases/account.json");

    bool registerAccount(const std::string &username, const std::string &password);
    bool loginAccount(const std::string &username, const std::string &password);
    [[nodiscard]] std::vector<std::string> getCharacterList(const std::string &username) const;
    [[nodiscard]] bool isUsernameTaken(const std::string &username) const;
};