#include "states/character_features.h"

#include "states/input_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"

#include <iostream>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void characterMenu(GameContext &ctx)
    {
        while (true)
        {
            clearScreen();
            showCharacterOverview(ctx);
            std::cout << colorText("1. Allocate Stats", Color::Green) << '\n';
            std::cout << colorText("2. Inventory & Equipment", Color::Cyan) << '\n';
            std::cout << colorText("3. Skill List", Color::Yellow) << '\n';
            std::cout << colorText("4. Back", Color::Magenta) << '\n';
            std::cout << "Choose: ";

            const std::string choice = readLine();
            if (choice == "1")
                allocateStats(ctx);
            else if (choice == "2")
                inventoryMenu(ctx);
            else if (choice == "3")
                skillMenu(ctx);
            else if (choice == "4")
                return;
            else
            {
                std::cout << "Pilihan tidak valid.\n";
                waitForEnter();
            }
        }
    }
} // namespace state_helpers
