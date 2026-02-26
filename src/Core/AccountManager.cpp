#include "Include/AccountManager.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

AccountManager::AccountManager(std::string filePath) : dbFilePath(std::move(filePath)) {
    loadDatabase();
}

void AccountManager::loadDatabase() {
    std::ifstream file(dbFilePath);
    if (file.is_open()) {
        file >> dbCache;
    } else {
        dbCache = json::object(); // Buat empty JSON jika file tidak ada
        saveDatabase();
    }
}

void AccountManager::saveDatabase() {
    std::ofstream file(dbFilePath);
    if (file.is_open()) {
        file << dbCache.dump(4);
    }
}

bool AccountManager::isUsernameTaken(const std::string& username) const {
    return dbCache.contains(username);
}

bool AccountManager::registerAccount(const std::string& username, const std::string& password) {
    if (isUsernameTaken(username)) return false;

    dbCache[username] = {
        {"password", password}, // TODO: Integrasikan hashing library seperti Argon2/Bcrypt di sini
        {"characters", json::array()} 
    };

    saveDatabase(); // Hanya write ke file saat ada perubahan
    return true;
}

bool AccountManager::loginAccount(const std::string& username, const std::string& password) {
    if (isUsernameTaken(username)) {
        return dbCache[username]["password"] == password;
    }
    return false;
}

std::vector<std::string> AccountManager::getCharacterList(const std::string& username) const {
    std::vector<std::string> charList;
    if (isUsernameTaken(username) && dbCache[username].contains("characters")) {
        for (const auto& character : dbCache[username]["characters"]) {
            charList.push_back(character["name"]); 
        }
    }
    return charList;
}