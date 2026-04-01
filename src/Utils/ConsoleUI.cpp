#include "Utils/ConsoleUI.h"
#include <iostream>
#include <cstdlib> 
using namespace std;

namespace Utils {
    void ConsoleUI::ClearScreen() {
        system("clear");
    }

    void ConsoleUI::Pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore(10000, '\n');
        cin.get();
    }
}