#include "Core/AccountManager.h"
#include <fstream>
#include <iostream>
#include <algorithm> // Diperlukan untuk std::find_if

using json = nlohmann::json;

AccountManager::AccountManager(std::string filePath) : dbFilePath(std::move(filePath))
{
    loadDatabase();
}

void AccountManager::loadDatabase()
{
    std::ifstream file(dbFilePath);
    if (file.is_open() && file.peek() != std::ifstream::traits_type::eof())
    {
        try
        {
            file >> dbCache;
        }
        catch (json::parse_error &e)
        {
            std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            dbCache = json::array();
        }
    }
    else
    {
        dbCache = json::array(); // Inisialisasi sebagai Array []
        saveDatabase();
    }
}

void AccountManager::saveDatabase()
{
    std::ofstream file(dbFilePath);
    if (file.is_open())
    {
        file << dbCache.dump(4);
    }
}

bool AccountManager::isUsernameTaken(const std::string &username) const
{
    // Karena dbCache adalah array, kita cari objek yang memiliki username tsb
    return std::any_of(dbCache.begin(), dbCache.end(), [&](const json &acc)
                       { return acc.is_object() && acc.contains("username") && acc["username"] == username; });
}

bool AccountManager::registerAccount(const std::string &username, const std::string &password)
{
    if (isUsernameTaken(username))
        return false;

    // Membuat objek akun baru sesuai struktur yang kamu kirim
    json newAccount = {
        {"account_id", std::to_string(dbCache.size() + 1)},
        {"username", username},
        {"email", username + "@mail.com"}, // Default email
        {"password", password},            // Plain text sesuai request
        {"created_at", "2026-02-26 10:00:00"},
        {"last_login", "2026-02-26 10:00:00"},
        {"characters", json::array()}};

    dbCache.push_back(newAccount);
    saveDatabase();
    return true;
}

bool AccountManager::loginAccount(const std::string &username, const std::string &password)
{
    // Cari akun dengan username yang cocok
    auto it = std::find_if(dbCache.begin(), dbCache.end(), [&](const json &acc)
                           { return acc.is_object() && acc.contains("username") && acc["username"] == username; });

    if (it != dbCache.end())
    {
        // Bandingkan password plain text
        return (*it)["password"] == password;
    }
    return false;
}

std::vector<std::string> AccountManager::getCharacterList(const std::string &username) const
{
    std::vector<std::string> charList;

    auto it = std::find_if(dbCache.begin(), dbCache.end(), [&](const json &acc)
                           { return acc.is_object() && acc.contains("username") && acc["username"] == username; });

    if (it != dbCache.end() && it->contains("characters"))
    {
        for (const auto &character : (*it)["characters"])
        {
            if (character.contains("name"))
            {
                charList.push_back(character["name"]);
            }
        }
    }
    return charList;
}