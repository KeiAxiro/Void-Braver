#include "states.h"
#include "database.h"
#include "state_helpers.h"
#include "utils/console_ui.h"

using namespace std;
using namespace consoleui;
using namespace state_helpers;

// Main menu dibuat sesederhana mungkin supaya alurnya mudah diikuti:
// tampilkan menu, baca input, lalu pindahkan state jika valid.
void runMainMenu(GameContext &ctx)
{
    clearScreen();
    printLine('=');
    cout << colorText("              VOID BRAVER               ", Color::Cyan, true) << '\n';
    printLine('=');
    cout << "1. New Game\n";
    cout << "2. Load Game\n";
    cout << "3. Exit\n";
    printLine('=');
    cout << "Choose: ";

    string choice = readLine();

    if (choice == "1")
    {
        loadSave(ctx);

        cout << "Masukkan nama character: ";
        string name = readLine();
        if (name.empty())
            name = "Hero";

        cout << MENU_SEPARATOR << '\n';
        cout << "Pilih class:\n";
        for (size_t i = 0; i < ctx.gameData["classes"].size(); ++i)
        {
            const auto &job = ctx.gameData["classes"][i];
            cout << (i + 1) << ". " << job.value("name", string())
                 << " | Primary Stat: " << job.value("primary_stat", string("STR")) << '\n';
        }
        cout << "Nomor class: ";

        string classChoice = readLine();
        int classIndex = 0;
        if (!tryParseInt(classChoice, classIndex) || classIndex < 1 || classIndex > static_cast<int>(ctx.gameData["classes"].size()))
        {
            cout << "Class tidak valid. Default ke Knight.\n";
            classIndex = 1;
        }

        string classId = ctx.gameData["classes"][static_cast<size_t>(classIndex - 1)].value("id", string("knight"));
        createNewGame(ctx, name, classId);
        refreshPlayerResources(ctx);

        string newCharacterId = ctx.player.id;
        if (saveGame(ctx))
            loadSave(ctx);

        for (int i = 0; i < static_cast<int>(ctx.characters.size()); ++i)
        {
            if (ctx.characters[static_cast<size_t>(i)].id == newCharacterId)
            {
                loadCharacter(ctx, i);
                break;
            }
        }

        ctx.stateStack.clear();
        ctx.stateStack.push_back(GameState::Hub);
        return;
    }

    if (choice == "2")
    {
        if (!loadSave(ctx) || ctx.characters.empty())
        {
            cout << "Belum ada character yang tersimpan.\n";
            waitForEnter();
            return;
        }

        while (true)
        {
            clearScreen();
            cout << MENU_SEPARATOR << '\n';
            cout << colorText("LOAD GAME", Color::Yellow, true) << '\n';
            cout << MENU_SEPARATOR << '\n';

            for (size_t i = 0; i < ctx.characters.size(); ++i)
            {
                const auto &character = ctx.characters[i];
                cout << (i + 1) << ". " << character.name
                     << " | Class: " << character.class_id
                     << " | Level: " << character.level
                     << " | Gold: " << character.gold << '\n';
            }

            cout << "Pilih nomor character atau c untuk kembali: ";
            string input = toLower(readLine());
            if (input == "c")
                return;

            int selected = 0;
            if (!tryParseInt(input, selected) || selected < 1 || selected > static_cast<int>(ctx.characters.size()))
            {
                cout << "Pilihan character tidak valid.\n";
                continue;
            }

            loadCharacter(ctx, selected - 1);
            ctx.stateStack.clear();
            ctx.stateStack.push_back(GameState::Hub);
            return;
        }
    }

    if (choice == "3")
    {
        ctx.running = false;
        return;
    }

    cout << "Pilihan tidak valid.\n";
    waitForEnter();
}
