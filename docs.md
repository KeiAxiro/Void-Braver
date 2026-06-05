# Dokumentasi Struktur Void Braver

Struktur ini dibuat modular secukupnya: file tidak ditumpuk semua di `main.cpp`, tetapi juga tidak dipecah terlalu banyak. Pembagian mengikuti fitur yang kegunaannya mirip agar mudah dicari oleh programmer pemula.

Refactor ini tidak mengubah game logic, rumus, data JSON, output menu, atau perilaku gameplay.

## Flow Program

```text
src/main.cpp
  -> startGame()
     -> loadGameData()
     -> runGame()
        -> runMainMenu()
        -> runHub()
        -> runBattle()
     -> saveGame()
```

`main.cpp` hanya menjadi pintu masuk. Flow aplikasi ada di `src/game.cpp`.

## Struktur File

```text
Void-Braver/
|- include/
|  |- game.h
|  |- globals.h
|  |- database.h
|  |- database_detail.h
|  |- state_helpers.h
|  |- states.h
|  |- database/
|  |  |- json_handler.h
|  |  `- data_lookup.h
|  |- states/
|  |  |- menu_hub.h
|  |  |- character_inventory.h
|  |  `- battle_dungeon.h
|  |- config.h
|  |- config/
|  `- utils/
|     `- console_ui.h
|- src/
|  |- main.cpp
|  |- game.cpp
|  |- database/
|  |  |- json_handler.cpp
|  |  `- data_lookup.cpp
|  `- states/
|     |- menu_hub.cpp
|     |- character_inventory.cpp
|     `- battle_dungeon.cpp
|- CMakeLists.txt
`- docs.md
```

## Modul Utama

### `src/main.cpp` dan `src/game.cpp`

`main.cpp` memanggil `startGame()`. `game.cpp` mengatur loop utama, dispatch state, load data awal, dan save saat keluar.

### `src/database/json_handler.cpp`

Mengurus data masuk/keluar:

- helper JSON dan path;
- serialization/deserialization player;
- load game data;
- load save;
- save game;
- create new game.

Header modulnya: `include/database/json_handler.h`.

### `src/database/data_lookup.cpp`

Mengurus pembacaan data master dan formula resource:

- lookup class, item, skill, dungeon, enemy;
- lookup depth dan dialog;
- kategori item dan slot equipment;
- EXP next level;
- bonus equipment;
- HP/MP maksimum;
- sync class tier;
- normalize resource player.

Header modulnya: `include/database/data_lookup.h`.

### `src/states/menu_hub.cpp`

Mengurus flow menu umum dan helper yang dipakai banyak state:

- input helper;
- inventory helper dasar;
- progress dungeon;
- stat player;
- UI header/detail;
- main menu;
- hub menu.

Header modulnya: `include/states/menu_hub.h`.

### `src/states/character_inventory.cpp`

Mengurus fitur karakter dan item:

- character menu;
- allocate stat;
- skill list;
- inventory;
- equipment;
- consumable;
- crafting;
- shop.

Header modulnya: `include/states/character_inventory.h`.

### `src/states/battle_dungeon.cpp`

Mengurus dungeon dan battle:

- pilih dungeon/depth;
- lore dungeon;
- enemy factory;
- dungeon map;
- encounter;
- battle loop;
- battle skill;
- effect dan reward.

Header modulnya: `include/states/battle_dungeon.h`.

## Header Aggregator

### `include/database.h`

Aggregator untuk database:

```cpp
#include "database/json_handler.h"
#include "database/data_lookup.h"
```

Gunakan ini jika file butuh akses umum ke database tanpa memilih header spesifik.

### `include/states.h`

Aggregator untuk state:

```cpp
#include "states/menu_hub.h"
#include "states/character_inventory.h"
#include "states/battle_dungeon.h"
```

Gunakan ini jika file butuh entry point state game.

### `include/state_helpers.h`

Kontrak helper internal antar modul state. Isinya deklarasi struct dan helper yang dipakai lintas `menu_hub.cpp`, `character_inventory.cpp`, dan `battle_dungeon.cpp`.

## Aturan Menambah Fitur

- Fitur load/save/JSON masuk ke `json_handler.cpp`.
- Fitur lookup data atau formula resource masuk ke `data_lookup.cpp`.
- Fitur main menu, hub, UI umum, progress, dan helper umum masuk ke `menu_hub.cpp`.
- Fitur character, inventory, skill list, crafting, dan shop masuk ke `character_inventory.cpp`.
- Fitur dungeon, enemy, battle, skill combat, dan reward masuk ke `battle_dungeon.cpp`.
- Kalau fungsi dipakai lintas modul state, deklarasikan di `state_helpers.h`.
- Kalau fungsi database dipakai file lain, deklarasikan di header `include/database/`.
- Kalau menambah `.cpp`, tambahkan ke `VOID_BRAVER_SOURCES` di `CMakeLists.txt`.

## Catatan Build

Build tetap memakai:

```bash
cmake -S . -B build
cmake --build build
```

Executable diarahkan ke folder `bin/`.
