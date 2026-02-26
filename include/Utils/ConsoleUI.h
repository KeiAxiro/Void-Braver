#pragma once
#include <string>

class ConsoleUI
{
public:
    // Membersihkan layar terminal
    static void clearScreen();

    // Mewarnai teks menjadi Magenta (biasanya untuk barang langka/cerita)
    static std::string Magenta(const std::string &text);

    // Mewarnai teks menjadi Merah (untuk error/darah)
    static std::string Red(const std::string &text);

    // Mewarnai teks lainnya, berguna untuk status, info, peringatan, dll.
    static std::string Green(const std::string &text);
    static std::string Yellow(const std::string &text);
    static std::string Blue(const std::string &text);
    static std::string Cyan(const std::string &text);

    // Teks tebal/terang untuk judul atau penekanan
    static std::string Bold(const std::string &text);

    // Header sederhana (garis dan judul) untuk tampilan layar
    static void printHeader(const std::string &title);

    // Struktur data kecil untuk menyimpan informasi pengguna yang ingin ditampilkan
    struct UserInfo
    {
        std::string username;
        int level = 0;
        int hp = 0;
        int mp = 0;
    };

    // Cetak informasi dasar pengguna (username + beberapa atribut) di bagian atas layar
    static void printUserInfo(const UserInfo &user);

    // Mengembalikan warna ke standar (putih/abu-abu terminal)
    static void resetColor();
};