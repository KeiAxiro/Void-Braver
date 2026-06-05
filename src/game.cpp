#include "game.h"

#include "database.h"
#include "states.h"

#include <iostream>

namespace
{
    void dispatchCurrentState(GameContext &ctx)
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
}

int startGame()
{
    GameContext ctx;

    if (!loadGameData(ctx))
    {
        std::cerr << "Game data missing. Put data/game_data.json in place.\n";
        return 1;
    }

    runGame(ctx);

    saveGame(ctx);
    std::cout << "Bye.\n";
    return 0;
}

void runGame(GameContext &ctx)
{
    while (ctx.running && !ctx.stateStack.empty())
        dispatchCurrentState(ctx);
}
