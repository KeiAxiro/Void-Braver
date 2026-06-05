#pragma once

namespace Config
{
    namespace Progress
    {
        inline constexpr int START_DEPTH = 1;                              // Depth awal dungeon.
        inline constexpr int NO_DEPTH_CLEARED = 0;                         // Nilai awal depth yang belum pernah diselesaikan.
        inline constexpr int NO_DEPTH_UNLOCKED = 0;                        // Nilai awal max depth global yang belum terbuka.
        inline constexpr int DEPTH_UNLOCK_STEP = 1;                        // Tambahan depth yang terbuka setelah clear.
        inline constexpr int LEVEL_RANGE_MIN_FALLBACK = 1;                 // Min level fallback jika data depth kosong.
        inline constexpr int LEVEL_RANGE_MAX_FALLBACK = 100;               // Max level fallback jika data depth kosong.
        inline constexpr int MISSING_LEVEL = -1;                           // Sentinel level saat data balance tidak ditemukan.
        inline constexpr const char *FINAL_DUNGEON_ID = "tahta_kehampaan"; // ID dungeon final yang dikunci oleh progress.
        inline constexpr int GRAPH_MAX_NODES = 16;                         // Kapasitas node graph dungeon.
        inline constexpr int ROUTE_INFINITY = 1000000;                     // Nilai jarak tak hingga untuk Dijkstra.
        inline constexpr int MIN_ROUTE_WEIGHT = 1;                         // Bobot rute minimal antar dungeon.
    }
}
