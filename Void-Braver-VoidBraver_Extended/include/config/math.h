#pragma once

namespace Config
{
    namespace Math
    {
        inline constexpr int ZERO = 0;                         // Nilai nol umum untuk counter, clamp bawah, dan fallback angka.
        inline constexpr int ONE = 1;                          // Nilai satu umum untuk jumlah minimal dan konversi indeks.
        inline constexpr int INDEX_OFFSET = 1;                 // Offset dari pilihan UI berbasis 1 ke array berbasis 0.
        inline constexpr double ZERO_RATIO = 0.0;              // Rasio nol saat rentang kalkulasi tidak valid.
        inline constexpr double UNIT_ROLL_MIN = 0.0;           // Nilai minimum roll acak pecahan 0..1.
        inline constexpr double UNIT_ROLL_MAX = 1.0;           // Nilai maksimum roll acak pecahan 0..1.
        inline constexpr int PERCENT_ROLL_MIN = 1;             // Nilai minimum roll persen inklusif.
        inline constexpr int PERCENT_ROLL_MAX = 100;           // Nilai maksimum roll persen inklusif.
        inline constexpr int COIN_FLIP_MIN = 0;                // Nilai minimum random dua pilihan.
        inline constexpr int COIN_FLIP_MAX = 1;                // Nilai maksimum random dua pilihan.
        inline constexpr int COIN_FLIP_LEFT_VALUE = 0;         // Nilai random yang dianggap memilih sisi kiri.
        inline constexpr int MIN_INTERPOLATED_STAT = 1;        // Nilai minimal hasil interpolasi stat musuh.
    }
}
