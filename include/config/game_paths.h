#pragma once

// Semua path penting proyek dikumpulkan di satu tempat
// supaya pemula tidak perlu mencari string path di banyak file.
namespace game_paths
{
    inline constexpr const char *kGameDataPath = "data/game_data.json";
    inline constexpr const char *kSaveFilePath = "data/save.json";
} // namespace game_paths
