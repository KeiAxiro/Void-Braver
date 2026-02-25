#include "ConsoleUI.h"
#include <iostream>

void ConsoleUI::clearScreen() {
    // Kombinasi ANSI terminal murni untuk membersihkan kanvas teks tanpa kedipan layar (flicker-free) [cite: 133, 134]
    std::cout << "\033[2J\033[1;1H"; 
}

void ConsoleUI::printMagenta(const std::string& text) {
    // \033[35m adalah kode warna Magenta/Ungu, sangat cocok untuk identitas prestise/harta karun [cite: 130]
    std::cout << "\033[35m" << text << "\033[0m"; // Selalu akhiri dengan \033[0m agar warna tidak bocor [cite: 131, 132]
}

void ConsoleUI::printRed(const std::string& text) {
    // \033[31m adalah kode warna Merah
    std::cout << "\033[31m" << text << "\033[0m";
}

void ConsoleUI::resetColor() {
    // Mengembalikan warna ke bawaan terminal
    std::cout << "\033[0m";
}

void ConsoleUI::pause() {
    std::cout << "\nTekan Enter untuk melanjutkan...";
    std::cin.ignore(10000, '\n'); 
}

void ConsoleUI::printHeader(const std::string& title) {
    clearScreen();
    std::cout << "=======================================\n";
    std::cout << "          " << title << "          \n";
    std::cout << "=======================================\n\n";
}