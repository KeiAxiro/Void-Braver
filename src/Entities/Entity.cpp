#include "Entities/Entity.h"
#include <algorithm>

using namespace std;
namespace Entities {
    // Konstruktor menggunakan member initializer list untuk efisiensi alokasi memori saat objek pertama kali dibuat
    Entity::Entity(const string& name, const Core::Stats& stats)
        : name(name), stats(stats) {}

    void Entity::TakeDamage(int damage) {
        // Mencegah nilai HP tembus menjadi negatif (underflow) menggunakan fungsi max dari std::algorithm
        stats.hp = max(0, stats.hp - damage);
    }

    bool Entity::IsDead() const {
        return stats.hp <= 0;
    }

    // Mengembalikan const reference (&) untuk menghindari overhead (biaya performa) dari copy data string di memori
    const string& Entity::GetName() const {
        return name;
    }

    // Getter bersifat read-only (const) untuk mengekspos data status secara aman tanpa risiko termodifikasi dari luar
    const Core::Stats& Entity::GetStats() const {
        return stats;
    }

    // Getter mutable yang mengembalikan reference asli, digunakan khusus saat butuh update/mutasi nilai status (misal: kalkulasi stat setelah naik level)
    Core::Stats& Entity::GetStatsMut() {
        return stats;
    }
}