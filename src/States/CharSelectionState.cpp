#include "States/CharSelectionState.h"
#include "States/HubState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <string>
using namespace std;

namespace States {

    // Inisialisasi default state dengan nilai -1 agar tidak ada class yang terpilih secara tidak sengaja
    CharSelectionState::CharSelectionState() : selectedClass(-1) {}

    void CharSelectionState::Init() {}

    void CharSelectionState::HandleInput(Core::StateManager& stateManager) {
        string input;
        cout << "\nEnter your choice: ";
        // Mengambil input sebagai string untuk mencegah game crash/infinite loop di cin jika user memasukkan tipe data yang salah (misal: huruf)
        cin >> input;

        // Mapping string input ke state integer
        if (input == "1") selectedClass = 1;
        else if (input == "2") selectedClass = 2;
        else if (input == "3") selectedClass = 3;
        else selectedClass = -1;
    }

    void CharSelectionState::Update(Core::StateManager& stateManager) {
        // Validasi: Hanya jalankan mutasi data jika input valid
        if (selectedClass != -1) {
            // Mendapatkan reference (&) ke objek player di GameContext agar perubahan data di sini bersifat persisten ke global state
            auto& p = stateManager.GetContext().player;
            
            // Inisialisasi base value untuk karakter baru
            p.level = 1;
            p.exp = 0;
            p.statPoints = 5;

            // Alokasi status fix dan pengisian struct equipment awal dari database/entity generator
            if (selectedClass == 1) {
                p.name = "Thorne"; p.job = "Knight";
                p.str = 10; p.intel = 5; p.agi = 10; p.vit = 40;
                p.weapon = Entities::getEquipmentData("Rusty Greatsword");
                p.armor = Entities::getEquipmentData("Knight's Plate");
            } else if (selectedClass == 2) {
                p.name = "Lara"; p.job = "Sorcerer";
                p.str = 5; p.intel = 25; p.agi = 15; p.vit = 20;
                p.weapon = Entities::getEquipmentData("Kosong"); 
                p.armor = Entities::getEquipmentData("Leather Jerkin");
            } else if (selectedClass == 3) {
                p.name = "Vane"; p.job = "Wanderer";
                p.str = 10; p.intel = 10; p.agi = 25; p.vit = 20;
                p.weapon = Entities::getEquipmentData("Rusty Greatsword");
                p.armor = Entities::getEquipmentData("Leather Jerkin");
            }
            
            // Menyimpan ID class ke context untuk referensi logis di state lain jika dibutuhkan
            stateManager.GetContext().playerClassId = selectedClass;

            cout << "\nYou have chosen " << p.name << "!\n";
            cout << "Data saved to Game Context!\n";
            Utils::ConsoleUI::Pause();

            // State Transition: Mengganti CharSelectionState saat ini dengan HubState
            // Menggunakan smart pointer (make_unique) untuk otomatisasi memory management (mencegah memory leak)
            stateManager.ChangeState(make_unique<HubState>(stateManager.GetContext()));
        }
    }

    void CharSelectionState::Render() {
        Utils::ConsoleUI::ClearScreen();

        cout << "========================================\n";
        cout << "            CHOOSE YOUR CLASS           \n";
        cout << "========================================\n";
        cout << "1. Thorne - Knight Type\n";
        cout << "2. Lara - Sorcerer Type\n";
        cout << "3. Vane - Wanderer Type\n";
        cout << "========================================\n";
    }
}