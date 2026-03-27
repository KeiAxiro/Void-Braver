cmake_minimum_required(VERSION 3.10)
project(VoidBraver)

# Gunakan C++17 (Disarankan karena fitur-fitur modern dan library nlohmann::json)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Otomatis mencari SEMUA file .cpp di dalam folder src dan subfoldernya
# (Jadi kamu nggak perlu nambahin manual ke sini tiap kali bikin file .cpp baru)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 3. Tambahkan source code SQLite dari folder lib
set(SQLITE_SRC "lib/sqlite/sqlite3.c")

# Bikin file executable (VoidBraver.exe)
add_executable(${PROJECT_NAME} ${SOURCES} ${SQLITE_SRC})

# 🔥 BAGIAN PALING PENTING 🔥
# Beritahu compiler di mana letak folder "include" kita.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 🔥 TAMBAHKAN BARIS INI UNTUK SQLITE3 🔥
# Hubungkan library SQLite ke game agar fungsi database tidak error
target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3)