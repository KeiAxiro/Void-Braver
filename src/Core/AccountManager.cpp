#include "AccountManager.h"
#include <fstream>
#include <iostream>
#include "lib/json.hpp"

using json = nlohmann::json;

AccountManager::AccountManager() : dbFile("../data/databases/accounts.json") {
    // Buat file JSON kosong jika belum ada saat program jalan
    std::ifstream file(dbFile);
    if (!file.is_open()) {
        std::ofstream newFile(dbFile);
        newFile << json::object(); // Tulis {}
        newFile.close();
    }
}

bool AccountManager::isUsernameTaken(const std::string& username) {
    std::ifstream file(dbFile);
    json db;
    if (file >> db) {
        return db.contains(username);
    }
    return false;
}

bool AccountManager::registerAccount(const std::string& username, const std::string& password) {
    if (isUsernameTaken(username)) return false; // Gagal jika username sudah ada

    std::ifstream inFile(dbFile);
    json db;
    inFile >> db; // Baca data yang sudah ada
    inFile.close();

    // Tambahkan user baru. (Catatan: Untuk game sungguhan password harus di-hash!)
    db[username] = {
        {"password", password},
        {"characters", json::array()} // Siapkan slot karakter untuk nanti
    };

    std::ofstream outFile(dbFile);
    outFile << db.dump(4); // Simpan kembali ke file dengan indentasi 4 spasi biar rapi
    outFile.close();

    return true;
}

bool AccountManager::loginAccount(const std::string& username, const std::string& password) {
    std::ifstream file(dbFile);
    json db;
    if (file >> db) {
        if (db.contains(username) && db[username]["password"] == password) {
            return true; // Login berhasil
        }
    }
    return false; // Login gagal (username tidak ada / password salah)
}
std::vector<std::string> AccountManager::getCharacterList(const std::string& username) {
    std::vector<std::string> charList;
    std::ifstream file(dbFile);
    json db;
    if (file >> db) {
        if (db.contains(username) && db[username].contains("characters")) {
            // Looping isi array "characters" di JSON
            for (const auto& character : db[username]["characters"]) {
                charList.push_back(character["name"]); 
            }
        }
    }
    return charList;
}