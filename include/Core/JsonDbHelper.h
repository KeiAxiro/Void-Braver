#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "lib/json.hpp"

using json = nlohmann::json;

template <typename T>
class JsonDbHelper
{
private:
    std::string dbFilePath;
    std::vector<T> dbCache; // Data sekarang disimpan sebagai Vector of Struct, bukan JSON mentah!

    void loadDatabase()
    {
        std::ifstream file(dbFilePath);
        // Cek apakah file bisa dibuka dan tidak kosong
        if (file.is_open() && file.peek() != std::ifstream::traits_type::eof())
        {
            try
            {
                json j;
                file >> j;
                // ✨ AJAIB: JSON array langsung berubah jadi vector of Struct C++
                dbCache = j.get<std::vector<T>>();
            }
            catch (const json::exception &e)
            {
                std::cerr << "JSON Error di " << dbFilePath << ": " << e.what() << "\n";
            }
        }
        else
        {
            // Kalau file gak ada/kosong, bikin file dengan array kosong []
            saveChanges();
        }
    }

public:
    explicit JsonDbHelper(const std::string &path) : dbFilePath(path)
    {
        loadDatabase();
    }

    // 1. Ambil semua data (Bisa untuk di-looping pakai for)
    std::vector<T> &getAll()
    {
        return dbCache;
    }

    // 2. Simpan item baru & otomatis tulis ke file
    void insert(const T &item)
    {
        dbCache.push_back(item);
        saveChanges();
    }

    // 3. Update file JSON (Panggil ini kalau ada data yang diedit)
    void saveChanges()
    {
        std::ofstream file(dbFilePath);
        if (file.is_open())
        {
            json j = dbCache; // ✨ AJAIB: Vector C++ langsung jadi JSON Array
            file << j.dump(4);
        }
    }

    // 4. Fitur Spesial: Cari 1 data pakai Lambda biar gampang di Controller
    template <typename Func>
    T *findFirst(Func condition)
    {
        auto it = std::find_if(dbCache.begin(), dbCache.end(), condition);
        if (it != dbCache.end())
        {
            return &(*it); // Mengembalikan pointer ke data yang ketemu
        }
        return nullptr; // Kalau gak ketemu
    }
};