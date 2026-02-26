#pragma once
#include <string>
#include <vector>
#include "lib/json.hpp" // Sesuaikan path

class AccountManager {
private:
    std::string dbFilePath;
    nlohmann::json dbCache; // Cache JSON di memori

    void loadDatabase();
    void saveDatabase();

public:
    explicit AccountManager(std::string filePath = "../data/databases/accounts.json");
    
    bool registerAccount(const std::string& username, const std::string& password);
    bool loginAccount(const std::string& username, const std::string& password);
    [[nodiscard]] std::vector<std::string> getCharacterList(const std::string& username) const;
    [[nodiscard]] bool isUsernameTaken(const std::string& username) const;
};