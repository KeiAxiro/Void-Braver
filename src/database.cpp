// Legacy aggregator:
// file ini dipertahankan agar command build manual lama masih bisa dipakai.
// CMake modern proyek ini tidak lagi mengandalkan file ini sebagai sumber utama.

#include "database.h"

#include "database/json_and_paths.cpp"
#include "database/player_serialization.cpp"
#include "database/save_system.cpp"
#include "database/lookups_and_balance.cpp"
