#pragma once
#include <string>
#include <vector>

class AccountManager {
private:
    std::string dbFile;

public:
    AccountManager();
    
    // Fungsi utama
    bool registerAccount(const std::string& username, const std::string& password);
    bool loginAccount(const std::string& username, const std::string& password);
    std::vector<std::string> getCharacterList(const std::string& username);
    
    // Mengecek apakah username sudah ada
    bool isUsernameTaken(const std::string& username);
};