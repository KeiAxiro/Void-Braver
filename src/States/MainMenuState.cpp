#include "States/MainMenuState.h"
#include "Core/StateManager.h"
#include "States/CharSelectionState.h"
#include <iostream>
#include <string>
using namespace std;

namespace States
{

    // Konstruktor inisialisasi default index pilihan
    MainMenuState::MainMenuState() : selectedOption(-1) {}

    void MainMenuState::Init()
    {
        // Menggunakan ANSI escape sequence (\033[2J\033[1;1H) untuk membersihkan buffer terminal dan mereset kursor ke koordinat (1,1)
        cout << "\033[2J\033[1;1H";

        cout << "========================================\n";
        cout << "              VOID BRAVER               \n";
        cout << "========================================\n";
        cout << "1. New Game\n";
        cout << "2. Load Game\n";
        cout << "3. Exit\n";
        cout << "========================================\n";
    }

    void MainMenuState::HandleInput(Core::StateManager &stateManager)
    {
        string input;
        cout << "\nEnter your choice: ";
        // Menerima input sebagai tipe data string untuk menghindari buffer flag error pada std::cin jika user memasukkan karakter non-numerik
        cin >> input;

        // Mapping logika murni dari string input ke integer identifier lokal
        if (input == "1")
        {
            selectedOption = 0;
        }
        else if (input == "2")
        {
            selectedOption = 1;
        }
        else if (input == "3")
        {
            selectedOption = 2;
        }
        else
        {
            selectedOption = -1; 
        }
    }

    void MainMenuState::Update(Core::StateManager &stateManager)
    {
        if (selectedOption == 0)
        {
            cout << "Transitioning to Character Selection...\n";
            // Melakukan transisi state: Mendealokasi state saat ini dan menginjeksi CharSelectionState ke memori menggunakan smart pointer
            stateManager.ChangeState(make_unique<CharSelectionState>());

            selectedOption = -1; 
        }
        else if (selectedOption == 1)
        {
            cout << "Transitioning to Load Game...\n";
            // Nanti kita uncomment baris di bawah ini setelah LoadState dibuat
            // stateManager.ChangeState(make_unique<LoadState>());

            selectedOption = -1;
        }
        else if (selectedOption == 2)
        {
            cout << "Exiting Void Braver. Goodbye!\n";
            // Memanggil metode Quit untuk memberikan sinyal terminasi (break) pada game loop utama di Core
            stateManager.Quit();
        }
    }

    void MainMenuState::Render()
    {
        cout << "\033[2J\033[1;1H";

        cout << "========================================\n";
        cout << "              VOID BRAVER               \n";
        cout << "========================================\n";
        cout << "1. New Game\n";
        cout << "2. Load Game\n";
        cout << "3. Exit\n";
        cout << "========================================\n";
    }

}