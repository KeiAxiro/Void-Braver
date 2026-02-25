#pragma once
#include <string>

class ConsoleUI {
public:
    // Membersihkan layar terminal
    static void clearScreen();
    
    // Mewarnai teks menjadi Magenta (biasanya untuk barang langka/cerita)
    static void printMagenta(const std::string& text);
    
    // Mewarnai teks menjadi Merah (untuk error/darah)
    static void printRed(const std::string& text);
    
    // Mengembalikan warna ke standar (putih/abu-abu terminal)
    static void resetColor();
};