# 1. Batas versi minimum CMake yang dibutuhkan
cmake_minimum_required(VERSION 3.10)

# 2. Nama proyek game kamu dan bahasa yang dipakai (CXX = C++)
project(CLI_RPG_Game CXX)

# 3. Atur standar C++ (C++17 sangat disarankan agar pustaka JSON bekerja optimal)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. Beri tahu CMake di mana letak "Buku Menu" (file .h / .hpp)
# Agar file .cpp di folder manapun bisa saling mengenali satu sama lain
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/lib)

# 5. TRIK AJAIB UNTUK PEMULA: Kumpulkan semua file .cpp secara otomatis!
# Perintah ini akan memindai folder src/ beserta semua sub-foldernya (Core, Combat, dll)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 6. Rakit semuanya menjadi satu aplikasi (file .exe / executable) yang bisa dimainkan
add_executable(${PROJECT_NAME} ${SOURCES})