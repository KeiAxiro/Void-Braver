# VoidBraver

CLI RPG berbasis C++ dan `nlohmann::json` yang dirancang supaya data game bisa dibaca dari file JSON, sedangkan logika game dipisah ke file-file C++ yang lebih kecil. Proyek ini cocok untuk belajar:

- state machine sederhana
- load/save JSON
- inventory dan equipment
- battle turn-based
- modularisasi kode C++
- arsitektur game data driven

README ini ditulis sebagai panduan belajar. Jadi isinya bukan cuma cara build, tetapi juga menjelaskan bagaimana alur program berjalan, kenapa file dipisah seperti sekarang, dan bagaimana cara menambah fitur baru dengan aman.

---

## 1. Gambaran Besar Proyek

Secara sederhana, game ini bekerja seperti ini:

1. `main()` membuat `GameContext`.
2. `GameContext` memuat data game (`game_data.json`) dan menyimpan character aktif.
3. Game loop membaca state paling atas dari `stateStack`.
4. State yang aktif akan menjalankan menu atau battle.
5. Saat pemain melakukan aksi, kode membaca data dari JSON lalu mengubah `ctx.player`.
6. Saat perlu, data player disimpan ke `save.json`.

Artinya, ada dua pusat utama dalam proyek ini:

- `GameContext` sebagai pusat runtime game
- `game_data.json` sebagai pusat data konten game

Kalau dianalogikan:

- `GameContext` = isi RAM saat game berjalan
- `save.json` = file penyimpanan permanen
- `game_data.json` = database konten game

---

## 2. Teknologi yang Dipakai

### Bahasa

- C++17

### Library

- `nlohmann::json` untuk parsing dan serialisasi JSON

### Build System

- CMake
- `g++` manual juga masih bisa dipakai

### Tipe aplikasi

- Console / terminal based RPG

---

## 3. Struktur Folder

```text
Optimized-Void-Braver/
├─ bin/
│  └─ data/
│     ├─ game_data.json
│     └─ save.json
├─ include/
│  ├─ database.h
│  ├─ globals.h
│  ├─ state_helpers.h
│  ├─ states.h
│  └─ utils/
│     └─ console_ui.h
├─ src/
│  ├─ main.cpp
│  ├─ database.cpp
│  ├─ states.cpp
│  ├─ database/
│  │  ├─ json_and_paths.cpp
│  │  ├─ lookups_and_balance.cpp
│  │  ├─ player_serialization.cpp
│  │  └─ save_system.cpp
│  └─ states/
│     ├─ battle_helpers.cpp
│     ├─ battle_state.cpp
│     ├─ character_menu.cpp
│     ├─ common_helpers.cpp
│     ├─ crafting_menu.cpp
│     ├─ dungeon_menu.cpp
│     ├─ hub_state.cpp
│     ├─ main_menu_state.cpp
│     ├─ shop_menu.cpp
│     └─ helpers/
│        ├─ input_helpers.cpp
│        ├─ inventory_helpers.cpp
│        ├─ player_stats_helpers.cpp
│        ├─ progress_helpers.cpp
│        └─ ui_helpers.cpp
└─ README.md
```

### Prinsip pembagian folder

- `include/` berisi deklarasi data dan fungsi
- `src/database/` berisi urusan JSON, save, load, dan lookup data
- `src/states/` berisi logika menu dan flow game
- `src/states/helpers/` berisi helper kecil yang dipakai banyak state
- `bin/data/` berisi konten game dan save

---

## 4. Cara Build dan Run

### Build Dengan CMake

```bash
cmake -S . -B build-gcc -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
cmake --build build-gcc
```

Executable akan dibangun ke:

```bash
bin/VoidBraver.exe
```

### Build Manual

```bash
g++ -std=c++17 -Iinclude -Ilib src/main.cpp src/database.cpp src/states.cpp -o bin/VoidBraver.exe
```

### Menjalankan Game

Karena `GameContext` default memakai path `data/game_data.json` dan `data/save.json`, executable paling aman dijalankan dari folder `bin`.

Contoh:

```bash
cd bin
./VoidBraver.exe
```

Kalau nanti kamu ingin executable tetap bisa dijalankan dari root project, kamu bisa:

- ubah `gameDataPath` dan `savePath` default di `GameContext`
- atau tambahkan sistem path resolver lagi

---

## 5. Mental Model Kode

Supaya lebih gampang paham, anggap proyek ini punya 5 lapisan:

### 1. Lapisan Data

File:

- `bin/data/game_data.json`
- `bin/data/save.json`

Tugas:

- menyimpan item, class, dungeon, enemy, exp table, save character

### 2. Lapisan Model

File:

- `include/globals.h`

Tugas:

- mendefinisikan bentuk data seperti `Player`, `Stats`, `Progress`, `GameContext`

### 3. Lapisan Database / Service

File:

- `src/database/*.cpp`

Tugas:

- membaca JSON
- menyimpan JSON
- mencari item/class/enemy berdasarkan id
- menghitung stat dasar

### 4. Lapisan Helper

File:

- `src/states/helpers/*.cpp`

Tugas:

- input
- UI
- inventory math
- cooldown
- progress dungeon
- rumus stat player

### 5. Lapisan State

File:

- `src/states/*.cpp`

Tugas:

- membuat menu
- mengatur alur game
- memindahkan state
- menjalankan battle dan dungeon

---

## 6. Alur Program dari Awal Sampai Akhir

### 6.1 Saat Program Baru Dibuka

File utama:

- `src/main.cpp`

Urutan:

1. `main()` membuat `GameContext ctx`.
2. `loadGameData(ctx)` membaca `game_data.json`.
3. Game masuk ke loop:

```cpp
while (ctx.running && !ctx.stateStack.empty())
```

4. State teratas di `ctx.stateStack` diproses.

Contohnya:

- `MainMenu` memanggil `runMainMenu(ctx)`
- `Hub` memanggil `runHub(ctx)`
- `Battle` memanggil `runBattle(ctx)`

### 6.2 Saat New Game

Flow:

1. `runMainMenu(ctx)` meminta nama character.
2. Player memilih class berdasarkan nomor.
3. `createNewGame(ctx, name, classId)` membuat data player baru.
4. `giveStarterKit(ctx)` memberi equipment awal sesuai class.
5. `saveGame(ctx)` menyimpan character ke file save.
6. State dipindah ke `Hub`.

### 6.3 Saat Load Game

Flow:

1. `loadSave(ctx)` membaca semua character dari `save.json`.
2. User memilih salah satu character.
3. `loadCharacter(ctx, index)` mengaktifkan character itu.
4. State masuk ke `Hub`.

### 6.4 Saat Masuk Hub

File:

- `src/states/hub_state.cpp`

Hub adalah pusat navigasi:

- Character Menu
- Inventory
- Enter Dungeon
- Crafting
- Shop
- Save
- Kembali ke Main Menu

### 6.5 Saat Masuk Dungeon

Flow:

1. `chooseDungeon(ctx)` memanggil `selectDungeonDepth(ctx, true)`.
2. Player memilih dungeon.
3. Player memilih depth yang terbuka.
4. Validasi level range dijalankan.
5. State `Battle` didorong ke `stateStack`.

### 6.6 Saat Eksplorasi Dungeon

File:

- `src/states/battle_state.cpp`

Flow:

1. Depth aktif ditampilkan.
2. Jika pilih `Move Forward`, sistem membuat 5 langkah.
3. Di dalam 5 langkah itu, `buildEncounterStepsForMove()` menentukan 1-3 langkah yang memicu encounter.
4. Saat encounter terjadi, `spawnEnemyForDepth()` membuat instance musuh berdasarkan JSON.
5. `runEncounterBattle(...)` menjalankan battle.
6. Jika semua langkah selesai, depth dianggap clear dan reward depth diberikan.

### 6.7 Saat Battle

File:

- `src/states/battle_helpers.cpp`

Flow:

1. Game menggambar panel battle.
2. Sistem menentukan giliran awal berdasarkan speed.
3. Player memilih:

- Attack
- Skill
- Consumable
- Run

4. Rumus damage dihitung.
5. Efek battle diproses:

- shield
- evade
- invincible
- stun
- dot
- buff/debuff
- cooldown

6. Jika musuh kalah, reward EXP/gold/drop diberikan.
7. Jika player kalah, HP/MP dipulihkan ke sebagian dan save dijalankan.

---

## 7. Pusat Runtime: `GameContext`

`GameContext` adalah objek terpenting di proyek ini. Hampir semua fungsi menerima `GameContext &ctx`.

Isi pentingnya:

| Field | Fungsi |
|---|---|
| `gameData` | Semua data dari `game_data.json` |
| `player` | Character yang sedang aktif |
| `characters` | Semua character dari save |
| `activeCharacterIndex` | Index character aktif |
| `stateStack` | Stack state game |
| `gameDataPath` | Path file data |
| `savePath` | Path file save |
| `running` | Penanda game loop masih hidup |

Kalau bingung mencari “state game sekarang ada di mana?”, jawabannya hampir selalu ada di `ctx`.

---

## 8. Sistem State Stack

State stack adalah sistem sederhana untuk menentukan layar/menu yang sedang aktif.

State yang ada sekarang:

- `MainMenu`
- `Hub`
- `Battle`
- `Quit`

Contoh:

- awal game: `stateStack = [MainMenu]`
- setelah load character: `stateStack = [Hub]`
- masuk dungeon: `stateStack = [Hub, Battle]`
- keluar dari dungeon: state `Battle` dipop, kembali ke `Hub`

Konsep penting:

- state paling belakang = state yang sedang aktif
- `push_back(...)` = masuk ke layar baru
- `pop_back()` = keluar dari layar sekarang

---

## 9. Pola Data Driven

Game ini banyak bergantung pada JSON. Artinya konten game sebisa mungkin datang dari data, bukan hardcode angka di C++.

Contoh data yang diambil dari JSON:

- daftar class
- daftar skill per class
- daftar item
- harga beli/jual
- resep crafting
- daftar dungeon dan depth
- daftar enemy dan range stat
- kebutuhan EXP tiap level

Keuntungan pendekatan ini:

- menambah konten baru lebih cepat
- balancing lebih mudah
- logic dan data tidak bercampur terlalu banyak

Kapan sebaiknya isi JSON?

- saat menambah item baru
- saat menambah enemy baru
- saat menambah dungeon baru
- saat mengubah harga atau stat

Kapan sebaiknya ubah C++?

- saat menambah aturan baru
- saat menambah menu baru
- saat menambah perilaku battle baru
- saat menambah validasi baru

---

## 10. Cara Berpikir Saat Mengoding di Proyek Ini

Urutan berpikir yang disarankan:

1. Tentukan fitur baru ini termasuk data, logic, atau UI.
2. Cek apakah fitur baru cukup ditambah di JSON, atau memang perlu ubah C++.
3. Cari state yang paling dekat dengan fitur itu.
4. Cari helper yang sudah ada, jangan langsung buat kode baru kalau helper lama bisa dipakai.
5. Kalau logic mulai panjang, pindahkan ke helper file yang sesuai topik.
6. Kalau fitur memengaruhi save, update serialisasi juga.

Contoh:

### Jika ingin menambah item baru

- cukup ubah `game_data.json`
- tidak perlu ubah state, selama format item masih sama

### Jika ingin menambah menu baru di hub

- ubah `src/states/hub_state.cpp`
- bila logic panjang, pindahkan ke file state baru
- deklarasikan fungsi bila perlu

### Jika ingin menambah efek skill baru

- data skill ditambah di JSON
- implementasi efeknya ditambah di `resolveSkillUse(...)`

---

## 11. Cara Menambah Fitur Baru

### 11.1 Menambah Menu Baru

Contoh: ingin menambah menu `Quest`.

Langkah:

1. Tambah opsi di `runHub(ctx)`.
2. Buat file baru misalnya `src/states/quest_menu.cpp`.
3. Tambahkan fungsi utama misalnya `void questMenu(GameContext &ctx);`
4. Tambah deklarasi di header jika dibutuhkan.
5. Include file itu dari `src/states.cpp`.

Tips:

- kalau menu masih kecil, satu file cukup
- kalau menunya besar, buat helper tambahan

### 11.2 Menambah State Baru

Contoh: `QuestState`.

Langkah:

1. Tambah enum baru di `GameState`.
2. Tambah deklarasi fungsi di `include/states.h`.
3. Tambah `case` baru di `main.cpp`.
4. Buat file state baru.

Gunakan state baru jika:

- flow-nya beda dari hub/menu biasa
- butuh loop sendiri
- butuh identitas layar khusus

### 11.3 Menambah Item Baru

Langkah:

1. Tambah item di `game_data.json`.
2. Pastikan field penting ada:

- `id`
- `category`
- `name`
- `description`
- `buy_price`
- `sell_price`
- `required_class_id`
- `tier_level`
- `stats`

3. Jika item craftable, tambahkan `crafting.ingredients`.
4. Jika item shopable, beri `buy_price > 0`.
5. Jika item ingin muncul di inventory/equip, pastikan category-nya cocok.

### 11.4 Menambah Enemy Baru

Langkah:

1. Tambah enemy di `game_data.json`.
2. Isi:

- `id`
- `name`
- `type`
- `level_range`
- `stats_range`
- `crit_rate`
- `spawn_depth`
- `is_boss`
- `drops`

3. Pastikan `spawn_depth` cocok dengan dungeon yang diinginkan.

Enemy baru otomatis bisa dipakai oleh `spawnEnemyForDepth(...)` selama format datanya benar.

### 11.5 Menambah Skill Baru

Langkah:

1. Tambahkan skill ke class di `game_data.json`.
2. Skill otomatis muncul di `showBattleSkills(...)`.
3. Implementasi efek skill wajib ditambah di `resolveSkillUse(...)`.

Kalau tidak ditambah di `resolveSkillUse(...)`, skill hanya jadi data tanpa efek khusus.

### 11.6 Menambah Data Save Baru

Contoh: mau menambah `quest_progress`.

Langkah:

1. Tambah field di `Player` atau `Progress`.
2. Tambah serialisasi di `playerToJson(...)`.
3. Tambah deserialisasi di `fillPlayerCore(...)` atau `fillProgress(...)`.
4. Pastikan ada default aman jika save lama belum punya field itu.

Ini penting supaya save lama tidak rusak.

---

## 12. Aturan Aman Saat Mengubah Kode

Beberapa aturan yang bagus untuk diikuti:

### 1. Jangan taruh semua logic di satu file besar

Kalau logic mulai terlalu panjang:

- pindah ke helper
- atau buat state file baru

### 2. Pisahkan antara data dan aturan

- data konten taruh di JSON
- aturan permainan taruh di C++

### 3. Simpan default yang aman

Saat membaca JSON:

- selalu pakai `value(...)`
- atau helper seperti `asInt(...)`, `asString(...)`

Ini mencegah crash saat field belum ada.

### 4. Pastikan UI tidak langsung hilang

Kalau ada notif penting:

- tampilkan notif
- panggil `waitForEnter()`
- baru nanti layar berikutnya di-clear

### 5. Kalau fitur menyentuh save, pikirkan backward compatibility

Save lama mungkin belum punya field baru.

---

## 13. Penjelasan File per File dan Fungsi per Fungsi

Bagian ini adalah peta teknis utama proyek.

### 13.1 `include/globals.h`

Header ini berisi struktur data inti game.

#### Enum / Struct

| Nama | Fungsi |
|---|---|
| `GameState` | Menandai state aktif game |
| `Stats` | Menyimpan STR, INT, AGI, VIT |
| `InventoryEntry` | Satu baris item di inventory |
| `CooldownEntry` | Menyimpan cooldown skill |
| `Progress` | Menyimpan progress dungeon |
| `Player` | Model character lengkap |
| `GameContext` | Pusat data runtime game |

#### Konstanta

| Nama | Fungsi |
|---|---|
| `STARTING_GOLD` | Gold awal karakter baru |
| `STARTING_HP` | HP fallback saat load |
| `STARTING_MP` | MP fallback saat load |
| `STARTING_STAT_POINTS` | Stat point awal |
| `STAT_POINTS_PER_LEVEL` | Stat point tiap level up |
| `PAGE_SIZE` | Ukuran pagination default |
| `MENU_SEPARATOR` | Garis pemisah UI |
| `TITLE_SEPARATOR` | Garis title UI |

#### Fungsi

| Fungsi | Fungsi teknis |
|---|---|
| `clampInt(...)` | Membatasi angka agar tetap di rentang tertentu |

### 13.2 `include/database.h`

Header deklarasi fungsi database/service.

#### Fungsi public

| Fungsi | Peran |
|---|---|
| `loadGameData(ctx)` | Membaca `game_data.json` ke `ctx.gameData` |
| `loadSave(ctx)` | Membaca semua character dari save |
| `loadCharacter(ctx, index)` | Mengaktifkan character berdasarkan index |
| `saveGame(ctx)` | Menulis save multi-character |
| `createNewGame(ctx, playerName, classId)` | Membuat player baru |
| `findInArrayById(arr, id)` | Helper lookup generik by id |
| `getClassById(...)` | Mencari class |
| `getItemById(...)` | Mencari item |
| `getSkillById(...)` | Mencari skill |
| `getDungeonById(...)` | Mencari dungeon |
| `getEnemyById(...)` | Mencari enemy |
| `findDungeonDepth(...)` | Mencari row depth dalam dungeon |
| `expRequiredForNextLevel(...)` | Menghitung EXP next level |
| `itemAtkBonus(...)` | Bonus ATK dari item |
| `itemDefBonus(...)` | Bonus DEF dari item |
| `itemHpBonus(...)` | Bonus HP dari item |
| `itemMpBonus(...)` | Bonus MP dari item |
| `itemCategory(...)` | Mengambil category item |
| `classPrimaryStat(...)` | Mengambil primary stat class |
| `itemRequiredClass(...)` | Mengambil class requirement item |
| `slotFromCategory(...)` | Mengubah category equipment menjadi nama slot |
| `isEquipmentCategory(...)` | Mengecek apakah category adalah equipment |
| `baseMaxHp(...)` | Menghitung max HP dasar |
| `baseMaxMp(...)` | Menghitung max MP dasar |
| `effectiveMaxHp(...)` | Menghitung HP setelah bonus equipment |
| `effectiveMaxMp(...)` | Menghitung MP setelah bonus equipment |
| `normalizePlayerResources(...)` | Menyelaraskan hp/mp dengan max terbaru |

### 13.3 `include/states.h`

Header sederhana untuk state utama.

| Fungsi | Peran |
|---|---|
| `runMainMenu(ctx)` | Menjalankan menu awal |
| `runHub(ctx)` | Menjalankan main hub |
| `runBattle(ctx)` | Menjalankan eksplorasi dungeon + battle loop level atas |

### 13.4 `include/state_helpers.h`

Header deklarasi helper state. File ini penting karena banyak fungsi lintas state dideklarasikan di sini.

#### Struct

| Nama | Peran |
|---|---|
| `EnemyInstance` | Musuh runtime hasil spawn dari template JSON |
| `BattleState` | Status efek sementara selama battle |
| `EncounterResult` | Hasil battle: menang, mundur, kalah |

#### Fungsi kelompok input dan utility

| Fungsi | Peran |
|---|---|
| `randInt(...)` | Bilangan acak integer |
| `randUnit()` | Bilangan acak 0.0 sampai 1.0 |
| `trim(...)` | Menghapus spasi kiri kanan |
| `toLower(...)` | Ubah string jadi lowercase |
| `readLine()` | Baca input user |
| `tryParseInt(...)` | Parse string ke integer dengan aman |
| `getPrimaryStatValue(...)` | Ambil nilai stat utama player |

#### Fungsi kelompok inventory dan cooldown

| Fungsi | Peran |
|---|---|
| `inventoryCount(...)` | Hitung total item tertentu di inventory |
| `mergeInventory(...)` | Gabungkan entri inventory yang sama |
| `addItem(...)` | Tambah item ke inventory |
| `removeItem(...)` | Kurangi atau hapus item |
| `itemBonusFromEquipped(...)` | Hitung bonus dari equipment aktif |
| `getCooldown(...)` | Ambil cooldown skill |
| `setCooldown(...)` | Set cooldown skill |
| `reduceCooldowns(...)` | Kurangi semua cooldown setelah turn |

#### Fungsi kelompok progress dungeon

| Fungsi | Peran |
|---|---|
| `ensureDungeonProgress(...)` | Pastikan progress dungeon tersedia |
| `getDungeonUnlockedDepth(...)` | Ambil depth terbuka |
| `isDungeonCompleted(...)` | Cek dungeon selesai atau belum |
| `dungeonMaxDepth(...)` | Ambil jumlah maksimum depth |
| `isFinalDungeonLocked(...)` | Mengunci final dungeon jika dungeon lain belum selesai |

#### Fungsi kelompok stat player

| Fungsi | Peran |
|---|---|
| `calculatePlayerAttack(...)` | Rumus ATK player |
| `calculatePlayerDefense(...)` | Rumus DEF player |
| `calculatePlayerSpeed(...)` | Rumus speed player |
| `calculatePlayerCritRate(...)` | Rumus crit rate player |
| `refreshPlayerResources(...)` | Sinkronisasi max HP/MP |
| `levelUpIfNeeded(...)` | Proses level up jika EXP cukup |

#### Fungsi kelompok UI

| Fungsi | Peran |
|---|---|
| `printHubHeader(...)` | Header konteks player |
| `printStateHeader(...)` | Header player + judul state |
| `showItemDetails(...)` | Menampilkan detail item lengkap |
| `unlockedTierName(...)` | Nama tier class yang sudah terbuka |
| `printClassDetails(...)` | Info class dan tier |
| `itemDisplayName(...)` | Nama item dari `item_id` |
| `showCharacterOverview(...)` | Ringkasan player di character menu |

#### Fungsi kelompok character / inventory menu

| Fungsi | Peran |
|---|---|
| `allocateStats(...)` | Alokasi stat point |
| `equipInventoryEntry(...)` | Equip item dari inventory |
| `unequipInventoryEntry(...)` | Unequip item |
| `useConsumable(...)` | Pakai item consumable |
| `inventoryMenu(...)` | Menu inventory |
| `skillMenu(...)` | Lihat daftar skill |
| `characterMenu(...)` | Menu character utama |
| `normalizedRatio(...)` | Rasio normalisasi untuk interpolasi |
| `interpolateStat(...)` | Interpolasi stat berdasarkan level |

#### Fungsi kelompok dungeon / crafting / shop

| Fungsi | Peran |
|---|---|
| `spawnEnemyForDepth(...)` | Membuat musuh dari data JSON |
| `isDepthLevelAllowed(...)` | Validasi level player untuk depth |
| `printDungeonLore(...)` | Tampilkan lore depth |
| `selectDungeonDepth(...)` | Pilih dungeon dan depth |
| `chooseDungeon(...)` | Shortcut masuk selection dungeon |
| `canCraftItem(...)` | Cek bahan crafting cukup atau tidak |
| `ownedIngredientCount(...)` | Hitung total bahan yang sudah dimiliki |
| `totalIngredientNeed(...)` | Hitung total bahan yang dibutuhkan |
| `collectCraftingCategories(...)` | Ambil kategori crafting yang valid |
| `categoriesForShopMode(...)` | Ambil kategori shop untuk mode buy/sell |
| `showRecipeDetail(...)` | Tampilkan detail resep |
| `craftingMenu(...)` | Menu crafting utama |
| `shopMenu(...)` | Menu shop utama |

#### Fungsi kelompok battle

| Fungsi | Peran |
|---|---|
| `calculateEnemySpeed(...)` | Rumus speed musuh |
| `enemyGetsFirstTurn(...)` | Menentukan giliran pertama |
| `buildEncounterStepsForMove()` | Menentukan langkah encounter |
| `runEncounterBattle(...)` | Battle inti melawan 1 musuh |
| `grantEnemyDrops(...)` | Memberi drop item |
| `advanceDungeonProgress(...)` | Membuka depth berikutnya |
| `calculateEnemyDamage(...)` | Rumus damage musuh |
| `calculateEnemyDefense(...)` | Rumus defense musuh |
| `tickBattleEffects(...)` | Tick buff/debuff/dot/cooldown |
| `useBattleConsumable(...)` | Menu consumable di battle |
| `resolveSkillUse(...)` | Menerapkan efek skill |
| `showBattleSkills(...)` | UI daftar skill saat battle |
| `applyBattleRewards(...)` | Reward setelah battle |
| `applyDepthCompletionRewards(...)` | Reward clear depth |

### 13.5 `include/utils/console_ui.h`

Helper UI console.

#### Enum

| Nama | Peran |
|---|---|
| `Color` | Daftar warna teks |

#### Fungsi

| Fungsi | Peran |
|---|---|
| `colorCode(...)` | Menghasilkan ANSI code warna |
| `initializeConsole()` | Mengaktifkan ANSI di Windows |
| `colorText(...)` | Mewarnai teks |
| `clearScreen()` | Membersihkan terminal dengan cara yang lebih stabil |
| `printLine(...)` | Mencetak garis |
| `printTitleBox(...)` | Mencetak judul boxed |
| `waitForEnter(...)` | Pause hingga Enter ditekan |

### 13.6 `src/main.cpp`

Titik masuk program.

| Fungsi | Peran |
|---|---|
| `main()` | Membuat context, load data, menjalankan game loop, dan save saat keluar |

### 13.7 `src/database.cpp`

File penghubung. Tidak berisi logic bisnis.

Tujuan:

- menjaga command build manual tetap sederhana
- menggabungkan file kecil database menjadi satu unit compile

Isi yang di-include:

- `json_and_paths.cpp`
- `player_serialization.cpp`
- `save_system.cpp`
- `lookups_and_balance.cpp`

### 13.8 `src/states.cpp`

File penghubung untuk semua state.

Tujuan:

- menjaga build manual tetap sederhana
- menggabungkan semua modul state

Isi yang di-include:

- `common_helpers.cpp`
- `character_menu.cpp`
- `dungeon_menu.cpp`
- `crafting_menu.cpp`
- `shop_menu.cpp`
- `battle_helpers.cpp`
- `main_menu_state.cpp`
- `hub_state.cpp`
- `battle_state.cpp`

### 13.9 `src/database/json_and_paths.cpp`

Helper internal untuk parsing aman, path, dan starter kit.

| Fungsi | Peran |
|---|---|
| `asInt(...)` | Membaca JSON sebagai integer dengan fallback |
| `asString(...)` | Membaca JSON sebagai string dengan fallback |
| `fileExists(...)` | Cek file ada atau tidak |
| `resolveGameDataPath(ctx)` | Menentukan path game data |
| `resolveSavePath(ctx)` | Menentukan path save |
| `sanitizeIdPart(...)` | Membersihkan nama menjadi id aman |
| `equippedItemBonus(...)` | Menghitung bonus tertentu dari equipment aktif |
| `addStarterItem(...)` | Menambah item starter ke inventory |
| `giveStarterKit(ctx)` | Memberi starter item sesuai class |

Catatan belajar:

File ini contoh bagus untuk helper kecil yang tidak perlu diketahui semua file lain. Karena itu banyak fungsi diletakkan di anonymous namespace.

### 13.10 `src/database/player_serialization.cpp`

Semua urusan ubah `Player` <-> JSON.

| Fungsi | Peran |
|---|---|
| `playerToJson(...)` | Mengubah `Player` menjadi objek JSON save |
| `fillPlayerCore(...)` | Mengisi data inti player dari JSON |
| `fillInventory(...)` | Mengisi inventory dari JSON |
| `fillCooldowns(...)` | Mengisi cooldown dari JSON |
| `fillProgress(...)` | Mengisi progress dari JSON |
| `playerFromJson(...)` | Membuat `Player` dari save format baru |
| `playerFromLegacyJson(...)` | Membuat `Player` dari save lama |
| `upsertCharacter(...)` | Update character kalau id sama, tambah kalau belum ada |

Catatan belajar:

Ini adalah contoh penting backward compatibility. `playerFromLegacyJson(...)` membuat save lama tetap bisa dibaca.

### 13.11 `src/database/save_system.cpp`

Urusan load/save file dan create character.

| Fungsi | Peran |
|---|---|
| `loadGameData(ctx)` | Membaca `game_data.json` ke `ctx.gameData` |
| `loadSave(ctx)` | Membaca save multi-character ke `ctx.characters` |
| `loadCharacter(ctx, index)` | Memilih character aktif dari daftar save |
| `saveGame(ctx)` | Menyimpan semua character ke `save.json` |
| `createNewGame(ctx, playerName, classId)` | Membuat character baru dari nol |

Catatan belajar:

`saveGame(...)` tidak cuma menyimpan `ctx.player`, tetapi menyatukan player aktif ke `ctx.characters` dulu, baru seluruh array ditulis ke file.

### 13.12 `src/database/lookups_and_balance.cpp`

Tempat lookup data dan rumus balance dasar.

| Fungsi | Peran |
|---|---|
| `findInArrayById(...)` | Pencarian generik objek JSON by id |
| `getClassById(...)` | Lookup class |
| `getItemById(...)` | Lookup item |
| `getSkillById(...)` | Lookup skill |
| `getDungeonById(...)` | Lookup dungeon |
| `getEnemyById(...)` | Lookup enemy |
| `findDungeonDepth(...)` | Lookup row depth tertentu |
| `expRequiredForNextLevel(...)` | Ambil dan modifikasi kebutuhan EXP |
| `itemAtkBonus(...)` | ATK bonus item |
| `itemDefBonus(...)` | DEF bonus item |
| `itemHpBonus(...)` | HP bonus item |
| `itemMpBonus(...)` | MP bonus item |
| `itemCategory(...)` | Category item |
| `classPrimaryStat(...)` | Primary stat class |
| `itemRequiredClass(...)` | Class requirement item |
| `slotFromCategory(...)` | Mapping category ke slot equipment |
| `isEquipmentCategory(...)` | Cek category equipment |
| `baseMaxHp(...)` | Rumus HP dasar |
| `baseMaxMp(...)` | Rumus MP dasar |
| `effectiveMaxHp(...)` | HP dasar + bonus equipment |
| `effectiveMaxMp(...)` | MP dasar + bonus equipment |
| `normalizePlayerResources(...)` | Sinkronisasi HP/MP current vs max |

Catatan belajar:

Kalau ingin rebalance game secara global, file ini adalah salah satu tempat pertama yang harus dicek.

### 13.13 `src/states/common_helpers.cpp`

File penghubung helper state.

Tujuan:

- memecah helper besar menjadi beberapa file kecil
- memudahkan build manual

### 13.14 `src/states/helpers/input_helpers.cpp`

Helper input dan random.

| Fungsi | Peran |
|---|---|
| `rng()` | Generator random utama |
| `randInt(...)` | Random integer |
| `randUnit()` | Random floating 0-1 |
| `trim(...)` | Rapikan input |
| `toLower(...)` | Lowercase |
| `readLine()` | Baca input line |
| `tryParseInt(...)` | Parse integer aman |
| `getPrimaryStatValue(...)` | Mengambil nilai STR/INT/AGI/VIT berdasarkan nama |

### 13.15 `src/states/helpers/inventory_helpers.cpp`

Helper inventory, item, dan cooldown.

| Fungsi | Peran |
|---|---|
| `inventoryCount(...)` | Hitung total item |
| `mergeInventory(...)` | Gabungkan entry sejenis |
| `addItem(...)` | Tambahkan item |
| `removeItem(...)` | Hapus item |
| `itemBonusFromEquipped(...)` | Total bonus dari item equip |
| `getCooldown(...)` | Baca cooldown skill |
| `setCooldown(...)` | Set cooldown |
| `reduceCooldowns(...)` | Kurangi semua cooldown |

Catatan belajar:

Kalau inventory terasa “dobel” atau hasil equip aneh, file ini biasanya jadi tempat debugging utama.

### 13.16 `src/states/helpers/progress_helpers.cpp`

Helper progress dungeon.

| Fungsi | Peran |
|---|---|
| `ensureDungeonProgress(...)` | Membuat progress default jika belum ada |
| `getDungeonUnlockedDepth(...)` | Ambil unlocked depth |
| `isDungeonCompleted(...)` | Cek status selesai |
| `dungeonMaxDepth(...)` | Ambil depth maksimum |
| `isFinalDungeonLocked(...)` | Mengunci final dungeon sampai syarat terpenuhi |

### 13.17 `src/states/helpers/player_stats_helpers.cpp`

Rumus stat combat player.

| Fungsi | Peran |
|---|---|
| `calculatePlayerAttack(...)` | Rumus ATK player |
| `calculatePlayerDefense(...)` | Rumus DEF player |
| `calculatePlayerSpeed(...)` | Rumus speed player |
| `calculatePlayerCritRate(...)` | Rumus crit player |
| `refreshPlayerResources(...)` | Sinkronisasi HP/MP player |
| `levelUpIfNeeded(...)` | Level up jika EXP cukup |

Catatan belajar:

Kalau player terasa terlalu lemah atau terlalu kuat, file ini dan `lookups_and_balance.cpp` biasanya perlu dilihat bersama.

### 13.18 `src/states/helpers/ui_helpers.cpp`

Semua helper presentasi UI yang sering dipakai.

| Fungsi | Peran |
|---|---|
| `printHubHeader(ctx)` | Header player di atas layar |
| `printStateHeader(ctx, title)` | Header player + judul menu |
| `showItemDetails(...)` | Tampilkan detail item lengkap |
| `unlockedTierName(...)` | Hitung tier class yang sudah dibuka |
| `printClassDetails(...)` | Tampilkan detail class |
| `itemDisplayName(...)` | Nama item dari id |
| `showCharacterOverview(...)` | Ringkasan player untuk character menu |

### 13.19 `src/states/main_menu_state.cpp`

Logika main menu.

| Fungsi | Peran |
|---|---|
| `runMainMenu(ctx)` | Menjalankan new game, load game, dan exit |

Hal penting:

- `New Game` membuat player lalu save
- `Load Game` memilih character dari array save
- state berikutnya setelah character aktif adalah `Hub`

### 13.20 `src/states/hub_state.cpp`

Pusat navigasi pemain setelah masuk game.

| Fungsi | Peran |
|---|---|
| `runHub(ctx)` | Menampilkan menu utama hub dan mengarahkan ke fitur lain |

### 13.21 `src/states/character_menu.cpp`

Semua menu karakter, inventory, equip, konsumsi item, dan alokasi stat.

| Fungsi | Peran |
|---|---|
| `allocateStats(ctx)` | Menambah STR/INT/AGI/VIT dari stat point |
| `equipInventoryEntry(ctx, index)` | Memasang equipment |
| `unequipInventoryEntry(ctx, index)` | Melepas equipment |
| `useConsumable(ctx, index)` | Menggunakan potion/elixir |
| `inventoryMenu(ctx)` | UI inventory dengan pagination |
| `skillMenu(ctx)` | Menampilkan daftar skill class |
| `characterMenu(ctx)` | Menu utama character |
| `normalizedRatio(...)` | Rasio bantu interpolasi |
| `interpolateStat(...)` | Interpolasi stat enemy berdasarkan level |

Catatan belajar:

Walaupun `interpolateStat(...)` dipakai untuk spawn enemy, posisinya di file ini karena sebelumnya helper umum stat masih satu jalur dengan menu character. Kalau nanti mau dirapikan lagi, fungsi ini bisa dipindah ke helper numerik khusus.

### 13.22 `src/states/dungeon_menu.cpp`

Urusan pilih dungeon, pilih depth, spawn musuh, helper crafting/shop category.

| Fungsi | Peran |
|---|---|
| `spawnEnemyForDepth(...)` | Membuat instance musuh dari template JSON |
| `isDepthLevelAllowed(...)` | Validasi level range depth |
| `printDungeonLore(...)` | Menampilkan lore depth |
| `selectDungeonDepth(ctx, enterBattleState)` | Menu pilih dungeon dan depth |
| `chooseDungeon(ctx)` | Shortcut masuk ke `selectDungeonDepth(..., true)` |
| `canCraftItem(...)` | Cek apakah bahan lengkap |
| `ownedIngredientCount(...)` | Total progres bahan |
| `totalIngredientNeed(...)` | Total bahan resep |
| `collectCraftingCategories(...)` | Ambil kategori craftable |
| `categoriesForShopMode(...)` | Ambil kategori shopable |
| `showRecipeDetail(...)` | Menampilkan detail resep |

Catatan belajar:

File ini bukan cuma soal dungeon. Ia juga memuat helper yang dipakai crafting dan shop, karena helper itu sama-sama bergantung pada data item dari JSON.

### 13.23 `src/states/crafting_menu.cpp`

UI crafting utama.

| Fungsi | Peran |
|---|---|
| `craftingMenu(ctx)` | Menangani category, pagination, mode craft/recipe, dan crafting item |

Flow internal:

1. Ambil kategori crafting.
2. Player pilih kategori.
3. Sistem sorting recipe:

- craftable sekarang di atas
- class yang cocok diprioritaskan
- recipe dengan progres bahan lebih tinggi diprioritaskan

4. Player bisa lihat recipe atau craft.
5. Sebelum crafting, ada konfirmasi.

### 13.24 `src/states/shop_menu.cpp`

UI buy/sell shop.

| Fungsi | Peran |
|---|---|
| `shopBuyMenu(ctx)` | Menu beli item |
| `shopSellMenu(ctx)` | Menu jual item |
| `shopMenu(ctx)` | Menu utama shop |

Flow internal:

Buy:

1. pilih kategori
2. lihat item pagination
3. lihat detail item
4. pilih buy
5. masukkan jumlah
6. gold dikurangi, item ditambah

Sell:

1. pilih kategori yang memang ada di inventory
2. pilih item
3. lihat detail item
4. pilih sell
5. masukkan jumlah
6. item dikurangi, gold bertambah

### 13.25 `src/states/battle_state.cpp`

Flow eksplorasi dungeon level atas.

| Fungsi | Peran |
|---|---|
| `runBattle(ctx)` | Menangani menu depth aktif, langkah 5 tile, encounter acak, dan depth clear |

Catatan belajar:

File ini bukan battle turn-by-turn. Ia adalah “battle world flow”. Battle per musuh yang detail ada di `battle_helpers.cpp`.

### 13.26 `src/states/battle_helpers.cpp`

Inti sistem combat.

| Fungsi | Peran |
|---|---|
| `calculateEnemySpeed(...)` | Hitung speed musuh |
| `enemyGetsFirstTurn(...)` | Tentukan siapa jalan duluan |
| `buildEncounterStepsForMove()` | Tentukan 1-3 encounter dalam 5 langkah |
| `runEncounterBattle(...)` | Menjalankan battle lengkap melawan satu musuh |
| `grantEnemyDrops(...)` | Memberi item drop |
| `advanceDungeonProgress(...)` | Update progress setelah depth clear |
| `calculateEnemyDamage(...)` | Rumus ATK efektif musuh |
| `calculateEnemyDefense(...)` | Rumus DEF efektif musuh |
| `tickBattleEffects(...)` | Tick semua efek turn based |
| `useBattleConsumable(ctx)` | Inventory consumable khusus battle |
| `resolveSkillUse(...)` | Implementasi efek setiap skill |
| `showBattleSkills(ctx)` | Menampilkan daftar skill saat battle |
| `applyBattleRewards(...)` | Memberi EXP/gold/drop setelah menang |
| `applyDepthCompletionRewards(...)` | Bonus clear setelah 5 langkah selesai |

Titik paling penting untuk modding battle:

Kalau kamu ingin mengubah:

- damage skill
- buff/debuff
- efek stun, poison, burn
- shield
- reward battle

hampir semuanya ada di file ini.

---

## 14. Hubungan Antar File

Supaya tidak bingung pindah file, lihat peta hubungan berikut:

```text
main.cpp
  -> states.h
  -> runMainMenu / runHub / runBattle

main_menu_state.cpp
  -> save_system.cpp
  -> lookups_and_balance.cpp

hub_state.cpp
  -> character_menu.cpp
  -> crafting_menu.cpp
  -> shop_menu.cpp
  -> dungeon_menu.cpp

battle_state.cpp
  -> dungeon_menu.cpp
  -> battle_helpers.cpp

battle_helpers.cpp
  -> player_stats_helpers.cpp
  -> inventory_helpers.cpp
  -> ui_helpers.cpp
  -> database lookups
```

Kalau kamu bingung “fitur ini harus dikerjakan di file mana?”, gunakan aturan ini:

- menu = `src/states/`
- helper umum = `src/states/helpers/`
- load/save/data lookup = `src/database/`
- struktur data = `include/globals.h`

---

## 15. Contoh Skenario Menambah Fitur

### Contoh A: Menambah item potion baru

Misal mau menambah `Greater Healing Potion`.

Langkah:

1. Tambah item di `game_data.json`.
2. Category = `consumable`.
3. Tambah logic di `useConsumable(...)` jika ingin efeknya beda dari potion biasa.

Kalau efeknya cukup dikenali dari nama atau format yang sudah ada, mungkin tidak perlu ubah banyak C++.

### Contoh B: Menambah dungeon baru

Langkah:

1. Tambah object dungeon di `game_data.json`.
2. Isi `depths`.
3. Pastikan enemy punya `spawn_depth` yang relevan.
4. Tidak perlu ubah menu jika format data sudah sesuai.

### Contoh C: Menambah efek skill `lifesteal`

Langkah:

1. Tambahkan skill di class JSON.
2. Tambah `else if` baru di `resolveSkillUse(...)`.
3. Setelah damage dihitung, tambahkan heal ke player.

### Contoh D: Menambah quest system

Saran arsitektur:

1. Tambah data `quest_progress` di `Player` atau `Progress`.
2. Tambah serialisasi.
3. Buat `quest_menu.cpp`.
4. Tambah opsi menu di hub.
5. Jika quest kompleks, buat helper atau state baru lagi.

---

## 16. Tips Belajar dari Proyek Ini

Kalau kamu masih pemula di C++, urutan belajar dari codebase ini sebaiknya:

1. baca `include/globals.h`
2. baca `src/main.cpp`
3. baca `src/states/main_menu_state.cpp`
4. baca `src/states/hub_state.cpp`
5. baca `src/states/character_menu.cpp`
6. baca `src/states/dungeon_menu.cpp`
7. baca `src/states/battle_state.cpp`
8. baca `src/states/battle_helpers.cpp`
9. baru setelah itu dalami `src/database/*`

Kenapa urutannya begitu?

- karena kamu akan paham flow game dulu
- setelah flow paham, baru data dan helper terasa masuk akal

Tips tambahan:

- cari fungsi dari namanya dulu
- baca parameter dan return type
- lihat siapa yang memanggil fungsi itu
- jangan langsung membaca semua file dari atas sampai bawah

---

## 17. Tips Debugging

Kalau ada masalah, ini titik cek tercepat:

### Problem: save tidak kebaca

Cek:

- `loadSave(ctx)`
- `playerFromJson(...)`
- path save

### Problem: item tidak muncul

Cek:

- `game_data.json`
- `getItemById(...)`
- category item
- filter di shop/crafting/inventory

### Problem: damage aneh

Cek:

- `calculatePlayerAttack(...)`
- `calculatePlayerDefense(...)`
- `calculateEnemyDamage(...)`
- `calculateEnemyDefense(...)`
- `resolveSkillUse(...)`

### Problem: depth tidak bisa dipilih

Cek:

- `selectDungeonDepth(...)`
- `isDepthLevelAllowed(...)`
- `ensureDungeonProgress(...)`

### Problem: item craftable tapi tidak bisa di-craft

Cek:

- `canCraftItem(...)`
- `inventoryCount(...)`
- data `crafting.ingredients`

---

## 18. Kesimpulan Arsitektur

Arsitektur proyek ini bisa diringkas begini:

- `globals.h` mendefinisikan bentuk data
- `database` mengurus file JSON dan lookup data
- `state helpers` mengurus rumus, input, UI, inventory, progress
- `state files` mengurus flow menu
- `main.cpp` hanya mengatur loop state

Kalau kamu ingin menjaga proyek ini tetap enak dipelajari, pegang prinsip ini:

- satu file = satu tanggung jawab utama
- data konten = JSON
- aturan game = C++
- UI jangan bercampur terlalu jauh dengan serialisasi
- fitur baru harus mengikuti alur `GameContext`

---

## 19. Saran Pengembangan Selanjutnya

Kalau nanti kamu ingin proyek ini makin matang, fitur yang cocok ditambah berikutnya:

- quest system
- NPC dialogue
- equipment comparison screen
- passive skill system
- status effect data driven dari JSON
- enemy skill system
- chest/loot room
- map event di antara langkah dungeon
- auto save
- unit test untuk rumus damage dan save/load

---

## 20. Penutup

Kalau kamu sedang belajar C++, proyek ini sudah punya fondasi yang bagus untuk latihan software engineering dasar:

- data model
- modularisasi
- parsing file
- game loop
- state management
- balancing system

Cara terbaik mempelajarinya bukan dengan menghafal semua file, tetapi dengan mengikuti satu flow end-to-end.

Contoh latihan yang bagus:

1. Ikuti alur `New Game`
2. Ikuti alur `Enter Dungeon`
3. Ikuti alur `Attack`
4. Ikuti alur `Save`

Kalau empat flow itu sudah kamu pahami, kamu sudah mengerti tulang punggung proyek ini.
