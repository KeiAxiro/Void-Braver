#include "States/CharSelectionState.h"
#include "States/HubState.h"
#include "Utils/ConsoleUI.h"
#include "Core/Character.h" // PERBAIKAN: Menggunakan ../ untuk mundur ke folder src
#include <iostream>
#include <string>

void CharSelectionState::init(StateManager &stateManager) 
{
    // Mengosongkan player saat masuk ke layar ini agar tidak ada data sisa
    stateManager.getContext().player = nullptr;
}

void CharSelectionState::render()
{
    ConsoleUI::printHeader("PILIH KARAKTER");
    std::cout << "Pilih pahlawan yang akan kamu mainkan:\n\n";
    std::cout << "1. Knight     -   Tangguh dan berdaya tahan tinggi\n";
    std::cout << "2. Sorcerer   -   Ahli sihir dengan serangan kuat\n";
    std::cout << "3. Wanderer   -   Serba bisa dan lincah\n";
    std::cout << "\n------------------------------------\n";
    std::cout << "X. Logout (Kembali ke Main Menu)\n";
    std::cout << "\nPilihanmu: ";
}

void CharSelectionState::update(StateManager &stateManager)
{
    std::string input;
    std::cin >> input;

    if (input == "X" || input == "x")
    {
        stateManager.getContext().currentUsername = ""; 
        stateManager.setNotify(ConsoleUI::Green("Logout berhasil."));
        stateManager.popState(); 
        return;
    }

    // Logika pemilihan 3 karakter sistem
    if (input == "1") 
    {
        stateManager.getContext().player = std::make_unique<Character>("Knight", Gender::Male, JobClass::Knight);
        stateManager.setNotify(ConsoleUI::Green("Kamu memilih Knight! Memasuki kota..."));
        stateManager.pushState(std::make_unique<HubState>(), true);
    }
    else if (input == "2") 
    {
        stateManager.getContext().player = std::make_unique<Character>("Sorcerer", Gender::Female, JobClass::Sorcerer);
        stateManager.setNotify(ConsoleUI::Green("Kamu memilih Sorcerer! Memasuki kota..."));
        stateManager.pushState(std::make_unique<HubState>(), true);
    }
    else if (input == "3") 
    {
        stateManager.getContext().player = std::make_unique<Character>("Wanderer", Gender::Male, JobClass::Wanderer);
        stateManager.setNotify(ConsoleUI::Green("Kamu memilih Wanderer! Memasuki kota..."));
        stateManager.pushState(std::make_unique<HubState>(), true);
    }
    else 
    {
        stateManager.setNotify(ConsoleUI::Red("[Sistem]: Pilihan tidak valid! Pilih 1, 2, atau 3."));
    }
}
