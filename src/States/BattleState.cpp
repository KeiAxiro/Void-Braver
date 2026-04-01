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

    BattleState::BattleState(Core::GameContext& ctx, int dungeonId, int depth)
        : context(ctx), currentDungeonId(dungeonId), dungeonDepth(depth), battlePhase(0), currentMenu(0), selectedOption(-1),
          playerShielding(false), enemyShielding(false) {}

    void BattleState::spawnEnemy() {
        auto validEnemies = Entities::getEnemiesByDungeonAndDepth(currentDungeonId, dungeonDepth);
        if (validEnemies.empty()) {
            validEnemies = Entities::getAllEnemyTemplates(); 
        }

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, validEnemies.size() - 1);
        
        Entities::EnemyTemplate selectedTmpl = validEnemies[dist(gen)];
        currentEnemy.loadFromTemplate(selectedTmpl, context.player.level);
        
        battleLog = "A wild " + currentEnemy.name + " (Lv." + to_string(currentEnemy.level) + ") appeared!";
    }

    void BattleState::Init() {
        spawnEnemy();
        
        if (context.player.level >= currentEnemy.level) {
            battlePhase = 1;
            battleLog += "\n" + context.player.name + " takes the initiative!";
        } else {
            battlePhase = 2;
            battleLog += "\nEnemy level is higher. " + currentEnemy.name + " strikes first!";
        }
    }

    void BattleState::HandleInput(Core::StateManager& stateManager) {
        if (battlePhase == 1) { 
            string input;
            if (currentMenu == 0) cout << "\n Choose action: ";
            else if (currentMenu == 1) cout << "\n Select Skill (0 to Cancel): ";
            else if (currentMenu == 2) cout << "\n Select Item (0 to Cancel): ";
            cin >> input;
            
            try { selectedOption = stoi(input); } 
            catch (...) { selectedOption = -1; }
        } else if (battlePhase == 3) {
            string input;
            cout << "\n Press 0 to return to Hub: ";
            cin >> input;
            if (input == "0") selectedOption = 0;
            else selectedOption = -1;
        } else if (battlePhase == 2) {
            Utils::ConsoleUI::Pause();
        }
    }

    void BattleState::processEnemyTurn() {
        enemyShielding = false;
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(1, 20);
        int rng = dist(gen);

        int aiDecision = 1; 

        if (currentEnemy.isBoss) {
            if (playerShielding) {
                if (currentEnemy.hp <= 0.5 * currentEnemy.maxHp) aiDecision = 2; 
                else aiDecision = 1;
            } else {
                if (context.player.hp <= currentEnemy.hp) aiDecision = 1;
                else if (currentEnemy.hp < 0.25 * currentEnemy.maxHp) aiDecision = 2;
                else aiDecision = (rng <= 10) ? 1 : 2; 
            }
        } else {
            if (playerShielding) {
                if (currentEnemy.hp <= 0.2 * currentEnemy.maxHp && rng <= 8) aiDecision = 4; 
                else aiDecision = 2; 
            } else {
                if (currentEnemy.hp <= 0.1 * currentEnemy.maxHp && rng <= 8) aiDecision = 4;
                else if (rng <= 4 || context.player.hp <= currentEnemy.hp) aiDecision = 1;
                else if (currentEnemy.hp < 0.25 * currentEnemy.maxHp) aiDecision = 2;
                else aiDecision = 1;
            }
        }

        if (aiDecision == 1) {
            int pDef = playerShielding ? context.player.getDefense() * 2 : context.player.getDefense();
            int dmg = max(1, currentEnemy.atk - pDef);
            context.player.hp -= dmg;
            if (context.player.hp < 0) context.player.hp = 0;
            battleLog = currentEnemy.name + " attacks! " + context.player.name + " takes " + to_string(dmg) + " damage.";
        } else if (aiDecision == 2) {
            enemyShielding = true;
            battleLog = currentEnemy.name + " takes a defensive stance (Shielding).";
        } else if (aiDecision == 4) {
            battleLog = currentEnemy.name + " flees from the battle!";
            battlePhase = 3;
            return;
        }

        if (context.player.hp <= 0) { 
            battleLog += "\n\n" + context.player.name + " has fallen in battle...";
            battlePhase = 3;
        } else {
            battlePhase = 1; 
        }
    }

    void BattleState::calculateRewards() {
        battleLog = currentEnemy.name + " defeated!\n";
        
        Entities::DungeonExp expData = Entities::getDungeonDepthExp(dungeonDepth);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distExp(expData.minExp, expData.maxExp);
        int gainedExp = distExp(gen);
        
        battleLog += "Gained " + to_string(gainedExp) + " EXP.\n";
        context.player.addExp(gainedExp);

        // Boss specific guaranteed drops
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

        // Random drops for normal enemies
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

    void BattleState::Update(Core::StateManager& stateManager) {
        if (battlePhase == 2) {
            processEnemyTurn();
        } else if (battlePhase == 1) { 
            if (currentMenu == 0) {
                if (selectedOption == 1) { 
                    playerShielding = false;
                    int eDef = enemyShielding ? currentEnemy.def * 2 : currentEnemy.def;
                    int dmg = max(1, context.player.getAtkPower() - eDef);
                    
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_real_distribution<> dist(0.0, 1.0);
                    if (dist(gen) <= context.player.getCritRate()) {
                        dmg = static_cast<int>(dmg * 1.5f);
                        battleLog = "CRITICAL HIT! ";
                    } else {
                        battleLog = "";
                    }

                    currentEnemy.takeDamage(dmg);
                    battleLog += context.player.name + " attacks for " + to_string(dmg) + " damage.";
                    
                    selectedOption = -1;

                    for (auto& sk : context.player.skills) {
                        if (sk.currentCd > 0) sk.currentCd--;
                    }
                    
                    if (currentEnemy.isDead()) {
                        battlePhase = 3;
                        calculateRewards();
                    } else {
                        battlePhase = 2; 
                    }
                } else if (selectedOption == 2) { 
                    playerShielding = true;
                    battleLog = context.player.name + " guards against the next attack.";
                    selectedOption = -1;

                    for (auto& sk : context.player.skills) {
                        if (sk.currentCd > 0) sk.currentCd--;
                    }

                    battlePhase = 2;
                } else if (selectedOption == 3) { 
                    currentMenu = 1; 
                    selectedOption = -1;
                } else if (selectedOption == 4) { 
                    currentMenu = 2; 
                    selectedOption = -1;
                } else if (selectedOption == 5) { 
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
            } else if (currentMenu == 1) { 
                if (selectedOption == 0) {
                    currentMenu = 0; selectedOption = -1;
                } else if (selectedOption > 0 && selectedOption <= static_cast<int>(context.player.skills.size())) {
                    auto& skill = context.player.skills[selectedOption - 1];
                    if (skill.isUnlocked && context.player.mp >= skill.manaCost && skill.currentCd == 0) {
                        context.player.mp -= skill.manaCost;
                        playerShielding = false;
                        
                        if (skill.damageMultiplier < 0) {
                            int healAmt = static_cast<int>(-skill.damageMultiplier * context.player.getAtkPower());
                            context.player.hp = min(context.player.hp + healAmt, context.player.getMaxHp());
                            battleLog = context.player.name + " uses " + skill.name + " and heals " + to_string(healAmt) + " HP.";
                        } else {
                            int baseDmg = static_cast<int>(skill.damageMultiplier * context.player.getAtkPower());
                            int eDef = enemyShielding ? currentEnemy.def * 2 : currentEnemy.def;
                            int dmg = max(1, baseDmg - eDef);
                            currentEnemy.takeDamage(dmg);
                            battleLog = context.player.name + " uses " + skill.name + " for " + to_string(dmg) + " damage.";
                        }

                        for (auto& sk : context.player.skills) {
                            if (sk.currentCd > 0) sk.currentCd--;
                        }
                        
                        skill.currentCd = skill.maxCooldown; 
                        
                        selectedOption = -1; currentMenu = 0;
                        
                        if (currentEnemy.isDead()) {
                            battlePhase = 3; calculateRewards();
                        } else {
                            battlePhase = 2; 
                        }
                    } else {
                        selectedOption = -1; 
                    }
                } else {
                    selectedOption = -1;
                }
            } else if (currentMenu == 2) { 
                if (selectedOption == 0) {
                    currentMenu = 0; selectedOption = -1;
                } else {
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
                        if (item.name == "Healing Potion") {
                            int heal = context.player.getMaxHp() * 0.3f;
                            context.player.hp = min(context.player.hp + heal, context.player.getMaxHp());
                            battleLog = "Used Healing Potion. Recovered " + to_string(heal) + " HP.";
                        } else if (item.name == "Mana Elixir") {
                            int healMp = context.player.getMaxMp() * 0.3f;
                            context.player.mp = min(context.player.mp + healMp, context.player.getMaxMp());
                            battleLog = "Used Mana Elixir. Recovered " + to_string(healMp) + " MP.";
                        }
                        
                        item.quantity--;
                        if (item.quantity <= 0) context.player.inventory.erase(context.player.inventory.begin() + targetIdx);
                        
                        selectedOption = -1; currentMenu = 0;

                        for (auto& sk : context.player.skills) {
                            if (sk.currentCd > 0) sk.currentCd--;
                        }

                        battlePhase = 2;
                    } else {
                        selectedOption = -1;
                    }
                }
            }
        } else if (battlePhase == 3) {
            if (selectedOption == 0) {
                stateManager.PopState(); 
            }
        }
    }

    void BattleState::Render() {
        Utils::ConsoleUI::ClearScreen();
        cout << string(60, '=') << "\n";
        cout << "                   D U N G E O N   D E P T H : " << dungeonDepth << "\n";
        cout << string(60, '=') << "\n\n";

        cout << "  [ENEMY] " << currentEnemy.name << " (Lv." << currentEnemy.level << ")" << (currentEnemy.isBoss ? " [BOSS]" : "") << "\n";
        cout << "  HP : " << currentEnemy.hp << " / " << currentEnemy.maxHp << "\n";
        if (enemyShielding) cout << "  (Status: Guarding)\n";
        cout << "\n";

        cout << "  [PLAYER] " << context.player.name << " (Lv." << context.player.level << ")\n";
        cout << "  HP : " << context.player.hp << " / " << context.player.getMaxHp() << "\n";
        cout << "  MP : " << context.player.mp << " / " << context.player.getMaxMp() << "\n";
        if (playerShielding) cout << "  (Status: Guarding)\n";
        cout << string(60, '-') << "\n";

        cout << "  > " << battleLog << "\n";
        cout << string(60, '-') << "\n";

        if (battlePhase == 1) {
            if (currentMenu == 0) {
                cout << "  [1] Attack    [2] Defend\n";
                cout << "  [3] Skill     [4] Item\n";
                cout << "  [5] Flee\n";
            } else if (currentMenu == 1) {
                cout << "  --- SKILLS ---\n";
                if (context.player.skills.empty()) {
                    cout << "  (No skills available)\n";
                } else {
                    for (size_t i = 0; i < context.player.skills.size(); ++i) {
                        const auto& sk = context.player.skills[i];
                        cout << "  [" << (i+1) << "] " << sk.name << " (MP: " << sk.manaCost << ")";
                        if (!sk.isUnlocked) cout << " [LOCKED]";
                        else if (sk.currentCd > 0) cout << " [CD: " << sk.currentCd << "]";
                        cout << "\n";
                    }
                }
                cout << "  [0] Back\n";
            } else if (currentMenu == 2) {
                cout << "  --- CONSUMABLES ---\n";
                int displayIdx = 1;
                bool hasItem = false;
                for (const auto& item : context.player.inventory) {
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
        } else if (battlePhase == 3) {
            cout << "\n  Battle Concluded. Press 0 to return.\n";
        }
        cout << string(60, '=') << "\n";
    }
}