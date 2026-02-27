#include "Core/AccountManager.h"
#include <ctime>

// Constructor langsung panggil path
AccountManager::AccountManager(std::string filePath) : db(std::move(filePath)) {}

bool AccountManager::isUsernameTaken(const std::string &username)
{
    // Pakai fitur spesial findFirst dari helper kita
    Account *found = db.findFirst([&](const Account &acc)
                                  { return acc.username == username; });
    return found != nullptr; // Kalau ketemu (gak nullptr), berarti username sudah dipakai
}

bool AccountManager::registerAccount(const std::string &username, const std::string &password)
{
    if (isUsernameTaken(username))
        return false;

    // set variabel tanggall
    std::time_t now = std::time(nullptr);
    // std::string createdAt = std::ctime(&now);
    // std::string lastLogin = createdAt;

    std::tm *ltm = std::localtime(&now);

    // Menggunakan stringstream untuk menyimpan hasil format
    std::stringstream ss;
    ss << std::put_time(ltm, "%Y-%m-%d %H:%M:%S");

    std::string createdAt = ss.str();
    std::string lastLogin = createdAt;

    // Bikin objek Account pakai struct C++ biasa!
    Account newAccount;
    newAccount.account_id = std::to_string(db.getAll().size() + 1);
    newAccount.username = username;
    newAccount.email = username + "@mail.com";
    newAccount.password = password;
    newAccount.created_at = createdAt;
    newAccount.last_login = lastLogin;
    // newAccount.characters = {}; // Kosong saat baru daftar

    // Lempar ke mesin helper, otomatis tersimpan ke file JSON!
    db.insert(newAccount);
    return true;
}

bool AccountManager::loginAccount(const std::string &username, const std::string &password)
{
    Account *found = db.findFirst([&](const Account &acc)
                                  { return acc.username == username; });

    // Kalau akun ketemu, tinggal cek passwordnya (tinggal ketik .password, ada autocompletenya!)
    if (found != nullptr)
    {
        return found->password == password;
    }
    return false;
}

// std::vector<std::string> AccountManager::getCharacterList(const std::string &username)
// {
//     Account *found = db.findFirst([&](const Account &acc)
//                                   { return acc.username == username; });

//     if (found != nullptr)
//     {
//         return found->characters;
//     }
//     return {}; // Return vector kosong kalau akun gak ada
// }