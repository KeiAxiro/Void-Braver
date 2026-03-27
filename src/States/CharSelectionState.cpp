#include "States/CharSelectionState.h"
#include "States/HubState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <string>

namespace States {

    CharSelectionState::CharSelectionState() : selectedClass(-1) {}

    void CharSelectionState::Init() {}

    void CharSelectionState::HandleInput(Core::StateManager& stateManager) {
        std::string input;
        std::cout << "\nEnter your choice: ";
        std::cin >> input;

        if (input == "1") selectedClass = 1;
        else if (input == "2") selectedClass = 2;
        else if (input == "3") selectedClass = 3;
        else selectedClass = -1;
    }

void CharSelectionState::Update(Core::StateManager& stateManager) {
        if (selectedClass != -1) {
            auto& p = stateManager.GetContext().player;
            p.level = 1;
            p.exp = 0;
            p.statPoints = 5;

            if (selectedClass == 1) {
                p.name = "Thorne"; p.job = "Knight";
                p.str = 10; p.intel = 5; p.agi = 10; p.vit = 40;
                p.weapon = Entities::getEquipmentData("Rusty Greatsword");
                p.armor = Entities::getEquipmentData("Knight's Plate");
            } else if (selectedClass == 2) {
                p.name = "Lara"; p.job = "Sorcerer";
                p.str = 5; p.intel = 25; p.agi = 15; p.vit = 20;
                p.weapon = Entities::getEquipmentData("Kosong"); 
                p.armor = Entities::getEquipmentData("Leather Jerkin");
            } else if (selectedClass == 3) {
                p.name = "Vane"; p.job = "Wanderer";
                p.str = 10; p.intel = 10; p.agi = 25; p.vit = 20;
                p.weapon = Entities::getEquipmentData("Rusty Greatsword");
                p.armor = Entities::getEquipmentData("Leather Jerkin");
            }
            
            stateManager.GetContext().playerClassId = selectedClass;

            std::cout << "\nYou have chosen " << p.name << "!\n";
            std::cout << "Data saved to Game Context!\n";
            Utils::ConsoleUI::Pause();

            stateManager.ChangeState(std::make_unique<HubState>(stateManager.GetContext()));
        }
    }

    void CharSelectionState::Render() {
        Utils::ConsoleUI::ClearScreen();

        std::cout << "========================================\n";
        std::cout << "           CHOOSE YOUR CLASS            \n";
        std::cout << "========================================\n";
        std::cout << "1. Thorne - Knight Type\n";
        std::cout << "2. Lara - Sorcerer Type\n";
        std::cout << "3. Vane - Wanderer Type\n";
        std::cout << "========================================\n";
    }
}