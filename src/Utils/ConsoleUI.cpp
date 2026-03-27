#include "Utils/ConsoleUI.h"
#include <iostream>
#include <cstdlib> 

namespace Utils {
    void ConsoleUI::ClearScreen() {
        std::system("clear");
    }

    void ConsoleUI::Pause() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(10000, '\n');
        std::cin.get();
    }
}