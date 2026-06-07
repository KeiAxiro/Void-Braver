# Fitur Project Yang Memakai Materi Struktur Data

Catatan: nomor line mengacu pada kondisi source saat laporan ini dibuat. Beberapa fitur memakai materi yang sama lewat struktur data pendukung, jadi ada beberapa line yang sengaja muncul di lebih dari satu materi.

## Pertemuan 2: Array

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Unlock level skill berdasarkan slot | `Config::Skill::UNLOCK_LEVELS[]`, `UNLOCK_LEVEL_COUNT` | `include/config/skill.h:18-19` | Array konstan berisi level unlock skill per urutan slot. |
| Lookup unlock skill | `skillUnlockLevelForIndex` | `src/states/character_inventory.cpp:169-179` | Mengubah index skill menjadi index array unlock level. |
| Lookup data game dari JSON array | `findInArrayById`, `getClassById`, `getItemById`, `getSkillById`, `getDungeonById`, `getEnemyById`, `findDungeonDepth`, `getDepthDialog` | `src/database/data_lookup.cpp:12-98` | Iterasi array JSON seperti `classes`, `items`, `skills`, `dungeons`, `enemies`, dan `depths`. |
| Save/load array karakter dan inventory | `playerToJson`, `fillInventory`, `fillCooldowns`, `loadSave`, `writeCharactersToSave` | `src/database/json_handler.cpp:190-247`, `278-307`, `381-408`, `503-549` | Membaca/menulis `inventory`, `cooldowns`, dan `characters` sebagai array JSON. |
| Pilih class saat New Game | `runMainMenu` | `src/states/menu_hub.cpp:856-873` | Menampilkan `ctx.gameData["classes"]` berdasarkan index array. |
| Jadwal encounter lama | `buildEncounterStepsForMove` | `src/states/battle_dungeon.cpp:447-470` | Memakai `bool selectedSteps[]` untuk menandai step encounter yang sudah dipilih. |
| Graph route dungeon berbasis matrix | `DungeonGraphMatrix`, `dijkstraDungeonRoute`, `kruskalDungeonRouteCost` | `src/states/menu_hub.cpp:389-395`, `445-545` | Memakai array 1D/2D: `ids[]`, `weights[][]`, `distance[]`, `visited[]`, `edges[]`, dan `parent[]`. |

**Penjelasan:**  
Array dipakai untuk data yang butuh akses berdasarkan index tetap: tabel unlock skill, matrix graph dungeon, dan buffer algoritma Dijkstra/Kruskal. Selain itu, banyak data game disimpan sebagai JSON array, lalu diiterasi untuk lookup class, item, enemy, dungeon, skill, dan save character.

## Pertemuan 4: Pointer

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Container manual double linked list | `ManualList<T>`, `Node *prev`, `Node *next`, `head_`, `tail_` | `include/globals.h:21-205` | Pointer menghubungkan node dua arah. |
| Stack state game | `StateStack`, `push_back`, `pop_back`, `back` | `include/globals.h:221-277` | Pointer `top_` menunjuk node paling atas stack. |
| Queue encounter | `EncounterQueue`, `enqueue`, `dequeue` | `include/state_helpers.h:24-108` | Pointer `front_` dan `rear_` mengatur antrian node. |
| Lookup data tanpa copy JSON | `const json *findInArrayById`, `getClassById`, `getItemById`, `getDungeonById`, dll. | `src/database/data_lookup.cpp:12-98`, `177-209`, `237-264` | Fungsi mengembalikan pointer ke elemen JSON, atau `nullptr` jika tidak ditemukan. |
| Skill tree | `SkillTreeNode`, `buildSkillTree`, `findSkillInTree`, `clearSkillTree`, `printSkillTreeInOrder` | `include/state_helpers.h:110-117`, `src/states/character_inventory.cpp:186-240` | Pointer `left` dan `right` membentuk binary tree skill. |
| Dungeon map tree | `DungeonMapNode`, `buildDungeonMapNode`, `buildDungeonMap`, `clearDungeonMap`, `resolveDungeonMapNodeEncounter` | `include/state_helpers.h:175-183`, `src/states/battle_dungeon.cpp:473-738` | Pointer `left` dan `right` menjadi cabang jalur dungeon. |
| Graph adjacency list dungeon | `DungeonGraphVertex`, `DungeonGraphEdge`, `findDungeonVertex`, `addDungeonVertex`, `addDungeonEdge`, `clearDungeonGraph` | `src/states/menu_hub.cpp:304-387` | Pointer `next` dan `edges` membentuk linked list vertex dan edge. |

**Penjelasan:**  
Pointer adalah fondasi struktur data manual di project ini. Node list, stack, queue, tree, dan graph semuanya dialokasikan dengan `new`, dihubungkan lewat pointer, lalu dibersihkan dengan `delete` lewat fungsi `clear` masing-masing.

## Pertemuan 6: Single Linked List

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Stack state game berbasis single linked list | `StateStack::Node`, `push_back`, `pop_back`, `clear` | `include/globals.h:221-277` | Tiap node hanya punya `next`; push/pop dilakukan dari `top_`. |
| Queue encounter berbasis single linked list | `EncounterQueue::Node`, `contains`, `enqueue`, `dequeue`, `clear` | `include/state_helpers.h:24-108` | Node berjalan satu arah dari `front_` ke `rear_`. |
| Graph vertex dan edge list | `DungeonGraphVertex`, `DungeonGraphEdge`, `findDungeonVertex`, `addDungeonVertex`, `addDungeonEdge`, `clearDungeonGraph` | `src/states/menu_hub.cpp:304-387` | Vertex graph dan daftar edge disimpan sebagai linked list satu arah. |

**Penjelasan:**  
Single linked list muncul pada struktur yang hanya perlu maju satu arah. `StateStack` cukup menelusuri dari top, `EncounterQueue` cukup maju dari front ke next, dan graph adjacency list cukup menyambungkan vertex/edge lewat pointer `next`.

## Pertemuan 7: Double Linked List

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Implementasi list manual utama | `ManualList<T>`, `push_back`, `erase`, `clear`, `swapData`, `manualSort` | `include/globals.h:21-219` | Double linked list generic untuk banyak data runtime. |
| Alias list yang dipakai project | `PlayerList`, `StringList`, `IndexList`, `JsonPointerList` | `include/globals.h:343-346` | Nama khusus untuk list karakter, string, index, dan pointer JSON. |
| Inventory dasar | `inventoryCount`, `mergeInventory`, `addItem`, `removeItem`, `itemBonusFromEquipped` | `src/states/menu_hub.cpp:101-204` | Menambah, menghapus, menggabungkan, dan membaca bonus item dari `ManualList<InventoryEntry>`. |
| Inventory menu | `collectInventoryCategories`, `collectInventoryIndexes`, `equipInventoryEntry`, `unequipInventoryEntry`, `useConsumable`, `inventoryMenu` | `src/states/character_inventory.cpp:302-705` | Kategori, pagination, equip/unequip, dan consumable memakai list inventory dan list index. |
| Skill cooldown | `getCooldown`, `setCooldown`, `reduceCooldowns`, `resolveSkillUse`, `tickBattleEffects` | `src/states/menu_hub.cpp:206-233`, `src/states/battle_dungeon.cpp:1122-1153`, `1251-1275` | Cooldown skill disimpan di `ManualList<CooldownEntry>`. |
| Progress dungeon per character | `ensureDungeonProgress`, `getDungeonUnlockedDepth`, `advanceDungeonProgress` | `src/states/menu_hub.cpp:241-289`, `src/states/battle_dungeon.cpp:1057-1077` | Progress tiap dungeon disimpan di `ManualList<DungeonProgressEntry>`. |
| Save/load daftar character | `upsertCharacter`, `writeCharactersToSave`, `loadSave`, `loadCharacter`, `saveGame`, `deleteCharacter` | `src/database/json_handler.cpp:361-408`, `503-631` | `PlayerList` dipakai untuk menyimpan, memilih, update, dan menghapus character. |
| Crafting dan shop | `collectCraftingCategories`, `categoriesForShopMode`, `craftingMenu`, `shopBuyMenu`, `shopSellMenu` | `src/states/character_inventory.cpp:758-822`, `855-1402` | Kategori, recipe, item shop, dan index item memakai `StringList`, `JsonPointerList`, dan `IndexList`. |
| Drop enemy | `EnemyInstance::drops`, `spawnEnemyForDepth`, `grantEnemyDrops` | `include/state_helpers.h:119-135`, `src/states/battle_dungeon.cpp:326-423`, `1040-1055` | Drop musuh disimpan di `DropList`, lalu diproses saat reward battle. |

**Penjelasan:**  
Double linked list adalah container utama project. Hampir semua list gameplay penting, seperti inventory, cooldown, progress dungeon, karakter tersimpan, recipe crafting, shop, pool enemy, dan drop musuh, memakai `ManualList<T>`. Operasi pentingnya terlihat pada `push_back`, `erase`, `clear`, dan `manualSort`.

## Pertemuan 9: Stack (Tumpukan)

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Stack state game | `StateStack`, `back`, `push_back`, `pop_back`, `clear` | `include/globals.h:221-277` | Menyimpan state aktif game dengan prinsip LIFO. |
| Dispatcher state | `dispatchCurrentState`, `runGame` | `src/game.cpp:10-27`, `47-50` | Mengambil state teratas lewat `ctx.stateStack.back()`. |
| Masuk battle dari pilih dungeon | `selectDungeonDepth` | `src/states/battle_dungeon.cpp:170-177` | Push `GameState::Battle` ke stack saat player masuk dungeon. |
| Keluar battle | lambda `leaveBattleState` di `runBattle` | `src/states/battle_dungeon.cpp:1497-1504` | Pop `GameState::Battle` agar kembali ke state sebelumnya. |
| Navigasi main menu/hub | `runMainMenu`, `runHub` | `src/states/menu_hub.cpp:889-890`, `967-968`, `1041-1042` | Stack di-reset lalu diisi state tujuan seperti `Hub` atau `MainMenu`. |

**Penjelasan:**  
Stack dipakai sebagai pengatur alur layar. State terakhir yang masuk menjadi state yang diproses lebih dulu. Saat masuk dungeon, `Battle` dipush; saat selesai atau keluar battle, state tersebut dipop.

## Pertemuan 10: Queue (Antrian)

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Queue encounter manual | `EncounterQueue`, `contains`, `enqueue`, `dequeue`, `clear` | `include/state_helpers.h:24-108` | Implementasi FIFO untuk menyimpan urutan step encounter. |
| Builder jadwal encounter | `buildEncounterStepsForMove` | `src/states/battle_dungeon.cpp:447-470` | Membuat daftar step encounter, lalu memasukkannya dengan `enqueue`. |

**Penjelasan:**  
Queue disiapkan untuk jadwal encounter berurutan: data masuk lewat `enqueue` dan keluar lewat `dequeue`. Dari pencarian source saat ini, flow eksplorasi baru memakai dungeon map node, sehingga `buildEncounterStepsForMove` terlihat sebagai helper eksplorasi lama/pendukung yang belum terpanggil langsung dari `runBattle`.

## Pertemuan 11: Tree - Part I

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Struktur binary skill tree | `SkillTreeNode` | `include/state_helpers.h:110-117` | Node skill punya `left` dan `right`. |
| Membentuk skill tree dari daftar skill | `buildSkillTree` | `src/states/character_inventory.cpp:186-199` | Mengambil elemen tengah sebagai root, lalu rekursif membangun kiri dan kanan. |
| Menu skill karakter | `skillMenu` | `src/states/character_inventory.cpp:242-258` | Membuat tree skill dan menampilkannya ke player. |
| Tampilan skill saat battle | `showBattleSkills` | `src/states/battle_dungeon.cpp:1454-1474` | Membuat tree skill untuk daftar skill battle. |

**Penjelasan:**  
Tree Part I paling jelas ada di skill list. Data skill yang awalnya array JSON diubah menjadi binary tree agar bisa ditampilkan dan dicari berdasarkan nomor skill. Root dipilih dari index tengah supaya bentuk tree lebih seimbang.

## Pertemuan 12: Tree - Part II

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Search skill di binary tree | `findSkillInTree` | `src/states/character_inventory.cpp:201-210` | Mencari skill dengan aturan kiri jika index lebih kecil, kanan jika lebih besar. |
| Traversal InOrder skill tree | `printSkillTreeInOrder` | `src/states/character_inventory.cpp:221-240` | Menampilkan skill berurutan dari subtree kiri, root, lalu subtree kanan. |
| Cleanup tree | `clearSkillTree` | `src/states/character_inventory.cpp:212-219` | Menghapus node tree secara post-order. |
| Pemakaian search saat battle | skill selection di `runEncounterBattle` | `src/states/battle_dungeon.cpp:935-980` | Saat player memilih skill, tree dibangun, dicari dengan `findSkillInTree`, dipakai, lalu dibersihkan. |
| Dungeon map tree | `DungeonMapNode`, `buildDungeonMapNode`, `collectDungeonMapLeaves`, `collectDungeonMapNodes`, `buildDungeonMap`, `clearDungeonMap` | `include/state_helpers.h:175-183`, `src/states/battle_dungeon.cpp:473-655` | Tree jalur dungeon dibuat, dikumpulkan leaf/node-nya, lalu dibersihkan setelah eksplorasi. |
| Traversal interaktif dungeon map | `renderDungeonMapView`, `resolveDungeonMapNodeEncounter`, `runBattle` | `src/states/battle_dungeon.cpp:699-738`, `1610-1727` | Player berjalan dari node ke `left`/`right` sampai final node. |

**Penjelasan:**  
Tree Part II muncul pada operasi lanjutan: search, traversal, dan cleanup. Skill tree memakai binary search dan InOrder traversal, sedangkan dungeon map memakai tree traversal untuk membangun jalur, mencari leaf, menampilkan cabang, dan membersihkan node agar tidak leak memory.

## Pertemuan 14: Graph - Part I

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Representasi graph dungeon manual | `DungeonGraphVertex`, `DungeonGraphEdge` | `src/states/menu_hub.cpp:304-318` | Vertex menyimpan daftar edge sebagai adjacency list. |
| Operasi vertex dan edge | `findDungeonVertex`, `addDungeonVertex`, `addDungeonEdge`, `clearDungeonGraph` | `src/states/menu_hub.cpp:320-350`, `372-387` | Menambah/mencari node graph dan membersihkan memory graph. |
| DFS prerequisite final dungeon | `dfsDungeonGraph`, `graphHasLockedPrerequisite` | `src/states/menu_hub.cpp:352-370` | DFS menandai dungeon yang terhubung dari final dungeon, lalu cek yang belum selesai. |
| Lock final dungeon | `isFinalDungeonLocked` | `src/states/menu_hub.cpp:547-580` | Membangun graph semua dungeon, menjalankan DFS, lalu menentukan final dungeon terkunci atau tidak. |

**Penjelasan:**  
Graph Part I dipakai untuk fitur unlock final dungeon. Setiap dungeon menjadi vertex, hubungan prerequisite menjadi edge, lalu DFS memastikan semua dungeon non-final yang terhubung sudah selesai sebelum final dungeon terbuka.

## Pertemuan 15: Graph - Part II

| Fitur | Fungsi / Struktur | File dan line | Ringkasnya |
|---|---|---:|---|
| Adjacency matrix dungeon | `DungeonGraphMatrix`, `findDungeonMatrixIndex`, `addDungeonMatrixVertex`, `addDungeonMatrixEdge`, `buildDungeonGraphMatrix` | `src/states/menu_hub.cpp:389-443` | Graph dungeon juga direpresentasikan sebagai matrix bobot rute. |
| Shortest path ringan | `dijkstraDungeonRoute` | `src/states/menu_hub.cpp:445-482` | Menghitung biaya rute terpendek memakai array `distance` dan `visited`. |
| Minimum spanning tree ringan | `MatrixEdge`, `findParent`, `kruskalDungeonRouteCost` | `src/states/menu_hub.cpp:484-545` | Mengurutkan edge dan memakai parent array untuk menghindari siklus. |
| Pemakaian algoritma graph lanjutan | `isFinalDungeonLocked` | `src/states/menu_hub.cpp:582-590` | Matrix dibangun dan fungsi Dijkstra/Kruskal dipanggil sebagai kalkulasi rute dungeon. |

**Penjelasan:**  
Graph Part II terlihat pada algoritma berbasis bobot. Project memakai adjacency matrix untuk menyimpan biaya rute antar dungeon, lalu menjalankan Dijkstra untuk rute terpendek dan Kruskal untuk total biaya rute stabil/MST ringan.
