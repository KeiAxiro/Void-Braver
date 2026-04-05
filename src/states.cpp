// Legacy aggregator:
// file ini dipertahankan agar command build manual lama masih bisa dipakai.
// CMake modern proyek ini mengompilasi setiap file state secara terpisah.

#include "states/common_helpers.cpp"
#include "states/character_menu.cpp"
#include "states/character_progression.cpp"
#include "states/inventory_menu.cpp"
#include "states/math_helpers.cpp"
#include "states/dungeon_menu.cpp"
#include "states/enemy_factory.cpp"
#include "states/item_catalog_features.cpp"
#include "states/crafting_menu.cpp"
#include "states/shop_menu.cpp"
#include "states/battle_helpers.cpp"
#include "states/battle_effects.cpp"
#include "states/battle_skills.cpp"
#include "states/main_menu_state.cpp"
#include "states/hub_state.cpp"
#include "states/battle_state.cpp"
