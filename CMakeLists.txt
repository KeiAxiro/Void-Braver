cmake_minimum_required(VERSION 3.10)
project(Void-Braver)

# Gunakan C++17 (Disarankan karena fitur-fitur modern dan library nlohmann::json)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Otomatis mencari SEMUA file .cpp di dalam folder src dan subfoldernya
# (Jadi kamu nggak perlu nambahin manual ke sini tiap kali bikin file .cpp baru)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Bikin file executable (Void-Braver.exe)
add_executable(${PROJECT_NAME} ${SOURCES})

# 🔥 BAGIAN PALING PENTING 🔥
# Beritahu compiler di mana letak folder "include" kita.
# Berkat baris ini, kamu bisa langsung ketik: #include "Core/AccountManager.h" 
# di file .cpp manapun tanpa pusing mikirin "../" atau "../../"
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)