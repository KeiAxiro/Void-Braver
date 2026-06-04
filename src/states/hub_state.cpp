#include "states.h"
#include "database.h"
#include "states/character_features.h"
#include "states/crafting_features.h"
#include "states/dungeon_features.h"
#include "states/input_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/shop_features.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"

using namespace std;
using namespace consoleui;
using namespace state_helpers;

// Hub adalah pusat navigasi pemain setelah character aktif masuk game.
void runHub(GameContext &ctx)
{
    clearScreen();
    refreshPlayerResources(ctx);
    printHubHeader(ctx);
    printTitleBox("MAIN HUB");

    cout << colorText("1. Character Menu", Color::Green) << '\n';
    cout << colorText("2. Inventory", Color::Cyan) << '\n';
    cout << colorText("3. Enter Dungeon", Color::Yellow) << '\n';
    cout << colorText("4. Crafting", Color::Blue) << '\n';
    cout << colorText("5. Shop", Color::Magenta) << '\n';
    cout << colorText("6. Quest Board", Color::Yellow) << '\n';
    cout << "7. Save\n";
    cout << "8. Kembali ke Main Menu\n";
    cout << "Choose: ";

    string choice = readLine();
    if (choice == "1")
    {
        characterMenu(ctx);
        return;
    }
    if (choice == "2")
    {
        inventoryMenu(ctx);
        return;
    }
    if (choice == "3")
    {
        chooseDungeon(ctx);
        return;
    }
    if (choice == "4")
    {
        craftingMenu(ctx);
        return;
    }
    if (choice == "5")
    {
        shopMenu(ctx);
        return;
    }
    if (choice == "6")
    {
        questMenu(ctx);
        return;
    }
    if (choice == "7")
    {
        if (saveGame(ctx))
            cout << "Game berhasil disimpan.\n";
        waitForEnter();
        return;
    }
    if (choice == "8")
    {
        saveGame(ctx);
        ctx.stateStack.clear();
        ctx.stateStack.push_back(GameState::MainMenu);
        return;
    }

    cout << "Pilihan tidak valid.\n";
    waitForEnter();
}
