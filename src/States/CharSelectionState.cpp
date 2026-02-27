// #include "States/CharSelectionState.h"
// // #include "CharacterCreationState.h"
// // #include "InGameState.h"
// #include "Utils/ConsoleUI.h"
// #include <iostream>
// #include <string>

// // Tidak butuh constructor yang aneh-aneh lagi karena username ada di Context
// void CharSelectionState::init(StateManager &stateManager)
// {
//     // Ambil daftar karakter menggunakan akun yang sedang login di Context
//     std::string currentUser = stateManager.getContext().currentUsername;
//     characters = stateManager.getContext().accountManager.getCharacterList(currentUser);
// }

// void CharSelectionState::render()
// {
//     ConsoleUI::printHeader("DAFTAR KARAKTER");

//     if (characters.empty())
//     {
//         std::cout << "[!] Kamu belum memiliki karakter.\n";
//     }
//     else
//     {
//         for (size_t i = 0; i < characters.size(); ++i)
//         {
//             std::cout << i + 1 << ". " << characters[i] << "\n";
//         }
//     }

//     std::cout << "------------------------------------\n";
//     std::cout << "N. Buat Karakter Baru\n";
//     std::cout << "X. Logout (Kembali ke Main Menu)\n";
//     std::cout << "Pilihan: ";
// }

// void CharSelectionState::update(StateManager &stateManager)
// {
//     std::string input;
//     std::cin >> input;

//     if (input == "N" || input == "n")
//     {
//         stateManager.setNotify("Masuk ke menu pembuatan karakter...");
//         // stateManager.pushState(std::make_unique<CharacterCreationState>());
//     }
//     else if (input == "X" || input == "x")
//     {
//         stateManager.getContext().currentUsername = ""; // Kosongkan sesi user
//         stateManager.setNotify(ConsoleUI::Green("Logout berhasil."));
//         stateManager.popState(); // Kembali
//     }
//     else
//     {
//         try
//         {
//             int choice = std::stoi(input);
//             if (choice > 0 && choice <= characters.size())
//             {
//                 std::string chosenChar = characters[choice - 1];
//                 stateManager.setNotify(ConsoleUI::Green("Memasuki dunia dengan karakter " + chosenChar + "..."));

//                 // Nanti load data karakter ke context:
//                 // stateManager.getContext().player = loadKarakterDariDb(chosenChar);
//                 // stateManager.pushState(std::make_unique<InGameState>());
//             }
//             else
//             {
//                 stateManager.setNotify(ConsoleUI::Red("[Sistem]: Pilihan karakter tidak valid!"));
//             }
//         }
//         catch (const std::invalid_argument &)
//         {
//             stateManager.setNotify(ConsoleUI::Red("[Sistem]: Input harus berupa angka atau huruf yang valid."));
//         }
//     }
// }