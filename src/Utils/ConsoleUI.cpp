#include "ConsoleUI.h"
#include <iostream>

// helper internal untuk memudahkan pencetakan dengan kode ANSI apapun
static std::string WithCode(const std::string& code, const std::string& text) {
    return code + text + "\033[0m";
}

void ConsoleUI::clearScreen() {
    // Kombinasi ANSI terminal murni untuk membersihkan kanvas teks tanpa kedipan layar (flicker-free) [cite: 133, 134]
    std::cout << "\033[2J\033[1;1H"; 
}

std::string ConsoleUI::Magenta(const std::string& text) {
    // \033[35m adalah kode warna Magenta/Ungu, sangat cocok untuk identitas prestise/harta karun [cite: 130]
    return WithCode("\033[35m", text);
}

std::string ConsoleUI::Red(const std::string& text) {
    // \033[31m adalah kode warna Merah
    return WithCode("\033[31m", text);
}

std::string ConsoleUI::Green(const std::string& text) {
    // Kode 32m hijau, sering untuk pesan sukses atau status sehat
    return WithCode("\033[32m", text);
}

std::string ConsoleUI::Yellow(const std::string& text) {
    // Kode 33m kuning, biasanya untuk peringatan
    return WithCode("\033[33m", text);
}

std::string ConsoleUI::Blue(const std::string& text) {
    // Kode 34m biru, untuk informasi umum
    return WithCode("\033[34m", text);
}

std::string ConsoleUI::Cyan(const std::string& text) {
    // Kode 36m cyan, sering digunakan untuk teks netral
    return WithCode("\033[36m", text);
}

std::string ConsoleUI::Bold(const std::string& text) {
    // Bold/bright; bukan semua terminal mendukung, tapi banyak yang ya
    return WithCode("\033[1m", text);
}

void ConsoleUI::printHeader(const std::string& title) {
    // Cetak judul besar dengan garis di bawahnya
    Bold("=== " + title + " ===\n");
}

void ConsoleUI::printUserInfo(const ConsoleUI::UserInfo& user) {
    // Tampilkan baris informasi pengguna singkat
    // contoh: [Player: alice | Lv.5 | HP: 120/120 | MP: 30/30]
    std::cout << "[Player: " << user.username
              << " | Lv." << user.level
              << " | HP: " << user.hp
              << " | MP: " << user.mp
              << "]\n";
}

void ConsoleUI::resetColor() {
    // Mengembalikan warna ke bawaan terminal
    std::cout << "\033[0m";
}