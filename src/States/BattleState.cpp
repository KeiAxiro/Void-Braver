#include "States/BattleState.h"
#include "States/HubState.h"
#include "Entities/Item.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <random>

using namespace std;
namespace States {

    // Konstruktor: Inisialisasi state pertarungan dengan data dungeon dan mereset status battle
    BattleState::BattleState(Core::GameContext& ctx, int dungeonId, int depth)
        : context(ctx), currentDungeonId(dungeonId), dungeonDepth(depth), battlePhase(0), currentMenu(0), selectedOption(-1),
          playerShielding(false), enemyShielding(false) {}

    // Fungsi untuk memunculkan musuh secara acak berdasarkan ID dungeon dan kedalaman
    void BattleState::spawnEnemy() {
        // Ambil daftar musuh yang valid untuk lantai ini
        auto validEnemies = Entities::getEnemiesByDungeonAndDepth(currentDungeonId, dungeonDepth);
        if (validEnemies.empty()) {
            validEnemies = Entities::getAllEnemyTemplates(); // Fallback kalau gak ada musuh spesifik
        }

        // Setup RNG (Random Number Generator) standar C++11
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, validEnemies.size() - 1);
        
        // Pilih musuh secara acak dan sesuaikan stats-nya dengan level player
        Entities::EnemyTemplate selectedTmpl = validEnemies[dist(gen)];
        currentEnemy.loadFromTemplate(selectedTmpl, context.player.level);
        
        battleLog = "A wild " + currentEnemy.name + " (Lv." + to_string(currentEnemy.level) + ") appeared!";
    }

    // Inisialisasi awal saat masuk ke battle state
    void BattleState::Init() {
        spawnEnemy();
        
        // Penentuan siapa yang jalan duluan berdasarkan perbandingan level
        // Phase 1 = Player Turn, Phase 2 = Enemy Turn
        if (context.player.level >= currentEnemy.level) {
            battlePhase = 1;
            battleLog += "\n" + context.player.name + " takes the initiative!";
        } else {
            battlePhase = 2;
            battleLog += "\nEnemy level is higher. " + currentEnemy.name + " strikes first!";
        }
    }

    // Mengurus input dari pemain tergantung di phase atau menu apa mereka berada
    void BattleState::HandleInput(Core::StateManager& stateManager) {
        if (battlePhase == 1) { // Giliran Player
            string input;
            // Menampilkan prompt sesuai menu yang sedang aktif (0: Main, 1: Skill, 2: Item)
            if (currentMenu == 0) cout << "\n Choose action: ";
            else if (currentMenu == 1) cout << "\n Select Skill (0 to Cancel): ";
            else if (currentMenu == 2) cout << "\n Select Item (0 to Cancel): ";
            cin >> input;
            
            // Konversi input string ke integer, pakai try-catch biar game gak crash kalau diisi huruf
            try { selectedOption = stoi(input); } 
            catch (...) { selectedOption = -1; }
        } else if (battlePhase == 3) { // Battle Selesai
            string input;
            cout << "\n Press 0 to return to Hub: ";
            cin >> input;
            if (input == "0") selectedOption = 0;
            else selectedOption = -1;
        } else if (battlePhase == 2) { // Giliran Musuh / Transisi
            Utils::ConsoleUI::Pause(); // Tunggu player pencet Enter
        }
    }

    // Logika AI (Artificial Intelligence) untuk giliran musuh
    void BattleState::processEnemyTurn() {
        enemyShielding = false; // Reset status bertahan musuh
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(1, 20);
        int rng = dist(gen);

        int aiDecision = 1; // 1 = Attack, 2 = Defend, 4 = Flee

        // AI Khusus Boss
        if (currentEnemy.isBoss) {
            if (playerShielding) {
                // Boss lebih cenderung bertahan kalau player juga bertahan dan HP boss tipis
                if (currentEnemy.hp <= 0.5 * currentEnemy.maxHp) aiDecision = 2; 
                else aiDecision = 1;
            } else {
                // Boss agresif kalau HP player lebih kecil
                if (context.player.hp <= currentEnemy.hp) aiDecision = 1;
                else if (currentEnemy.hp < 0.25 * currentEnemy.maxHp) aiDecision = 2; // sekarat = bertahan
                else aiDecision = (rng <= 10) ? 1 : 2; // 50/50 chance
            }
        } 
        // AI Musuh Biasa
        else {
            if (playerShielding) {
                // Musuh biasa bisa kabur (decision 4) kalau HP sekarat
                if (currentEnemy.hp <= 0.2 * currentEnemy.maxHp && rng <= 8) aiDecision = 4; 
                else aiDecision = 2; 
            } else {
                if (currentEnemy.hp <= 0.1 * currentEnemy.maxHp && rng <= 8) aiDecision = 4;
                else if (rng <= 4 || context.player.hp <= currentEnemy.hp) aiDecision = 1;
                else if (currentEnemy.hp < 0.25 * currentEnemy.maxHp) aiDecision = 2;
                else aiDecision = 1;
            }
        }

        // Eksekusi keputusan AI
        if (aiDecision == 1) { // Attack
            // Kalkulasi damage: pertimbangkan status shielding player (defense x2)
            int pDef = playerShielding ? context.player.getDefense() * 2 : context.player.getDefense();
            int dmg = max(1, currentEnemy.atk - pDef); // Minimal damage = 1
            context.player.hp -= dmg;
            if (context.player.hp < 0) context.player.hp = 0;
            battleLog = currentEnemy.name + " attacks! " + context.player.name + " takes " + to_string(dmg) + " damage.";
        } else if (aiDecision == 2) { // Defend
            enemyShielding = true;
            battleLog = currentEnemy.name + " takes a defensive stance (Shielding).";
        } else if (aiDecision == 4) { // Flee
            battleLog = currentEnemy.name + " flees from the battle!";
            battlePhase = 3; // Langsung ke end battle phase
            return;
        }

        // Cek kematian player
        if (context.player.hp <= 0) { 
            battleLog += "\n\n" + context.player.name + " has fallen in battle...";
            battlePhase = 3;
        } else {
            battlePhase = 1; // Balik ke giliran player
        }
    }

    // Menghitung EXP dan drop item setelah musuh mati
    void BattleState::calculateRewards() {
        battleLog = currentEnemy.name + " defeated!\n";
        
        // Kalkulasi EXP berdasar kedalaman dungeon
        Entities::DungeonExp expData = Entities::getDungeonDepthExp(dungeonDepth);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distExp(expData.minExp, expData.maxExp);
        int gainedExp = distExp(gen);
        
        battleLog += "Gained " + to_string(gainedExp) + " EXP.\n";
        context.player.addExp(gainedExp);

        // Drop Spesifik Boss (Guaranteed 100%)
        if (currentEnemy.id == 51) {
            context.player.addItemToInventory("Crimson Orb", Entities::getCategoryName(6));
            battleLog += "Obtained Boss Reward: Crimson Orb\n";
        } else if (currentEnemy.id == 52) {
            context.player.addItemToInventory("Azure Orb", Entities::getCategoryName(6));
            battleLog += "Obtained Boss Reward: Azure Orb\n";
        } else if (currentEnemy.id == 53) {
            context.player.addItemToInventory("Obsidian Orb", Entities::getCategoryName(6));
            battleLog += "Obtained Boss Reward: Obsidian Orb\n";
        } else if (currentEnemy.id == 54) {
            context.player.addItemToInventory("Emerald Orb", Entities::getCategoryName(6));
            battleLog += "Obtained Boss Reward: Emerald Orb\n";
        } else if (currentEnemy.id == 55) {
            context.player.addItemToInventory("Radiant Orb", Entities::getCategoryName(6));
            battleLog += "Obtained Boss Reward: Radiant Orb\n";
        }

        // Drop Random Musuh Biasa (Chance 30% per item di loot table)
        auto drops = Entities::getEnemyDrops(currentEnemy.id);
        for (const auto& drop : drops) {
            uniform_real_distribution<> distChance(0.0, 1.0);
            if (distChance(gen) <= 0.3f) { 
                Entities::ItemTemplate item = Entities::getItemTemplate(drop.itemId);
                context.player.addItemToInventory(item.name, Entities::getCategoryName(item.categoryId));
                battleLog += "Looted: " + item.name + "\n";
            }
        }
    }

    // Loop utama logika battle
    void BattleState::Update(Core::StateManager& stateManager) {
        if (battlePhase == 2) {
            processEnemyTurn();
        } else if (battlePhase == 1) { // Giliran Player
            if (currentMenu == 0) { // Main Menu Action
                if (selectedOption == 1) { // Attack Biasa
                    playerShielding = false;
                    int eDef = enemyShielding ? currentEnemy.def * 2 : currentEnemy.def;
                    int dmg = max(1, context.player.getAtkPower() - eDef);
                    
                    // Cek kemungkinan Critical Hit
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_real_distribution<> dist(0.0, 1.0);
                    if (dist(gen) <= context.player.getCritRate()) {
                        dmg = static_cast<int>(dmg * 1.5f); // Crit damage multiplier x1.5
                        battleLog = "CRITICAL HIT! ";
                    } else {
                        battleLog = "";
                    }

                    currentEnemy.takeDamage(dmg);
                    battleLog += context.player.name + " attacks for " + to_string(dmg) + " damage.";
                    selectedOption = -1;

                    // Mengurangi Cooldown skill setiap kali player mengambil aksi
                    for (auto& sk : context.player.skills) {
                        if (sk.currentCd > 0) sk.currentCd--;
                    }
                    
                    if (currentEnemy.isDead()) {
                        battlePhase = 3;
                        calculateRewards();
                    } else {
                        battlePhase = 2; // Oper turn ke musuh
                    }
                } else if (selectedOption == 2) { // Defend
                    playerShielding = true;
                    battleLog = context.player.name + " guards against the next attack.";
                    selectedOption = -1;

                    for (auto& sk : context.player.skills) {
                        if (sk.currentCd > 0) sk.currentCd--;
                    }
                    battlePhase = 2;
                } else if (selectedOption == 3) { // Buka Menu Skill
                    currentMenu = 1; 
                    selectedOption = -1;
                } else if (selectedOption == 4) { // Buka Menu Item
                    currentMenu = 2; 
                    selectedOption = -1;
                } else if (selectedOption == 5) { // Logika Flee / Kabur (40% sukses)
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_int_distribution<> dist(1, 100);
                    if (dist(gen) <= 40) { 
                        battleLog = "Successfully fled the battle.";
                        battlePhase = 3;
                    } else {
                        battleLog = "Failed to flee!";
                        battlePhase = 2;
                    }
                    selectedOption = -1;

                    for (auto& sk : context.player.skills) {
                        if (sk.currentCd > 0) sk.currentCd--;
                    }
                }
            } else if (currentMenu == 1) { // Menu Eksekusi Skill
                if (selectedOption == 0) { // Cancel/Back
                    currentMenu = 0; selectedOption = -1;
                } else if (selectedOption > 0 && selectedOption <= static_cast<int>(context.player.skills.size())) {
                    auto& skill = context.player.skills[selectedOption - 1];
                    
                    // Cek apakah skill sudah unlock, MP cukup, dan tidak cooldown
                    if (skill.isUnlocked && context.player.mp >= skill.manaCost && skill.currentCd == 0) {
                        context.player.mp -= skill.manaCost;
                        playerShielding = false;
                        
                        // Sistem baca damage multiplier negatif sebagai spell Healing
                        if (skill.damageMultiplier < 0) {
                            int healAmt = static_cast<int>(-skill.damageMultiplier * context.player.getAtkPower());
                            context.player.hp = min(context.player.hp + healAmt, context.player.getMaxHp());
                            battleLog = context.player.name + " uses " + skill.name + " and heals " + to_string(healAmt) + " HP.";
                        } else { // Serangan Skill
                            int baseDmg = static_cast<int>(skill.damageMultiplier * context.player.getAtkPower());
                            int eDef = enemyShielding ? currentEnemy.def * 2 : currentEnemy.def;
                            int dmg = max(1, baseDmg - eDef);
                            currentEnemy.takeDamage(dmg);
                            battleLog = context.player.name + " uses " + skill.name + " for " + to_string(dmg) + " damage.";
                        }

                        // Mengurangi CD semua skill sebelum menset CD skill yang baru dipakai
                        for (auto& sk : context.player.skills) {
                            if (sk.currentCd > 0) sk.currentCd--;
                        }
                        
                        skill.currentCd = skill.maxCooldown; // Mulai cooldown skill ini
                        
                        selectedOption = -1; currentMenu = 0;
                        
                        if (currentEnemy.isDead()) {
                            battlePhase = 3; calculateRewards();
                        } else {
                            battlePhase = 2; 
                        }
                    } else {
                        selectedOption = -1; // Kalau gagal (MP gak cukup / masih CD), reset opsi
                    }
                } else {
                    selectedOption = -1;
                }
            } else if (currentMenu == 2) { // Menu Eksekusi Item Consumable
                if (selectedOption == 0) { // Cancel/Back
                    currentMenu = 0; selectedOption = -1;
                } else {
                    // Mapping urutan display ke index asli di inventory player
                    int consumeIdx = 1;
                    int targetIdx = -1;
                    for (int i = 0; i < static_cast<int>(context.player.inventory.size()); ++i) {
                        if (context.player.inventory[i].type == "Consumable") {
                            if (consumeIdx == selectedOption) { targetIdx = i; break; }
                            consumeIdx++;
                        }
                    }
                    
                    if (targetIdx != -1) {
                        auto& item = context.player.inventory[targetIdx];
                        
                        // Hardcode efek item. Healing/Mana Elixir me-restore 30% dari stat maksimum
                        if (item.name == "Healing Potion") {
                            int heal = context.player.getMaxHp() * 0.3f;
                            context.player.hp = min(context.player.hp + heal, context.player.getMaxHp());
                            battleLog = "Used Healing Potion. Recovered " + to_string(heal) + " HP.";
                        } else if (item.name == "Mana Elixir") {
                            int healMp = context.player.getMaxMp() * 0.3f;
                            context.player.mp = min(context.player.mp + healMp, context.player.getMaxMp());
                            battleLog = "Used Mana Elixir. Recovered " + to_string(healMp) + " MP.";
                        }
                        
                        // Kurangi jumlah, hapus dari tas kalau habis
                        item.quantity--;
                        if (item.quantity <= 0) context.player.inventory.erase(context.player.inventory.begin() + targetIdx);
                        
                        selectedOption = -1; currentMenu = 0;

                        for (auto& sk : context.player.skills) {
                            if (sk.currentCd > 0) sk.currentCd--;
                        }

                        battlePhase = 2; // Pakai item memakan 1 turn
                    } else {
                        selectedOption = -1;
                    }
                }
            }
        } else if (battlePhase == 3) { // End of Battle
            if (selectedOption == 0) {
                stateManager.PopState(); // Buang battle state, kembali ke state sebelumnya (biasanya Hub/Dungeon state)
            }
        }
    }

    // Sistem rendering UI teks ke konsol
    void BattleState::Render() {
        Utils::ConsoleUI::ClearScreen();
        
        // Header
        cout << string(60, '=') << "\n";
        cout << "                  D U N G E O N   D E P T H : " << dungeonDepth << "\n";
        cout << string(60, '=') << "\n\n";

        // Info Musuh
        cout << "  [ENEMY] " << currentEnemy.name << " (Lv." << currentEnemy.level << ")" << (currentEnemy.isBoss ? " [BOSS]" : "") << "\n";
        cout << "  HP : " << currentEnemy.hp << " / " << currentEnemy.maxHp << "\n";
        if (enemyShielding) cout << "  (Status: Guarding)\n"; // Tanda visual kalau AI defend
        cout << "\n";

        // Info Player
        cout << "  [PLAYER] " << context.player.name << " (Lv." << context.player.level << ")\n";
        cout << "  HP : " << context.player.hp << " / " << context.player.getMaxHp() << "\n";
        cout << "  MP : " << context.player.mp << " / " << context.player.getMaxMp() << "\n";
        if (playerShielding) cout << "  (Status: Guarding)\n";
        cout << string(60, '-') << "\n";

        // Battle Log / Notifikasi aksi
        cout << "  > " << battleLog << "\n";
        cout << string(60, '-') << "\n";

        // Render Menu Berdasarkan Phase/Menu State
        if (battlePhase == 1) {
            if (currentMenu == 0) {
                cout << "  [1] Attack    [2] Defend\n";
                cout << "  [3] Skill     [4] Item\n";
                cout << "  [5] Flee\n";
            } else if (currentMenu == 1) { // Render Menu Skill
                cout << "  --- SKILLS ---\n";
                if (context.player.skills.empty()) {
                    cout << "  (No skills available)\n";
                } else {
                    for (size_t i = 0; i < context.player.skills.size(); ++i) {
                        const auto& sk = context.player.skills[i];
                        cout << "  [" << (i+1) << "] " << sk.name << " (MP: " << sk.manaCost << ")";
                        
                        // Penanda status skill (Terkunci atau sedang Cooldown)
                        if (!sk.isUnlocked) cout << " [LOCKED]";
                        else if (sk.currentCd > 0) cout << " [CD: " << sk.currentCd << "]";
                        cout << "\n";
                    }
                }
                cout << "  [0] Back\n";
            } else if (currentMenu == 2) { // Render Menu Item Consumable
                cout << "  --- CONSUMABLES ---\n";
                int displayIdx = 1;
                bool hasItem = false;
                for (const auto& item : context.player.inventory) {
                    // Filter: Hanya tampilkan barang "Consumable"
                    if (item.type == "Consumable") {
                        cout << "  [" << displayIdx << "] " << item.name << " (x" << item.quantity << ")\n";
                        displayIdx++;
                        hasItem = true;
                    }
                }
                if (!hasItem) cout << "  (Inventory is empty)\n";
                cout << "  [0] Back\n";
            }
        } else if (battlePhase == 2) {
            cout << "\n  [Press Enter to continue...]\n";
        } else if (battlePhase == 3) { // Akhir Pertarungan
            cout << "\n  Battle Concluded. Press 0 to return.\n";
        }
        cout << string(60, '=') << "\n";
    }
}