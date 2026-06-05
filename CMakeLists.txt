cmake_minimum_required(VERSION 3.16)
project(VoidBraver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VOID_BRAVER_SOURCES
    src/main.cpp
    src/game.cpp
    src/database/json_handler.cpp
    src/database/data_lookup.cpp
    src/states/menu_hub.cpp
    src/states/character_inventory.cpp
    src/states/battle_dungeon.cpp
)

set(VOID_BRAVER_HEADERS
    include/game.h
    include/config.h
    include/config/paths.h
    include/config/math.h
    include/config/rules.h
    include/config/defaults.h
    include/config/progress.h
    include/config/starter_kit.h
    include/config/player.h
    include/config/enemy.h
    include/config/item.h
    include/config/dungeon_map.h
    include/config/ui.h
    include/config/skill.h
    include/config/legacy.h
    include/globals.h
    include/database.h
    include/database/json_handler.h
    include/database/data_lookup.h
    include/database_detail.h
    include/states.h
    include/states/menu_hub.h
    include/states/character_inventory.h
    include/states/battle_dungeon.h
    include/state_helpers.h
    include/utils/console_ui.h
)

add_executable(VoidBraver
    ${VOID_BRAVER_SOURCES}
    ${VOID_BRAVER_HEADERS}
)

target_include_directories(VoidBraver
    PRIVATE
        include
        lib
)

set_target_properties(VoidBraver PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
