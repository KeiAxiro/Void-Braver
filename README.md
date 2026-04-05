# VoidBraver

CLI RPG berbasis C++17 dan `nlohmann::json`.

Proyek ini dirancang supaya:

- data game ada di JSON
- logika game ada di C++
- balancing mudah diubah dari header config
- struktur file mudah dibaca programmer pemula
- fitur baru bisa ditambah tanpa merusak flow lama

README ini fokus ke 4 hal:

1. bagaimana alur program berjalan
2. file mana mengerjakan apa
3. fungsi mana sebaiknya kamu buka saat ingin mengubah fitur tertentu
4. bagaimana menambah fitur baru dengan aman

---

## 1. Gambaran Besar

Secara mental, proyek ini punya 6 lapisan:

1. `bin/data/game_data.json`
   Ini adalah sumber data konten game: class, skill, item, dungeon, enemy, exp table, dan sebagainya.

2. `bin/data/save.json`
   Ini adalah save multi-character.

3. `include/globals.h`
   Ini adalah model data runtime game seperti `Player`, `Progress`, dan `GameContext`.

4. `include/config/*.h`
   Ini pusat angka balance dan path default. Kalau kamu mau rebalance game, biasanya mulai dari sini.

5. `src/database/*.cpp`
   Ini lapisan service/database: load JSON, save JSON, lookup item/class/enemy, hitung resource dasar player.

6. `src/states/*.cpp`
   Ini lapisan flow game: menu, hub, dungeon, battle, crafting, shop, character menu.

Kalau dianalogikan:

- `game_data.json` = database konten
- `save.json` = database save
- `GameContext` = RAM saat game sedang berjalan

---

## 2. Teknologi

- Bahasa: C++17
- Library JSON: `nlohmann::json`
- Build system utama: CMake
- Build manual legacy: `g++`

---

## 3. Struktur Folder

```text
Optimized-Void-Braver/
|- bin/
|  |- data/
|  |  |- game_data.json
|  |  `- save.json
|- include/
|  |- config/
|  |  |- enemy_balance.h
|  |  |- game_paths.h
|  |  |- game_rules.h
|  |  |- player_balance.h
|  |  `- skill_balance.h
|  |- database/
|  |  `- database_detail.h
|  |- states/
|  |  |- battle_features.h
|  |  |- battle_types.h
|  |  |- character_features.h
|  |  |- crafting_features.h
|  |  |- dungeon_features.h
|  |  |- input_helpers.h
|  |  |- inventory_helpers.h
|  |  |- math_helpers.h
|  |  |- player_stats_helpers.h
|  |  |- progress_helpers.h
|  |  |- shop_features.h
|  |  `- ui_helpers.h
|  |- utils/
|  |  `- console_ui.h
|  |- database.h
|  |- globals.h
|  |- state_helpers.h
|  `- states.h
|- src/
|  |- database/
|  |  |- data_lookup.cpp
|  |  |- json_and_paths.cpp
|  |  |- lookups_and_balance.cpp
|  |  |- player_serialization.cpp
|  |  |- resource_balance.cpp
|  |  `- save_system.cpp
|  |- states/
|  |  |- battle_effects.cpp
|  |  |- battle_helpers.cpp
|  |  |- battle_skills.cpp
|  |  |- battle_state.cpp
|  |  |- character_menu.cpp
|  |  |- character_progression.cpp
|  |  |- common_helpers.cpp
|  |  |- crafting_menu.cpp
|  |  |- dungeon_menu.cpp
|  |  |- enemy_factory.cpp
|  |  |- hub_state.cpp
|  |  |- inventory_menu.cpp
|  |  |- item_catalog_features.cpp
|  |  |- main_menu_state.cpp
|  |  |- math_helpers.cpp
|  |  |- shop_menu.cpp
|  |  `- helpers/
|  |     |- input_helpers.cpp
|  |     |- inventory_helpers.cpp
|  |     |- player_stats_helpers.cpp
|  |     |- progress_helpers.cpp
|  |     `- ui_helpers.cpp
|  |- database.cpp
|  |- main.cpp
|  `- states.cpp
|- CMakeLists.txt
`- README.md
```

Prinsip pembagiannya:

- `include/` untuk deklarasi
- `src/` untuk implementasi
- `include/config/` untuk angka balance dan path default
- `src/database/` untuk semua urusan JSON/save/load/lookup
- `src/states/helpers/` untuk helper kecil lintas fitur
- `src/states/` untuk flow fitur yang dilihat player
- `src/database.cpp`, `src/states.cpp`, `src/states/common_helpers.cpp` dipertahankan sebagai file legacy aggregator untuk build manual lama

---

## 4. Build dan Run

### Build dengan CMake

```bash
cmake -S . -B build-gcc -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
cmake --build build-gcc
```

Output executable utama:

```text
bin/VoidBraver.exe
```

### Build manual legacy

Masih bisa dipakai, tetapi ini bukan jalur yang direkomendasikan untuk pengembangan harian:

```bash
g++ -std=c++17 -Iinclude -Ilib src/main.cpp src/database.cpp src/states.cpp -o bin/VoidBraver.exe
```

### Menjalankan game

Karena path default di `include/config/game_paths.h` adalah:

- `data/game_data.json`
- `data/save.json`

maka executable paling aman dijalankan dari folder `bin`.

```bash
cd bin
./VoidBraver.exe
```

### Kenapa editor kadang merah padahal build aman?

Biasanya penyebabnya:

- editor belum membaca `compile_commands.json`
- file `.cpp` lama memakai include aggregator
- executable lama sedang terkunci sehingga build CMake berhenti di tahap akhir

Supaya editor lebih akurat:

1. jalankan `cmake -S . -B build-gcc`
2. pastikan editor membaca `build-gcc/compile_commands.json`
3. lebihkan memakai CMake daripada build manual legacy

---

## 5. Struktur CMake Sekarang

Struktur CMake proyek ini sekarang modular:

- `void_braver_database`
- `void_braver_state_helpers`
- `void_braver_states`
- `VoidBraver`

Artinya:

- modul database punya target sendiri
- helper state punya target sendiri
- state utama punya target sendiri
- executable hanya menjadi titik masuk yang menghubungkan semuanya

Manfaatnya:

- dependency antar bagian lebih jelas
- editor lebih mudah memahami deklarasi dan implementasi
- file baru lebih mudah ditambahkan
- compile error lebih mudah dilacak

---

## 6. Alur Program dari Awal Sampai Akhir

### 6.1 `main()`

File: `src/main.cpp`

Flow:

1. buat `GameContext ctx`
2. load `game_data.json`
3. jalankan game loop berdasarkan `ctx.stateStack`
4. saat keluar, simpan jika perlu

### 6.2 Main menu

File: `src/states/main_menu_state.cpp`

Flow:

1. tampilkan `New Game`, `Load Game`, `Exit`
2. kalau `New Game`, player pilih nama dan class
3. `createNewGame(...)` membuat player baru
4. save dilakukan
5. state masuk ke `Hub`

### 6.3 Hub

File: `src/states/hub_state.cpp`

Hub adalah pusat navigasi:

- Character Menu
- Inventory
- Enter Dungeon
- Crafting
- Shop
- Save
- Kembali ke Main Menu

### 6.4 Character Menu

File utama:

- `src/states/character_menu.cpp`
- `src/states/character_progression.cpp`
- `src/states/inventory_menu.cpp`

Flow:

- `character_menu.cpp` hanya mengatur menu utama character
- `character_progression.cpp` mengatur alokasi stat dan daftar skill
- `inventory_menu.cpp` mengatur equip, unequip, consume, dan pagination inventory

### 6.5 Dungeon

File utama:

- `src/states/dungeon_menu.cpp`
- `src/states/enemy_factory.cpp`
- `src/states/battle_state.cpp`

Flow:

1. player pilih dungeon
2. player pilih depth yang terbuka
3. game validasi level range
4. state `Battle` dimasukkan ke stack
5. player melangkah
6. encounter bisa muncul di tengah langkah

### 6.6 Battle

File utama:

- `src/states/battle_state.cpp`
- `src/states/battle_helpers.cpp`
- `src/states/battle_effects.cpp`
- `src/states/battle_skills.cpp`

Pembagiannya:

- `battle_state.cpp` = flow eksplorasi level atas
- `battle_helpers.cpp` = loop battle inti
- `battle_effects.cpp` = damage musuh, reward, progress, tick efek
- `battle_skills.cpp` = UI skill dan efek skill

### 6.7 Crafting dan Shop

File utama:

- `src/states/crafting_menu.cpp`
- `src/states/shop_menu.cpp`
- `src/states/item_catalog_features.cpp`

Pembagiannya:

- `crafting_menu.cpp` = flow crafting
- `shop_menu.cpp` = flow beli/jual
- `item_catalog_features.cpp` = helper kategori, resep, dan filter item lintas fitur

---

## 7. Pusat Runtime: `GameContext`

`GameContext` adalah objek yang paling penting di proyek ini.

Hampir semua fungsi besar menerima:

```cpp
GameContext &ctx
```

Field penting:

- `gameData`: semua data dari `game_data.json`
- `player`: character aktif
- `characters`: daftar character di save
- `activeCharacterIndex`: index character aktif
- `stateStack`: stack state
- `gameDataPath`: path data game
- `savePath`: path save
- `running`: penanda game loop masih jalan

Kalau bingung "data sekarang sedang ada di mana?", jawabannya biasanya ada di `ctx`.

---

## 8. Pusat Balancing

Kalau tujuanmu adalah rebalance game, urutan file yang paling penting sekarang:

1. `include/config/player_balance.h`
2. `include/config/enemy_balance.h`
3. `include/config/skill_balance.h`
4. `include/config/game_rules.h`
5. `src/database/resource_balance.cpp`
6. `src/states/helpers/player_stats_helpers.cpp`
7. `src/states/battle_effects.cpp`
8. `src/states/battle_skills.cpp`

Artinya kamu tidak perlu lagi memburu angka acak di terlalu banyak tempat.

Contoh:

- base HP/MP player ada di `player_balance.h`
- rumus ATK/DEF player ada di `player_stats_helpers.cpp`
- rumus damage musuh ada di `battle_effects.cpp`
- multiplier skill ada di `skill_balance.h`
- jumlah langkah dungeon ada di `game_rules.h`

---

## 9. Cara Berpikir Saat Ngoding di Proyek Ini

Urutan berpikir yang disarankan:

1. tentukan fitur ini termasuk data, logic, atau UI
2. kalau hanya konten, ubah JSON
3. kalau hanya angka balance, cek `include/config/`
4. kalau flow menu berubah, cek `src/states/`
5. kalau rumus kecil berubah, cek `src/states/helpers/` atau `src/database/resource_balance.cpp`
6. kalau fitur menyentuh save, jangan lupa serialisasi

Aturan praktis:

- data konten = JSON
- aturan permainan = C++
- satu file = satu tanggung jawab utama
- file besar dipecah berdasarkan topik, bukan dipotong acak

---

## 10. Cara Menambah Fitur Baru

### 10.1 Menambah item baru

Langkah:

1. tambah item di `bin/data/game_data.json`
2. isi field seperti `id`, `category`, `name`, `description`, `stats`
3. jika craftable, isi `crafting.ingredients`
4. jika shopable, pastikan `buy_price > 0`

Kalau format item tetap sama, biasanya tidak perlu ubah C++.

### 10.2 Menambah skill baru

Langkah:

1. tambah skill di class pada `game_data.json`
2. UI skill otomatis bisa membaca datanya
3. implementasikan efek skill di `src/states/battle_skills.cpp`

Kalau kamu hanya menambah data tanpa implementasi efek, skill akan tampil tetapi perilaku spesialnya belum ada.

### 10.3 Menambah dungeon baru

Langkah:

1. tambah dungeon di `game_data.json`
2. isi `depths`
3. pastikan ada enemy yang punya `spawn_depth` cocok
4. jika format data benar, menu dungeon biasanya langsung bisa membaca dungeon baru

### 10.4 Menambah menu baru di hub

Langkah:

1. ubah `src/states/hub_state.cpp`
2. buat file state baru, misalnya `src/states/quest_menu.cpp`
3. buat header deklarasinya di `include/states/`
4. tambahkan source baru itu ke `CMakeLists.txt`

Pola penting:

- deklarasi di `include/`
- implementasi di `src/`
- daftarkan file baru ke target CMake yang sesuai

### 10.5 Menambah field save baru

Contoh: `quest_progress`

Langkah:

1. tambah field ke `Player` atau `Progress`
2. serialisasi di `playerToJson(...)`
3. deserialisasi di `playerFromJson(...)`
4. beri fallback aman untuk save lama

---

## 11. Peta File per File

Bagian ini adalah peta teknis utama proyek.

### 11.1 Header inti

#### `include/globals.h`

Isi:

- enum `GameState`
- struct `Stats`
- struct `InventoryEntry`
- struct `CooldownEntry`
- struct `Progress`
- struct `Player`
- struct `GameContext`
- konstanta alias seperti `STARTING_GOLD`, `PAGE_SIZE`
- helper `clampInt(...)`

Kapan dibuka:

- saat mau menambah field player/save
- saat mau memahami bentuk data runtime

#### `include/database.h`

Berisi deklarasi fungsi public database/service.

Fungsi penting:

- `loadGameData(...)`
- `loadSave(...)`
- `loadCharacter(...)`
- `saveGame(...)`
- `createNewGame(...)`
- semua fungsi lookup item/class/enemy/dungeon
- semua fungsi base/effective HP/MP

#### `include/database/database_detail.h`

Berisi helper internal database lintas file.

Fungsi penting:

- `asInt(...)`
- `asString(...)`
- `fileExists(...)`
- `resolveGameDataPath(...)`
- `resolveSavePath(...)`
- `sanitizeIdPart(...)`
- `equippedItemBonus(...)`
- `addStarterItem(...)`
- `giveStarterKit(...)`
- `playerToJson(...)`
- `playerFromJson(...)`
- `playerFromLegacyJson(...)`
- `upsertCharacter(...)`

Catatan:

Kalau editor pernah memberi error seperti `identifier "asInt" is undefined`, biasanya masalahnya bukan di build, tetapi di editor yang belum membaca header internal atau `compile_commands.json`.

#### `include/states.h`

Berisi state entry utama:

- `runMainMenu(...)`
- `runHub(...)`
- `runBattle(...)`

#### `include/state_helpers.h`

Ini adalah umbrella header legacy.

Fungsinya:

- memudahkan file aggregator lama
- menggabungkan banyak header state untuk build manual legacy

Untuk file baru, lebih baik include header yang spesifik dari `include/states/`.

### 11.2 Header config

#### `include/config/game_paths.h`

Berisi path default:

- `data/game_data.json`
- `data/save.json`

#### `include/config/game_rules.h`

Berisi aturan global seperti:

- jumlah langkah depth
- page size
- starting gold
- starting stat points

#### `include/config/player_balance.h`

Berisi angka balance player:

- base HP/MP
- growth per level
- divisor stat
- buff multiplier
- crit base
- level up multiplier

#### `include/config/enemy_balance.h`

Berisi angka balance enemy:

- fallback musuh
- multiplier boss
- jitter damage
- reward battle
- reward clear depth

#### `include/config/skill_balance.h`

Berisi multiplier skill dan durasi efek skill.

### 11.3 Header state modular

#### `include/states/input_helpers.h`

Fungsi:

- `randInt(...)`
- `randUnit()`
- `trim(...)`
- `toLower(...)`
- `readLine()`
- `tryParseInt(...)`
- `getPrimaryStatValue(...)`

#### `include/states/inventory_helpers.h`

Fungsi:

- `inventoryCount(...)`
- `mergeInventory(...)`
- `addItem(...)`
- `removeItem(...)`
- `itemBonusFromEquipped(...)`
- `getCooldown(...)`
- `setCooldown(...)`
- `reduceCooldowns(...)`

#### `include/states/math_helpers.h`

Fungsi:

- `normalizedRatio(...)`
- `interpolateStat(...)`

Ini sengaja dipisah supaya rumus interpolasi tidak menempel ke character menu.

#### `include/states/player_stats_helpers.h`

Fungsi:

- `calculatePlayerAttack(...)`
- `calculatePlayerDefense(...)`
- `calculatePlayerSpeed(...)`
- `calculatePlayerCritRate(...)`
- `refreshPlayerResources(...)`
- `levelUpIfNeeded(...)`

#### `include/states/progress_helpers.h`

Fungsi:

- `ensureDungeonProgress(...)`
- `getDungeonUnlockedDepth(...)`
- `isDungeonCompleted(...)`
- `dungeonMaxDepth(...)`
- `isFinalDungeonLocked(...)`

#### `include/states/ui_helpers.h`

Fungsi:

- `printHubHeader(...)`
- `printStateHeader(...)`
- `showItemDetails(...)`
- `unlockedTierName(...)`
- `printClassDetails(...)`
- `itemDisplayName(...)`
- `showCharacterOverview(...)`

#### `include/states/character_features.h`

Fungsi:

- `allocateStats(...)`
- `equipInventoryEntry(...)`
- `unequipInventoryEntry(...)`
- `useConsumable(...)`
- `inventoryMenu(...)`
- `skillMenu(...)`
- `characterMenu(...)`

#### `include/states/dungeon_features.h`

Fungsi:

- `spawnEnemyForDepth(...)`
- `isDepthLevelAllowed(...)`
- `printDungeonLore(...)`
- `selectDungeonDepth(...)`
- `chooseDungeon(...)`
- `canCraftItem(...)`
- `ownedIngredientCount(...)`
- `totalIngredientNeed(...)`
- `collectCraftingCategories(...)`
- `categoriesForShopMode(...)`
- `showRecipeDetail(...)`

#### `include/states/crafting_features.h`

Fungsi:

- `craftingMenu(...)`

#### `include/states/shop_features.h`

Fungsi:

- `shopMenu(...)`

#### `include/states/battle_features.h`

Fungsi:

- `calculateEnemySpeed(...)`
- `enemyGetsFirstTurn(...)`
- `buildEncounterStepsForMove()`
- `runEncounterBattle(...)`
- `grantEnemyDrops(...)`
- `advanceDungeonProgress(...)`
- `calculateEnemyDamage(...)`
- `calculateEnemyDefense(...)`
- `tickBattleEffects(...)`
- `useBattleConsumable(...)`
- `resolveSkillUse(...)`
- `showBattleSkills(...)`
- `applyBattleRewards(...)`
- `applyDepthCompletionRewards(...)`

#### `include/states/battle_types.h`

Struct dan enum:

- `EnemyInstance`
- `BattleState`
- `EncounterResult`

### 11.4 Source database

#### `src/database/json_and_paths.cpp`

Tanggung jawab:

- parsing aman JSON
- path file
- starter kit
- bonus equipment internal

Fungsi:

- `asInt(...)`
- `asString(...)`
- `fileExists(...)`
- `resolveGameDataPath(...)`
- `resolveSavePath(...)`
- `sanitizeIdPart(...)`
- `equippedItemBonus(...)`
- `addStarterItem(...)`
- `giveStarterKit(...)`

#### `src/database/player_serialization.cpp`

Tanggung jawab:

- serialize/deserialize `Player`

Fungsi:

- `playerToJson(...)`
- `fillPlayerCore(...)`
- `fillInventory(...)`
- `fillCooldowns(...)`
- `fillProgress(...)`
- `playerFromJson(...)`
- `playerFromLegacyJson(...)`
- `upsertCharacter(...)`

#### `src/database/save_system.cpp`

Tanggung jawab:

- load game data
- load save
- save multi-character
- create new game

Fungsi:

- `loadGameData(...)`
- `loadSave(...)`
- `loadCharacter(...)`
- `saveGame(...)`
- `createNewGame(...)`

#### `src/database/data_lookup.cpp`

Tanggung jawab:

- semua lookup by id
- mapping category dan slot

Fungsi:

- `findInArrayById(...)`
- `getClassById(...)`
- `getItemById(...)`
- `getSkillById(...)`
- `getDungeonById(...)`
- `getEnemyById(...)`
- `findDungeonDepth(...)`
- `itemCategory(...)`
- `classPrimaryStat(...)`
- `itemRequiredClass(...)`
- `slotFromCategory(...)`
- `isEquipmentCategory(...)`

#### `src/database/resource_balance.cpp`

Tanggung jawab:

- exp requirement
- bonus stat item
- base/effective HP MP
- sinkronisasi resource player

Fungsi:

- `expRequiredForNextLevel(...)`
- `itemAtkBonus(...)`
- `itemDefBonus(...)`
- `itemHpBonus(...)`
- `itemMpBonus(...)`
- `baseMaxHp(...)`
- `baseMaxMp(...)`
- `effectiveMaxHp(...)`
- `effectiveMaxMp(...)`
- `normalizePlayerResources(...)`

#### `src/database/lookups_and_balance.cpp`

File ini sekarang hanya legacy aggregator.

Fungsinya:

- menjaga build manual lama tetap hidup
- meng-include `data_lookup.cpp` dan `resource_balance.cpp`

### 11.5 Source helper state

#### `src/states/helpers/input_helpers.cpp`

Fungsi:

- `rng()`
- `randInt(...)`
- `randUnit()`
- `trim(...)`
- `toLower(...)`
- `readLine()`
- `tryParseInt(...)`
- `getPrimaryStatValue(...)`

#### `src/states/helpers/inventory_helpers.cpp`

Fungsi:

- `inventoryCount(...)`
- `mergeInventory(...)`
- `addItem(...)`
- `removeItem(...)`
- `itemBonusFromEquipped(...)`
- `getCooldown(...)`
- `setCooldown(...)`
- `reduceCooldowns(...)`

#### `src/states/helpers/progress_helpers.cpp`

Fungsi:

- `ensureDungeonProgress(...)`
- `getDungeonUnlockedDepth(...)`
- `isDungeonCompleted(...)`
- `dungeonMaxDepth(...)`
- `isFinalDungeonLocked(...)`

#### `src/states/helpers/player_stats_helpers.cpp`

Fungsi:

- `calculatePlayerAttack(...)`
- `calculatePlayerDefense(...)`
- `calculatePlayerSpeed(...)`
- `calculatePlayerCritRate(...)`
- `refreshPlayerResources(...)`
- `levelUpIfNeeded(...)`

#### `src/states/helpers/ui_helpers.cpp`

Fungsi:

- `printHubHeader(...)`
- `printStateHeader(...)`
- `showItemDetails(...)`
- `unlockedTierName(...)`
- `printClassDetails(...)`
- `itemDisplayName(...)`
- `showCharacterOverview(...)`

### 11.6 Source state utama

#### `src/states/main_menu_state.cpp`

Fungsi:

- `runMainMenu(...)`

Tugas:

- new game
- load game
- exit

#### `src/states/hub_state.cpp`

Fungsi:

- `runHub(...)`

Tugas:

- pusat menu utama setelah player aktif

#### `src/states/character_menu.cpp`

Fungsi:

- `characterMenu(...)`

Tugas:

- hanya menjadi pintu masuk menu character

#### `src/states/character_progression.cpp`

Fungsi:

- `allocateStats(...)`
- `skillMenu(...)`

Tugas:

- memisahkan logic progression dari inventory

#### `src/states/inventory_menu.cpp`

Fungsi:

- `equipInventoryEntry(...)`
- `unequipInventoryEntry(...)`
- `useConsumable(...)`
- `inventoryMenu(...)`

Tugas:

- inventory detail
- equip / unequip
- consumable
- pagination inventory

#### `src/states/math_helpers.cpp`

Fungsi:

- `normalizedRatio(...)`
- `interpolateStat(...)`

Tugas:

- helper numerik kecil yang dipakai lintas fitur

#### `src/states/dungeon_menu.cpp`

Fungsi:

- `printDungeonLore(...)`
- `selectDungeonDepth(...)`
- `chooseDungeon(...)`

#### `src/states/enemy_factory.cpp`

Fungsi:

- `isDepthLevelAllowed(...)`
- `spawnEnemyForDepth(...)`

Tugas:

- membangun enemy instance dari template JSON

#### `src/states/item_catalog_features.cpp`

Fungsi:

- `canCraftItem(...)`
- `ownedIngredientCount(...)`
- `totalIngredientNeed(...)`
- `collectCraftingCategories(...)`
- `categoriesForShopMode(...)`
- `showRecipeDetail(...)`

#### `src/states/crafting_menu.cpp`

Fungsi:

- `craftingMenu(...)`

#### `src/states/shop_menu.cpp`

Fungsi:

- `shopBuyMenu(...)`
- `shopSellMenu(...)`
- `shopMenu(...)`

#### `src/states/battle_state.cpp`

Fungsi:

- `runBattle(...)`

Tugas:

- flow eksplorasi dungeon tingkat atas

#### `src/states/battle_helpers.cpp`

Fungsi:

- `calculateEnemySpeed(...)`
- `enemyGetsFirstTurn(...)`
- `buildEncounterStepsForMove()`
- `runEncounterBattle(...)`

Tugas:

- loop battle inti

#### `src/states/battle_effects.cpp`

Fungsi:

- `grantEnemyDrops(...)`
- `advanceDungeonProgress(...)`
- `calculateEnemyDamage(...)`
- `calculateEnemyDefense(...)`
- `tickBattleEffects(...)`
- `applyBattleRewards(...)`
- `applyDepthCompletionRewards(...)`

Tugas:

- efek turn-based, reward, progress

#### `src/states/battle_skills.cpp`

Fungsi:

- `useBattleConsumable(...)`
- `resolveSkillUse(...)`
- `showBattleSkills(...)`

Tugas:

- skill dan consumable khusus battle

### 11.7 File legacy aggregator

#### `src/database.cpp`

Fungsi:

- menjaga build manual lama tetap sederhana

#### `src/states/common_helpers.cpp`

Fungsi:

- menggabungkan helper state untuk build manual lama

#### `src/states.cpp`

Fungsi:

- menggabungkan semua state untuk build manual lama

Catatan:

Untuk pengembangan harian, abaikan tiga file ini dan fokus ke file modular di dalam `src/database/`, `src/states/`, dan `src/states/helpers/`.

---

## 12. Hubungan Antar File

```text
main.cpp
  -> states.h
  -> database.h

main_menu_state.cpp
  -> save_system.cpp
  -> resource_balance.cpp

hub_state.cpp
  -> character_menu.cpp
  -> inventory_menu.cpp
  -> crafting_menu.cpp
  -> shop_menu.cpp
  -> dungeon_menu.cpp

character_menu.cpp
  -> character_progression.cpp
  -> inventory_menu.cpp

dungeon_menu.cpp
  -> enemy_factory.cpp
  -> battle_state.cpp

battle_state.cpp
  -> battle_helpers.cpp

battle_helpers.cpp
  -> battle_effects.cpp
  -> battle_skills.cpp

database public API
  -> data_lookup.cpp
  -> resource_balance.cpp
  -> save_system.cpp
  -> player_serialization.cpp
```

---

## 13. Urutan Belajar yang Disarankan

Kalau kamu masih pemula, baca codebase ini dengan urutan berikut:

1. `include/globals.h`
2. `src/main.cpp`
3. `src/states/main_menu_state.cpp`
4. `src/states/hub_state.cpp`
5. `src/states/character_menu.cpp`
6. `src/states/character_progression.cpp`
7. `src/states/inventory_menu.cpp`
8. `src/states/dungeon_menu.cpp`
9. `src/states/enemy_factory.cpp`
10. `src/states/battle_state.cpp`
11. `src/states/battle_helpers.cpp`
12. `src/states/battle_effects.cpp`
13. `src/states/battle_skills.cpp`
14. `src/database/save_system.cpp`
15. `src/database/data_lookup.cpp`
16. `src/database/resource_balance.cpp`

Kenapa urutannya seperti ini?

- kamu paham flow dulu
- baru paham detail helper
- baru paham service/data layer

---

## 14. Tempat yang Paling Sering Dibuka Saat Maintenance

Kalau ingin:

- mengubah HP/MP/ATK/DEF player: buka `include/config/player_balance.h` lalu `src/states/helpers/player_stats_helpers.cpp`
- mengubah balance musuh: buka `include/config/enemy_balance.h` lalu `src/states/battle_effects.cpp`
- mengubah damage skill: buka `include/config/skill_balance.h` lalu `src/states/battle_skills.cpp`
- mengubah resource dasar player: buka `src/database/resource_balance.cpp`
- mengubah starter kit: buka `src/database/json_and_paths.cpp`
- mengubah flow menu: buka file di `src/states/`
- mengubah save: buka `src/database/player_serialization.cpp` dan `src/database/save_system.cpp`

---

## 15. Tips Aman Saat Refactor Lagi

1. Kalau file mulai terasa punya dua tanggung jawab besar, pecah lagi.
2. Pindahkan angka balance ke `include/config/` bila angka itu sering disentuh.
3. Jangan campur serialisasi save dengan UI menu.
4. Jangan campur rumus stat dengan gambar UI kalau bisa dipisah.
5. Saat menambah file baru, jangan lupa:
   deklarasi header,
   implementasi source,
   tambah ke `CMakeLists.txt`.

---

## 16. Kesimpulan

Sekarang struktur proyek ini sengaja diarahkan supaya:

- mudah dibaca manusia
- mudah dicari saat debug
- mudah di-balance
- mudah di-expand dengan fitur baru

Kunci memahami proyek ini adalah:

- pahami `GameContext`
- pahami `include/config/`
- pahami pembagian `database`, `helpers`, dan `states`
- ikuti flow dari `main menu -> hub -> dungeon -> battle -> save`

Kalau empat flow itu sudah kamu pahami, kamu sudah mengerti tulang punggung proyek ini.
