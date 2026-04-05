#include "states.h"
#include "database.h"
#include "states/battle_features.h"
#include "states/dungeon_features.h"
#include "states/input_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"
#include "config/game_rules.h"

#include <algorithm>
#include <vector>

using namespace std;
using namespace consoleui;
using namespace state_helpers;

// Flow dungeon:
// 1. pilih aksi di depth aktif
// 2. jika move forward, pemain menempuh beberapa langkah tetap
// 3. di dalam langkah itu bisa muncul encounter acak
// 4. jika seluruh langkah selesai, depth dianggap clear
void runBattle(GameContext &ctx)
{
    auto leaveBattleState = [&]() {
        if (!ctx.stateStack.empty() && ctx.stateStack.back() == GameState::Battle)
            ctx.stateStack.pop_back();
    };

    while (ctx.running && !ctx.stateStack.empty() && ctx.stateStack.back() == GameState::Battle)
    {
        const json *dungeon = getDungeonById(ctx, ctx.player.progress.current_dungeon);
        if (dungeon == nullptr)
        {
            cout << "Dungeon tidak ditemukan. Kembali ke hub.\n";
            waitForEnter();
            leaveBattleState();
            return;
        }

        const json *depthData = findDungeonDepth(*dungeon, ctx.player.progress.current_depth);
        if (depthData == nullptr)
        {
            cout << "Depth tidak valid. Kembali ke hub.\n";
            waitForEnter();
            leaveBattleState();
            return;
        }

        clearScreen();
        refreshPlayerResources(ctx);

        printStateHeader(ctx, "EKSPLORASI DUNGEON");
        cout << colorText(dungeon->value("name", string()), Color::Yellow, true) << '\n';
        cout << colorText("Depth Aktif", Color::Cyan, true) << " : " << ctx.player.progress.current_depth << '\n';
        cout << colorText("Level Range", Color::Cyan, true) << " : "
             << (*depthData)["level_range"].value("min", 1)
             << "-" << (*depthData)["level_range"].value("max", 999) << '\n';
        cout << colorText("ATK / DEF", Color::Cyan, true) << "   : "
             << calculatePlayerAttack(ctx) << " / " << calculatePlayerDefense(ctx) << '\n';
        cout << colorText("HP / MP", Color::Cyan, true) << "     : "
             << ctx.player.hp << "/" << ctx.player.max_hp
             << " | " << ctx.player.mp << "/" << ctx.player.max_mp << "\n\n";

        printDungeonLore(*depthData);
        cout << "\n";
        printLine('=');
        cout << colorText("1. Move Forward (" + to_string(game_rules::kMoveStepsPerDepth) + " langkah)", Color::Green) << '\n';
        cout << colorText("2. Ganti Dungeon / Depth", Color::Cyan) << '\n';
        cout << colorText("3. Kembali ke Hub", Color::Magenta) << '\n';
        printLine('=');
        cout << "Choose: ";

        const string explorationChoice = readLine();
        if (explorationChoice == "2")
        {
            selectDungeonDepth(ctx, false);
            continue;
        }
        if (explorationChoice == "3")
        {
            leaveBattleState();
            return;
        }
        if (explorationChoice != "1")
        {
            cout << "Pilihan tidak valid.\n";
            waitForEnter();
            continue;
        }

        const vector<int> encounterSteps = buildEncounterStepsForMove();
        bool journeyCancelled = false;

        for (int step = 1; step <= game_rules::kMoveStepsPerDepth; ++step)
        {
            clearScreen();
            printStateHeader(ctx, "MENYUSURI KORIDOR");
            cout << colorText(dungeon->value("name", string()), Color::Yellow, true)
                 << " | Depth " << ctx.player.progress.current_depth << "\n\n";
            cout << colorText("Langkah", Color::Cyan, true) << " : " << step << "/" << game_rules::kMoveStepsPerDepth << "\n";
            cout << colorText("Encounter dalam perjalanan ini", Color::Cyan, true)
                 << " : " << encounterSteps.size() << "\n";
            printLine('-');

            const bool encounterNow = find(encounterSteps.begin(), encounterSteps.end(), step) != encounterSteps.end();
            if (!encounterNow)
            {
                cout << colorText("Lorong masih sepi, tapi hawa dungeon terasa berat.", Color::Blue) << '\n';
                if (step < game_rules::kMoveStepsPerDepth)
                    waitForEnter();
                continue;
            }

            cout << colorText("Ada langkah kaki lain yang mendekat. Musuh langsung menyerang!", Color::Red, true) << "\n\n";

            EnemyInstance enemy = spawnEnemyForDepth(ctx, *dungeon, ctx.player.progress.current_depth);
            const bool enemyStartsFirst = enemyGetsFirstTurn(ctx, enemy, false);

            waitForEnter();

            const EncounterResult result = runEncounterBattle(ctx, *dungeon, *depthData, enemy, enemyStartsFirst);
            if (result == EncounterResult::Defeat)
            {
                leaveBattleState();
                return;
            }
            if (result == EncounterResult::Retreat)
            {
                journeyCancelled = true;
                break;
            }

            if (step < game_rules::kMoveStepsPerDepth)
            {
                clearScreen();
                printStateHeader(ctx, "LANJUT MENYUSURI DEPTH");
                cout << colorText("Kamu menang dan kembali melangkah ke depan.", Color::Green) << '\n';
                cout << "Sisa langkah: " << (game_rules::kMoveStepsPerDepth - step) << '\n';
                waitForEnter();
            }
        }

        if (journeyCancelled)
            continue;

        clearScreen();
        printStateHeader(ctx, "UJUNG DEPTH TERCAPAI");
        cout << colorText("Kamu berhasil menyelesaikan perjalanan depth ini.", Color::Green, true) << '\n';
        applyDepthCompletionRewards(ctx, *dungeon, *depthData);
        waitForEnter();
    }
}
