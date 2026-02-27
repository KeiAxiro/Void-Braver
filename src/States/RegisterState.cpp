#include "States/RegisterState.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <string>

void RegisterState::init(StateManager &stateManager) {}

void RegisterState::render()
{
    ConsoleUI::printHeader("REGISTER ACCOUNT");
    std::cout << "Ketik 'batal' pada username atau password untuk kembali ke menu utama.\n\n";
}

void RegisterState::update(StateManager &stateManager)
{
    std::string username, password;

    std::cout << "Masukkan Username: ";
    // Menggunakan std::ws untuk membersihkan buffer dan getline agar aman dari spasi
    std::getline(std::cin >> std::ws, username);

    // Cek pembatalan di tahap username
    if (username == "batal")
    {
        stateManager.setNotify(ConsoleUI::Green("Registrasi dibatalkan. Kembali ke menu utama."));
        stateManager.popState();
        return;
    }

    std::cout << "Masukkan Password: ";
    std::getline(std::cin >> std::ws, password);

    // Cek pembatalan di tahap password
    if (password == "batal")
    {
        stateManager.setNotify(ConsoleUI::Green("Registrasi dibatalkan. Kembali ke menu utama."));
        stateManager.popState();
        return;
    }

    // MENGGUNAKAN ACCOUNT MANAGER DARI CONTEXT GLOBAL
    // Di balik layar, ini otomatis memanggil JsonDbHelper<Account> kita yang baru!
    if (stateManager.getContext().accountManager.registerAccount(username, password))
    {
        stateManager.setNotify(ConsoleUI::Green("Registrasi berhasil! Silakan login."));
        stateManager.popState(); // Balik ke Main Menu
    }
    else
    {
        stateManager.setNotify(ConsoleUI::Red("Registrasi Gagal! Username '" + username + "' sudah digunakan."));
        // Tidak di-pop, sehingga layar akan merender ulang RegisterState agar user bisa mencoba lagi
    }
}