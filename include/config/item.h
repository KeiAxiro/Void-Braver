#pragma once

namespace Config
{
    namespace Item
    {
        inline constexpr int EQUIPMENT_SPLIT_THRESHOLD = 1;      // Quantity di atas angka ini dipecah saat equip/unequip.
        inline constexpr int EQUIPMENT_TRANSFER_QUANTITY = 1;    // Jumlah item yang dipindah saat equip/unequip satu item.
        inline constexpr int CONSUMABLE_USE_QUANTITY = 1;        // Jumlah consumable yang habis setiap dipakai.
        inline constexpr int CONSUMABLE_HP_MIN_HEAL = 30;        // Heal HP minimum dari consumable HP.
        inline constexpr int CONSUMABLE_MP_MIN_HEAL = 20;        // Heal MP minimum dari consumable MP.
        inline constexpr int CONSUMABLE_HEAL_DIVISOR = 4;        // Pembagi max HP/MP untuk heal consumable.
        inline constexpr int CRAFT_RESULT_QUANTITY = 1;          // Jumlah item hasil crafting.
    }
}
