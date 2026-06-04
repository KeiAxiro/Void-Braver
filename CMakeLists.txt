cmake_minimum_required(VERSION 3.16)
project(VoidBraver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VOID_BRAVER_DATABASE_SOURCES
    src/database/json_and_paths.cpp
    src/database/player_serialization.cpp
    src/database/save_system.cpp
    src/database/data_lookup.cpp
    src/database/resource_balance.cpp
)

set(VOID_BRAVER_STATE_HELPER_SOURCES
    src/states/helpers/input_helpers.cpp
    src/states/helpers/inventory_helpers.cpp
    src/states/helpers/progress_helpers.cpp
    src/states/helpers/player_stats_helpers.cpp
    src/states/helpers/ui_helpers.cpp
)

set(VOID_BRAVER_STATE_SOURCES
    src/states/main_menu_state.cpp
    src/states/hub_state.cpp
    src/states/character_menu.cpp
    src/states/character_progression.cpp
    src/states/inventory_menu.cpp
    src/states/dungeon_menu.cpp
    src/states/enemy_factory.cpp
    src/states/item_catalog_features.cpp
    src/states/math_helpers.cpp
    src/states/crafting_menu.cpp
    src/states/shop_menu.cpp
    src/states/battle_helpers.cpp
    src/states/battle_effects.cpp
    src/states/battle_skills.cpp
    src/states/battle_state.cpp
    src/states/quest_menu.cpp
)

set(VOID_BRAVER_HEADER_FILES
    include/globals.h
    include/database.h
    include/state_helpers.h
    include/states.h
    include/database/database_detail.h
    include/config/game_paths.h
    include/config/game_rules.h
    include/config/player_balance.h
    include/config/enemy_balance.h
    include/config/skill_balance.h
    include/states/battle_types.h
    include/states/input_helpers.h
    include/states/inventory_helpers.h
    include/states/math_helpers.h
    include/states/progress_helpers.h
    include/states/player_stats_helpers.h
    include/states/ui_helpers.h
    include/states/character_features.h
    include/states/dungeon_features.h
    include/states/crafting_features.h
    include/states/shop_features.h
    include/states/battle_features.h
    include/utils/console_ui.h
)

add_library(void_braver_database STATIC
    ${VOID_BRAVER_DATABASE_SOURCES}
)

target_include_directories(void_braver_database
    PUBLIC
        include
        lib
)

add_library(void_braver_state_helpers STATIC
    ${VOID_BRAVER_STATE_HELPER_SOURCES}
)

target_include_directories(void_braver_state_helpers
    PUBLIC
        include
        lib
)

target_link_libraries(void_braver_state_helpers
    PUBLIC
        void_braver_database
)

add_library(void_braver_states STATIC
    ${VOID_BRAVER_STATE_SOURCES}
)

target_include_directories(void_braver_states
    PUBLIC
        include
        lib
)

target_link_libraries(void_braver_states
    PUBLIC
        void_braver_database
        void_braver_state_helpers
)

add_executable(VoidBraver
    src/main.cpp
    ${VOID_BRAVER_HEADER_FILES}
)

target_include_directories(VoidBraver
    PRIVATE
        include
        lib
)

target_link_libraries(VoidBraver
    PRIVATE
        void_braver_database
        void_braver_state_helpers
        void_braver_states
)

set_target_properties(VoidBraver PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
