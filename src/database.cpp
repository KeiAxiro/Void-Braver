#include "../include/database.h"

// File ini sengaja jadi penghubung agar build tetap sederhana.
// Semua logic database dipindah ke file yang lebih kecil dan fokus.

#include "database/json_and_paths.cpp"
#include "database/player_serialization.cpp"
#include "database/save_system.cpp"
#include "database/lookups_and_balance.cpp"
