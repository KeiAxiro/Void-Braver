#include "states.h"
#include "database.h"
#include "state_helpers.h"
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
    cout << "6. Save\n";
    cout << "7. Kembali ke Main Menu\n";
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
        if (saveGame(ctx))
            cout << "Game berhasil disimpan.\n";
        waitForEnter();
        return;
    }
    if (choice == "7")
    {
        saveGame(ctx);
        ctx.stateStack.clear();
        ctx.stateStack.push_back(GameState::MainMenu);
        return;
    }

    cout << "Pilihan tidak valid.\n";
    waitForEnter();
}
