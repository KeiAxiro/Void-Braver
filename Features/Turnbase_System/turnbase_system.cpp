#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <algorithm>
// Note:
// Testing (blm)
// Bug Fix (blm nemu)

// Dummy Data
struct StatusEffect {
    std::string name;
    int duration;
};

struct Skill {
    std::string name;
    int damage;
    int maxCooldown;
    int mpCost;
    StatusEffect inflict;
    int currentCd = 0;
};

struct Item {
    std::string name;
    int healAmount;
    int quantity;
};

struct OptionSkill {
    std::vector<int> avaliable_skill; // index skill
    int maxSkilDamage_index = -1; // Set ke -1 agar tahu jika tidak ada skill damage
    std::vector<int> healSkil_index; // index skill
    std::vector<int> statusSkil_index; // index skill

};
struct Entity {
    std::string name;
    int hp, maxHp;
    int mp, maxMp;
    float attack, defense;
    float baseATK, baseDEF;
    int spd, baseSPD;
    std::vector<StatusEffect> statusEffects;
    std::vector<Skill> skills;
    std::vector<Item> inventory;
    OptionSkill skill_option;
    bool isShielding = false;
    bool isAttacking = false;
    bool isBoss = false;
};
//Dummy data

// Deklarasi fungsi
void        printSeparator      ();
void        printStatus         (const Entity& entiti);
bool        isDead              (const Entity& entiti);
int         randomNumber        (int a, int b);
int         clamp               (int val, int lo, int hi);
void        updateCooldown      (Entity& entiti);
bool        applyStatusEffects  (Entity& entiti);
void        findNilai           (Entity& entiti);
int         statusPriority      (Entity& enemy, Entity& player);
bool        breachExist         (const Entity& entiti);
void        actionAttack        (Entity& main_e, Entity& second_e);
void        actionDefend        (Entity& main_e);
void        actionSkill         (Entity& main_e, Entity& second_e);
bool        playerSkill         (Entity& player, Entity& enemy);
bool        actionItem          (Entity& player);
bool        actionFlee          (Entity& main_e, Entity& second_e);
int         playerTurn          (Entity& player, Entity& enemy);
int         decision            (const Entity& enemy, const Entity& player);
void        bossSkillOption     (Entity& enemie, Entity& player, int decision);
int         bossDecision        (const Entity& enemy, const Entity& player);
int         enemyTurn           (Entity& enemy, Entity& player);
void        startBattle         (Entity& player, Entity& enemy);
int         turnState           ();
void        exit                (const Entity& enemy, Entity& player);
// Deklarasi fungsi

// ============================================================
//  MAIN
// ============================================================

int main() {

    // --- Inisialisasi Player ---
    Entity player;
    player.name    = "Hero";
    player.hp      = player.maxHp = 100;
    player.mp      = player.maxMp = 50;
    player.attack  = player.baseATK = 20;
    player.defense = player.baseDEF = 8;
    player.spd     = player.baseSPD = 10;

    player.skills = {
        {"Fireball",    35, 3, 15, {"burn", 2}},
        {"Slash",       25, 2, 8},
        {"Heal Pulse",  -10, 3,  10}  // khusus heal diri, dmg = -10 -> heal = +10
    };

    // Untuk sementara cuman buat heal krn gua gak tau efek yang lainnya apa
    player.inventory = {
        {"Potion",      30, 3},
        {"Hi-Potion",   60, 1}
    };

    //test status efek
    //player.statusEffects.push_back({"burn", 2});

    // --- Inisialisasi Enemy ---
    // Entity enemy;
    // enemy.name    = "Dark Goblin";
    // enemy.hp      = enemy.maxHp   = 100;
    // enemy.mp      = enemy.maxMp   = 30;
    // enemy.attack  = enemy.baseATK = 15;
    // enemy.defense = enemy.baseDEF = 5;
    // enemy.spd     = enemy.baseSPD = 5;

    // enemy.skills = {
    //     {"Poison Bite", 20, 3, 10, {"poison", 2}},
    //     {"Power Slash", 28, 3, 12}
    // };
    // ---Test Boss---
    Entity enemy;
    enemy.name    = "Lord Goblin";
    enemy.hp      = enemy.maxHp   = 120;
    enemy.mp      = enemy.maxMp   = 50;
    enemy.attack  = enemy.baseATK = 20;
    enemy.defense = enemy.baseDEF = 10;
    enemy.spd     = enemy.baseSPD = 8;
    enemy.isBoss = true;
    enemy.skills = {
        {"Poison Bite", 20, 3, 10, {"poison", 2}},
        {"Power Slash", 28, 3, 12},
        {"Heal Pulse", -10, 4, 10}, 
        {"EarthQuake", 40, 6, 30, {"breach", 2}}
    };
    // ---Test Boss---
    //test status efek
    //enemy.statusEffects.push_back({"breach", 2});

    // --- Mulai Battle ---
    startBattle(player, enemy);

    std::cout << "\n=== end ===\n";
    return 0;
}

// ============================================================
//  UTILITY
// ============================================================

void printSeparator() {
    std::cout << "========================================\n";
}
// print status dari suatu entity
void printStatus(const Entity& entiti) {
    std::cout << "[" << entiti.name << "] HP: " << entiti.hp << "/" << entiti.maxHp
              << "  MP: " << entiti.mp << "/" << entiti.maxMp;
    if (!entiti.statusEffects.empty()) {
        std::cout << "  Status: ";
        for (auto& status : entiti.statusEffects)
            std::cout << status.name << "(" << status.duration << ") ";
    }
    std::cout << "\n";
}
// cek apakah entity mati
bool isDead(const Entity& entiti) { return entiti.hp <= 0; }
// Untuk membuat random Number
int randomNumber(int a, int b){
    static std::random_device rnd;
    static std::mt19937 engine(rnd());
    std::uniform_int_distribution<int> distribution(a, b);
    int random_num = distribution(engine);
    return random_num;
}
// untuk penyempitan nilai
int clamp(int val, int lo, int hi) {
    return std::max(lo, std::min(val, hi));
}
// Untk mengupdate cooldown dari skill
void updateCooldown(Entity& entiti){
    for (auto& sk : entiti.skills){
        if (sk.currentCd > 0){
            sk.currentCd--;
        }
    }
}
// Untuk exit dari startBattle()
void exit(const Entity& main_e, Entity& second_e){
    std::cout << "\n" << second_e.name << " kalah!\n";
        std::cout << main_e.name << " MENANG!\n";
        printSeparator();

        // Function Buat Reward
        std::cout << "[Keluar dari State TurnBase Battle]\n";
}

// ============================================================
//  CEK & TERAPKAN STATUS EFFECT
// ============================================================

// Mengembalikan true jika entity di-stun (gilirannya di-skip)
bool applyStatusEffects(Entity& entiti) {
    bool stunned = false;
    entiti.attack = entiti.baseATK;
    entiti.defense = entiti.baseDEF;
    std::vector<StatusEffect> remaining;
    std::cout << "\n";
    for (auto& status : entiti.statusEffects) {
        if (status.name == "stun") {
            std::cout << entiti.name << " terkena STUN! Giliran dilewati.\n";
            stunned = true;
        } 
        else if (status.name == "burn") {
            int dmg = 5;
            entiti.hp -= dmg;
            std::cout << entiti.name << " terkena BURN! -" << dmg << " HP.\n";
        } 
        else if (status.name == "poison") {
            int dmg = 8;
            entiti.hp -= dmg;
            std::cout << entiti.name << " terkena POISON! -" << dmg << " HP.\n";
        } 
        else if (status.name == "curse") { //kalau skill mengakibatkan efek-curse di push.back
            std::cout << entiti.name << " terkena CURSE! ATK turun sementara.\n";
            entiti.attack *= 0.85;
        }
        else if (status.name == "breach") {
            float def_reduction = std::max(entiti.defense * 0.1f, 10.0f);
            entiti.defense -= def_reduction;
            entiti.isShielding = false;
            std::cout << entiti.name << " terkena BREACH! DEF turun dan tidak bisa Guard untuk sementara.\n";
        }

        status.duration--;
        if (status.duration > 0) {
            remaining.push_back(status);
        }
        else {
            std::cout << "  -> Efek " << status.name << " pada " << entiti.name << " telah berakhir.\n";
        }
    }

    entiti.statusEffects = remaining;
    return stunned;
}
// ini buat boss (untuk sementara)
void findNilai(Entity& entiti) {
    // Reset
    entiti.skill_option.avaliable_skill.clear();
    entiti.skill_option.healSkil_index.clear();
    entiti.skill_option.statusSkil_index.clear();
    entiti.skill_option.maxSkilDamage_index = -1; 
    float currentMaxDamage = -1.0f;
    for (int i = 0; i < entiti.skills.size(); i++) {
        if (entiti.skills[i].currentCd == 0) {
            entiti.skill_option.avaliable_skill.push_back(i);

            if (entiti.skills[i].damage < 0) {entiti.skill_option.healSkil_index.push_back(i);}

            else if (entiti.skills[i].damage > currentMaxDamage) {
                currentMaxDamage = entiti.skills[i].damage;
                entiti.skill_option.maxSkilDamage_index = i;
            }

            if (!entiti.skills[i].inflict.name.empty()) {entiti.skill_option.statusSkil_index.push_back(i);}
        }
    }
}
// mencari status efek prioriti untuk boss 
int statusPriority(Entity& enemy, Entity& player){
    for (int i = 0; i < enemy.skill_option.statusSkil_index.size(); i++){
        if (player.isShielding && enemy.skills[enemy.skill_option.statusSkil_index[i]].inflict.name == "breach"){
            return enemy.skill_option.statusSkil_index[i];
        }
        else if (enemy.skills[enemy.skill_option.statusSkil_index[i]].inflict.name == "curse"){
            return enemy.skill_option.statusSkil_index[i];
        }
    }
    return -1;
}
// cek apakah ada status breach di entiti
bool breachExist(const Entity& entiti){
    for( int i = 0; i < entiti.statusEffects.size(); i++){
        if (entiti.statusEffects[i].name == "breach"){
            return true;
        }
    }
    return false;
}
// ============================================================
//  AKSI ENTITY
// ============================================================

// serangan menggunakan senjata
void actionAttack(Entity& main_e, Entity& second_e) {
    main_e.isAttacking = true;
    int dmg = std::max((float)1.0, main_e.attack - (second_e.isShielding ? second_e.defense * 2 : second_e.defense)); // untuk dmg kalkulationnya di sini
    second_e.hp -= dmg;
    second_e.hp = clamp(second_e.hp, 0, second_e.maxHp);
    std::cout << main_e.name << " menyerang " << second_e.name << " dengan senjata!\n[Damage]: " << dmg << "\n";
    main_e.isShielding = false;
}
// Bertahan
void actionDefend(Entity& main_e) {
    main_e.isShielding = true;
    std::cout << main_e.name << " bertahan menggunakan Shield!\n";
}
// Menggunakan Skill Untuk Entity secara random
void actionSkill (Entity& main_e, Entity& second_e){
    for (int i = 0; i < main_e.skills.size(); i++){
        if (main_e.skills[i].currentCd == 0 && main_e.mp >= main_e.skills[i].mpCost){
            main_e.skill_option.avaliable_skill.push_back(i);
        }
    }
    if (main_e.skill_option.avaliable_skill.empty()){
        actionAttack(main_e, second_e); 
        return;
    }

    int sk_rng = main_e.skill_option.avaliable_skill[randomNumber(0, main_e.skill_option.avaliable_skill.size() - 1)];
    // Kalkulasi dmg dan mana cost
    int dmg;
    Skill& sk = main_e.skills[sk_rng];
    if (main_e.mp < sk.mpCost) {
        actionAttack(main_e, second_e); 
        return;
    }
    main_e.mp -= sk.mpCost;
    if (sk.damage < 0) {
        dmg = sk.damage;
        main_e.hp -= dmg;
        std::cout << main_e.name << " menggunakan skill " << sk.name << "!\n[Heal]: " << -dmg << "\n";
    }
    else {
        // matematika dmg
        dmg = std::max((float)1.0, sk.damage - (second_e.isShielding ? second_e.defense * 2 : second_e.defense));
        second_e.hp -= dmg;
        second_e.hp = clamp(second_e.hp, 0, second_e.maxHp);
        std::cout << main_e.name << " menggunakan skill " << sk.name << "!\n[Damage]: " << dmg << "\n";
    }
    if (!(sk.inflict.name.empty())) {
        second_e.statusEffects.push_back({sk.inflict.name, sk.inflict.duration});
    }
    sk.currentCd = sk.maxCooldown;
    
}
// Menggunakan Skill Untuk Player
bool playerSkill(Entity& player, Entity& enemy) {
    if (player.skills.empty()) {
        std::cout << "Tidak ada skill tersedia.\n";
        return false;
    }
    std::cout << "Pilih Skill:\n";
    for (int i = 0; i < (int)player.skills.size(); i++) {
        if (player.skills[i].damage < 0){
            std::cout << "  " << (i + 1) << ". " << player.skills[i].name
                  << " (Heal: " << -(player.skills[i].damage)
                  << ", MP: " << player.skills[i].mpCost << ")";
        }
        else{
            std::cout << "  " << (i + 1) << ". " << player.skills[i].name
                  << " (DMG: " << player.skills[i].damage
                  << ", MP: " << player.skills[i].mpCost << ")";
        }
        
        if (player.skills[i].currentCd > 0){
            std::cout << " [CD: " << player.skills[i].currentCd << " Turn]";
        }
        std::cout << "\n";
    }
    std::cout << "  0. Kembali\n> ";

    int choice;
    std::cin >> choice;

    if (choice == 0) return false; // Kembali ke menu utama

    if (choice < 1 || choice > (int)player.skills.size()) {
        std::cout << "Pilihan tidak valid.\n";
        return false;
    }

    Skill& sk = player.skills[choice - 1];
    if (sk.currentCd > 0){
        std::cout << "Skill ini masih cooldown! (" << sk.currentCd << " turn tersisa)\n";
        return false;
    }
    if (player.mp < sk.mpCost) {
        std::cout << "MP tidak cukup untuk menggunakan " << sk.name << "!\n";
        return false;
    }
    // Kalkulasi dmg dan mana cost
    int dmg;
    player.mp -= sk.mpCost;
    if (sk.damage < 0) {
        dmg = sk.damage;
        player.hp -= dmg;
        player.hp = clamp(player.hp, 0, player.maxHp);
        std::cout << player.name << " menggunakan skill " << sk.name << "!\n[Heal]: " << -dmg << "\n";
    }
    else{
        // matematika dmg
        dmg = std::max((float)1.0, sk.damage - (enemy.isShielding ? enemy.defense * 2 : enemy.defense));
        enemy.hp -= dmg;
        enemy.hp = clamp(enemy.hp, 0, enemy.maxHp);
        std::cout << player.name << " menggunakan skill " << sk.name << "!\n[Damage]: " << dmg << "\n";
    }
    if (!(sk.inflict.name.empty())) {
        enemy.statusEffects.push_back({sk.inflict.name, sk.inflict.duration});
    }
    sk.currentCd = sk.maxCooldown;
    player.isAttacking = true;
    return true;
}

// Menggunakan Item dari Inventori
bool actionItem(Entity& player) {
    bool hasItem = false;
    for (auto& item : player.inventory)
        if (item.quantity > 0) { hasItem = true; break; }

    if (!hasItem) {
        std::cout << "Inventori kosong.\n";
        return false;
    }

    std::cout << "Pilih Item:\n";
    for (int i = 0; i < (int)player.inventory.size(); i++) {
        if (player.inventory[i].quantity > 0)
            std::cout << "  " << (i + 1) << ". " << player.inventory[i].name
                      << " (Heal: " << player.inventory[i].healAmount
                      << ", Qty: " << player.inventory[i].quantity << ")\n";
    }
    std::cout << "  0. Kembali\n> ";

    int choice;
    std::cin >> choice;

    if (choice == 0) return false;

    if (choice < 1 || choice > (int)player.inventory.size() ||
        player.inventory[choice - 1].quantity <= 0) {
        std::cout << "Pilihan tidak valid atau item habis.\n";
        return false;
    }

    Item& itm = player.inventory[choice - 1];
    int heal = itm.healAmount;
    player.hp = clamp(player.hp + heal, 0, player.maxHp);
    itm.quantity--;
    std::cout << player.name << " menggunakan " << itm.name << "!\n[HP pulih] + " << heal << "\n";
    return true;
}

// Flee (Kabur)
// Mengembalikan true jika berhasil kabur
bool actionFlee(Entity& main_e, Entity& second_e) {
    // Hitung chance kabur: base 40% + selisih spd (sementara)
    int chance = 40 + (main_e.spd - second_e.spd) * 2; //chance kabur (sementara)
    chance = clamp(chance, 10, 90);
    int roll = randomNumber(1, 100);
    std::cout << "Mencoba kabur... (Chance: " << chance << "%, Roll: " << roll << ")\n";
    if (roll < chance) {
        std::cout << main_e.name << " berhasil kabur!\n";
        return true;
    }
    std::cout << main_e.name << " gagal kabur!\n";
    return false;
}

// ============================================================
//  GILIRAN PLAYER
// ============================================================

// Mengembalikan: 1 = aksi selesai, 2 = player kabur, 3 = skip turn
int playerTurn(Entity& player, Entity& enemy) {
    // Cek Status Effect Player
    bool stunned = applyStatusEffects(player);
    if (isDead(player)) {return 4;} // bisa mati dari DOT

    if (stunned) {return 3;} // Skip Turn karena stun

    // Loop input sampai aksi valid
    while (true) {
        std::cout << "================ STATS =================\n";
        printStatus(player);
        printStatus(enemy);
        std::cout << "================ STATS =================\n";
        std::cout << "Giliran " << player.name << "! Pilih aksi:\n"
                  << "  [1] Menyerang\n"
                  << "  [2] Bertahan\n"
                  << "  [3] Menggunakan Skill\n"
                  << "  [4] Menggunakan Item\n"
                  << "  [5] Flee (Kabur)\n"
                  << "  [0] Skip Turn\n"
                  << "> ";

        int input;
        if (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input = -1;
        }

        switch (input) {
            case 1:
                actionAttack(player, enemy);
                return 1;

            case 2:
                actionDefend(player);
                return 1;

            case 3: {
                bool used = playerSkill(player, enemy);
                if (used) {return 1;}
                // Kembali → ulangi loop
                break;
            }

            case 4: {
                bool used = actionItem(player);
                if (used) {return 1;}
                // Kembali → ulangi loop
                break;
            }

            case 5: {
                bool fled = actionFlee(player, enemy);
                if (fled) {return 2;}
                // Gagal kabur 
                return 1;
            }

            case 0:
                std::cout << player.name << " melewatkan giliran.\n";
                return 3;

            default:
                std::cout << "Input tidak valid! Silakan coba lagi.\n";
                break;
        }
    }
}

// ============================================================
//  GILIRAN LAWAN (AI - Random untuk sementara)
// ============================================================

//pilihan AI
int decision(const Entity &enemy, const Entity &player){
    int random_num = randomNumber(1, 20);
    if (player.isAttacking){
        if (enemy.hp <= 0.1 * enemy.maxHp && random_num <= 8) {return 4;}

        if (random_num <= 4 || player.hp <= enemy.hp || (player.hp <= 0.3 * player.maxHp && enemy.hp >= 0.4 * enemy.maxHp)) {return 1;}

        if (enemy.isShielding || enemy.hp < 0.25 * enemy.maxHp) {return 3;}

        if (breachExist(enemy)){return 1;}

        return 2;
    }

    if (player.isShielding){
        if (enemy.hp <= 0.2 * enemy.maxHp && random_num <= 8) {return 4;}

        if (!enemy.isShielding && breachExist(enemy)) {return 2;;}

        if (enemy.skills.empty()) {return 1;}

        return 3;
    }

    if (enemy.hp <= 0.2 * enemy.maxHp && random_num <= 10) {return 4;}

    return 1;    
}

// Khusus Buat Boss
void bossSkillOption(Entity& enemie, Entity& player, int pilihan){
    findNilai(enemie);
    Skill& sk = enemie.skills[enemie.skill_option.avaliable_skill[0]]; // place holder & untuk deklarasi variabel
    int dmg;
    // Pilihan boss ketika mencari skill dengan dmg terbesar
    if (pilihan == 5){
        if (enemie.skill_option.maxSkilDamage_index == -1){
            actionAttack(enemie, player);
            return;
        }

        sk = enemie.skills[enemie.skill_option.maxSkilDamage_index];
    }

    // Pilihan boss ketika mencari skill heal
    else if (pilihan == 6){
        if (!enemie.skill_option.healSkil_index.empty()){
            actionAttack(enemie, player);
            return;
        }

        int heal_rng = enemie.skill_option.healSkil_index[randomNumber(0, enemie.skill_option.healSkil_index.size() - 1)];
        sk = enemie.skills[enemie.skill_option.healSkil_index[heal_rng]];
        if (enemie.mp < sk.mpCost) {
            actionAttack(enemie, player);
            return ;
        }
        enemie.mp -= sk.mpCost;
        dmg = sk.damage;
        enemie.hp -= dmg;
        std::cout << enemie.name << " menggunakan skill " << sk.name << "!\n[Heal]: " << -dmg << "\n";
        sk.currentCd = sk.maxCooldown;
        return;
    }

    // Pilihan boss ketika mencari skill yang memberikan status efek kepada player
    else if (pilihan == 7){
        if (!enemie.skill_option.statusSkil_index.empty()){
            actionAttack(enemie, player);
            return;
        }

        // mencari status efek priority
        int priority = statusPriority(enemie, player);
        if (priority == -1){
            int status_rng = enemie.skill_option.statusSkil_index[randomNumber(0, enemie.skill_option.statusSkil_index.size() - 1)];
            sk = enemie.skills[status_rng];
        }
        sk = enemie.skills[priority];
    }

    if (enemie.mp < sk.mpCost) {
            actionAttack(enemie, player);
            return ;
    }
    // Kalkulasi dmg dan mana cost
    dmg = std::max((float)1.0, sk.damage - (player.isShielding ? player.defense * 2 : player.defense)); // matematika dmg
    enemie.mp -= sk.mpCost;
    player.hp -= dmg;
    player.hp = clamp(player.hp, 0, player.maxHp);
    std::cout << enemie.name << " menggunakan skill " << sk.name << "!\n[Damage]: " << dmg << "\n";

    if (!(sk.inflict.name.empty())) {
        player.statusEffects.push_back({sk.inflict.name, sk.inflict.duration});
    }
    sk.currentCd = sk.maxCooldown;
}

// decision khusus untuk boss
int bossDecision(const Entity &enemy, const Entity &player){
    int random_num = randomNumber(1, 20);
    if (player.isAttacking){
        if (!(player.statusEffects.empty())) {return 5;}

        if (player.hp <= enemy.hp) {return 1;}

        if (enemy.hp < 0.25 * enemy.maxHp) {return 6;}

        if (!enemy.isShielding && (!breachExist(enemy) || (enemy.hp <= 0.8 * enemy.maxHp && enemy.hp >= 0.6 * enemy.maxHp))) {return 2;}

        return 3;
    }

    if (player.isShielding){
        if (((enemy.hp == enemy.maxHp && !(enemy.isShielding)) || (enemy.hp <= 0.5 * enemy.maxHp && !(enemy.isShielding))) && !(breachExist(enemy))) {return 2;}

        return 7;
    }
    
    if (!(player.statusEffects.empty())) {return 5;} 

    return 1;
}
// self eksplenatory
int enemyTurn(Entity& enemy, Entity& player) {
    // Cek Status Effect Lawan
    bool stunned = applyStatusEffects(enemy);
    findNilai(enemy);
    if (isDead(enemy)) return 4;
    if (stunned) { std::cout << enemy.name << " giliran dilewati karena stun.\n"; return 1; }

    std::cout << "\n--- Giliran " << enemy.name << " ---\n";

    int random_num = randomNumber(1, 10);
    int ai_decision = decision(enemy, player);
    if (enemy.isBoss){
        ai_decision = bossDecision(enemy, player);
    }
    else {
        ai_decision = decision(enemy, player);
    }
    // 1. 50% basic attack, 50% skill random
    // 2. Shielding
    // 3. skill random
    // 4. kabur
    // 5. max damage skill
    // 6. heal skill
    // 7. status effect skill
    switch (ai_decision){
        case 1: {
            if (!(enemy.skills.empty()) && random_num > 5){
                actionSkill(enemy, player);
                break;
            }
            actionAttack(enemy, player);
            break;
        }
        
        case 2:{
            if (!breachExist(enemy)){
                enemy.isShielding = true;
                std::cout << enemy.name << " bertahan!\n";
                break; 
            }
            break;
        }

        case 3: {
            if (!(enemy.skills.empty())){
                actionSkill(enemy, player);
                break;
            }
            actionAttack(enemy, player);
            break;
        }

        case 4: {
            if (actionFlee(enemy, player)){
                return 2;
            }
            break;
        }

        case 5: {
            bossSkillOption(enemy, player, 5);
            break;
        }

        case 6: {
            bossSkillOption(enemy, player, 6);
            break;
        }

        case 7: {
            bossSkillOption(enemy, player, 7);
            break;
        }
        default:
            break;
    }
    std::cout << "Output Aksi: ";
    printStatus(player);
    printStatus(enemy);
    return 1;
}

// ============================================================
//  BATTLE STATE MACHINE  (Masuk ke State TurnBase Battle)
// ============================================================

void startBattle(Entity& player, Entity& enemy) {
    std::cout << "\n*** MEMASUKI STATE TURNBASE BATTLE ***\n";
    std::cout << player.name << " vs " << enemy.name << "\n";
    std::cout << "============== ENCOUNTER ==============\n\n";

    bool battleOver = false;

    while (!battleOver) {
        
        if (player.spd >= enemy.spd){
            // ---- TURN PLAYER ----
            updateCooldown(player);
            int result = playerTurn(player, enemy);
            std::cout << "\n[Output Aksi Player]\n";
            printStatus(enemy);
            printStatus(player);
            std::cout << "\n";

            // Turn Player berakhir
            std::cout << "\n--- Turn Player Berakhir ---\n";
            if (result == 4) {exit(enemy, player);}
            if (result == 2) {
                // Player kabur → keluar dari battle
                std::cout << "[Keluar dari State TurnBase Battle (Player Kabur)]\n";
                return;
            }

            // Cek musuh mati?
            if (isDead(enemy)) {
                std::cout << "\n" << enemy.name << " kalah!\n";
                std::cout << player.name << " MENANG!\n";
                printSeparator();

                // Function Buat Reward
                std::cout << "[Keluar dari State TurnBase Battle]\n";
                return;
            }

            // ---- TURN LAWAN ----
            updateCooldown(enemy);
            int hasil_lawan = enemyTurn(enemy, player);

            // Turn Lawan berakhir
            std::cout << "--- Turn Lawan Berakhir ---\n";

            if (hasil_lawan == 2){
                std::cout << "[Keluar dari state Turnbase (Musuh Kabur)]\n";
                return;
            }
            // Cek player mati?
            if (isDead(player)) {
                std::cout << "\n" << player.name << " kalah!\n";
                printSeparator();
                std::cout << "[Keluar dari State TurnBase Battle]\n";
                return;
            }

            // Kembali ke awal loop
            player.isAttacking = false;
            std::cout << "\n[Loop ke giliran berikutnya...]\n";
        } 

        else {
            // ---- TURN LAWAN ----
            updateCooldown(enemy);
            int hasil_lawan = enemyTurn(enemy, player);

            // Turn Lawan berakhir
            std::cout << "--- Turn Lawan Berakhir ---\n";

            if (hasil_lawan == 2){
                std::cout << "[Keluar dari state Turnbase (Musuh Kabur)]\n";
                return;
            }
            // Cek player mati?
            if (isDead(player)) {
                std::cout << "\n" << player.name << " kalah!\n";
                printSeparator();
                std::cout << "[Keluar dari State TurnBase Battle]\n";
                return;
            }
            // ---- TURN PLAYER ----
            updateCooldown(player);
            int result = playerTurn(player, enemy);
            std::cout << "\n[Output Aksi Player]\n";
            printStatus(enemy);
            printStatus(player);
            std::cout << "\n";

            // Turn Player berakhir
            std::cout << "\n--- Turn Player Berakhir ---\n";
            if (result == 4) {exit(enemy, player);}
            if (result == 2) {
                // Player kabur → keluar dari battle
                std::cout << "[Keluar dari State TurnBase Battle (Player Kabur)]\n";
                return;
            }

            // Cek musuh mati?
            if (isDead(enemy)) {
                std::cout << "\n" << enemy.name << " kalah!\n";
                std::cout << player.name << " MENANG!\n";
                printSeparator();

                // Function Buat Reward
                std::cout << "[Keluar dari State TurnBase Battle]\n";
                return;
            }
            // Kembali ke awal loop
            player.isAttacking = false;
            std::cout << "\n[Loop ke giliran berikutnya...]\n";
        }
    }
}
