#include "../include/globals.h"
#include "../include/database.h"
#include "../include/states.h"

#include <iostream>

using namespace std;

int main()
{
    // GameContext menyimpan data game, save aktif, dan state stack.
    GameContext ctx;

    if (!loadGameData(ctx))
    {
        cerr << "Game data missing. Put data/game_data.json in place.\n";
        return 1;
    }

    // Selama game masih berjalan, state paling atas pada stack yang akan diproses.
    while (ctx.running && !ctx.stateStack.empty())
    {
        switch (ctx.stateStack.back())
        {
        case GameState::MainMenu:
            runMainMenu(ctx);
            break;
        case GameState::Hub:
            runHub(ctx);
            break;
        case GameState::Battle:
            runBattle(ctx);
            break;
        case GameState::Quit:
            ctx.running = false;
            break;
        }
    }

    saveGame(ctx);
    cout << "Bye.\n";
    return 0;
}
