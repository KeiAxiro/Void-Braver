# Void Braver: Prelude to the Dark

CLI Text RPG berbasis C++17 untuk project UAS Struktur Data & Algoritma.

Metadata:

- Anggota 1: Keidjaru Axiro (2511446)
- Anggota 2: Keidjaru Axiro (2511446)
- Anggota 3: Keidjaru Axiro (2511446)
- Anggota 4: Keidjaru Axiro (2511446)
- Anggota 5: Keidjaru Axiro (2511446)

- Program Studi: Rekayasa Perangkat Lunak, Universitas Pendidikan Indonesia Kampus Cibiru

## Tujuan Refactor

Arsitektur project dibuat modular secukupnya: fitur yang mirip digabung dalam beberapa file general agar mudah dibaca, tetapi flow tetap terpisah dari `main.cpp`. Detail struktur ada di `docs.md`.

## Struktur Folder

```text
Void-Braver/
|- bin/
|  `- data/
|     |- game_data.json
|     `- save.json
|- include/
|  |- game.h
|  |- config.h
|  |- database.h
|  |- database_detail.h
|  |- globals.h
|  |- states.h
|  |- state_helpers.h
|  |- database/
|  |  |- json_handler.h
|  |  `- data_lookup.h
|  |- states/
|  |  |- menu_hub.h
|  |  |- character_inventory.h
|  |  `- battle_dungeon.h
|  `- utils/
|     `- console_ui.h
|- src/
|  |- game.cpp
|  |- database/
|  |  |- json_handler.cpp
|  |  `- data_lookup.cpp
|  |- states/
|  |  |- menu_hub.cpp
|  |  |- character_inventory.cpp
|  |  `- battle_dungeon.cpp
|  `- main.cpp
|- CMakeLists.txt
|- docs.md
`- README.md
```

## File Penting

- `include/game.h`: deklarasi flow aplikasi utama.
- `include/config.h`: aggregator path, rule, dan angka balance.
- `include/globals.h`: enum, struct runtime, dan `GameContext`.
- `include/database.h`: aggregator header database.
- `include/states.h`: aggregator header state.
- `include/state_helpers.h`: deklarasi helper internal antar modul state.
- `src/game.cpp`: load data, loop state, dispatch state, dan save saat keluar.
- `src/database/json_handler.cpp`: path, JSON, serialization, save/load, dan create new game.
- `src/database/data_lookup.cpp`: lookup ID, kategori item, formula EXP, HP/MP, dan bonus equipment.
- `src/states/menu_hub.cpp`: main menu, hub, input helper, progress helper, UI helper, dan stat helper.
- `src/states/character_inventory.cpp`: character menu, stat allocation, skill list, inventory, crafting, dan shop.
- `src/states/battle_dungeon.cpp`: dungeon selection, enemy factory, exploration, battle loop, reward, dan skill battle.

## Save File

Game tetap memakai:

```text
bin/data/save.json
```

Path default di `Config::Paths::SAVE_FILE` adalah `data/save.json`, jadi executable paling aman dijalankan dari folder `bin`.

Save tetap kompatibel dengan format lama, tetapi sekarang ikut menyimpan `class_tier_id` dan `class_tier_name` agar tier class yang terbuka tidak hanya tampil sementara di UI.

## Build

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
cmake --build build
```

Output executable:

```text
bin/VoidBraver.exe
```

Run:

```bash
cd bin
./VoidBraver.exe
```

## Implementasi SDA Manual

Fase 3 sudah mengganti struktur runtime yang dinilai ke Node/pointer manual:

- `ManualList<T>`: double linked list untuk inventory, cooldown, character list, progress dungeon, drop enemy, kategori shop/crafting, dan daftar pointer JSON sementara.
- `StateStack`: stack manual untuk state game.
- `EncounterQueue`: queue manual untuk jadwal encounter saat eksplorasi dungeon.
- `SkillTreeNode`: binary tree untuk skill list, traversal InOrder, search skill, clear tree, dan auto unlock berdasarkan level.
- `DungeonGraphVertex` / `DungeonGraphEdge`: adjacency list manual untuk DFS unlock final dungeon.
- `DungeonGraphMatrix`: adjacency matrix manual untuk Dijkstra dan Kruskal ringan pada rute dungeon.

Container STL untuk struktur data runtime tidak dipakai.

## Fase 4: Progression & UI

- Tier class disinkronkan saat new game, load, level up, refresh resource, dan save.
- Unlock depth dungeon memakai progres clear dan minimum level depth; level yang sudah melewati rekomendasi tidak lagi terkunci oleh batas maksimum range.
- Final dungeon tetap memakai graph manual: `Tahta Kehampaan` baru terbuka setelah semua dungeon non-final selesai.
- Header console memakai box ASCII dan helper ANSI selalu menutup warna dengan reset code.
