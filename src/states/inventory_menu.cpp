#include "states/character_features.h"

#include "database.h"
#include "states/input_helpers.h"
#include "states/inventory_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    bool equipInventoryEntry(GameContext &ctx, size_t index)
    {
        if (index >= ctx.player.inventory.size())
            return false;

        InventoryEntry selected = ctx.player.inventory[index];
        const auto *item = getItemById(ctx, selected.item_id);
        if (!item)
            return false;

        const string category = item->value("category", string());
        if (!isEquipmentCategory(category))
        {
            cout << "Item ini bukan equipment.\n";
            return false;
        }

        const string requiredClass = item->value("required_class_id", string());
        if (!requiredClass.empty() && requiredClass != ctx.player.class_id)
        {
            cout << "Class kamu tidak cocok untuk item ini.\n";
            return false;
        }

        const string slot = slotFromCategory(category);
        for (auto &entry : ctx.player.inventory)
        {
            if (entry.equipped && entry.slot == slot)
            {
                entry.equipped = false;
                entry.slot.clear();
            }
        }

        if (ctx.player.inventory[index].quantity > 1)
        {
            ctx.player.inventory[index].quantity -= 1;
            addItem(ctx.player, selected.item_id, 1, true, slot);
        }
        else
        {
            ctx.player.inventory[index].equipped = true;
            ctx.player.inventory[index].slot = slot;
        }

        mergeInventory(ctx.player);
        refreshPlayerResources(ctx);
        cout << item->value("name", selected.item_id) << " berhasil di-equip pada slot " << slot << ".\n";
        return true;
    }

    bool unequipInventoryEntry(GameContext &ctx, size_t index)
    {
        if (index >= ctx.player.inventory.size())
            return false;
        if (!ctx.player.inventory[index].equipped)
            return false;

        if (ctx.player.inventory[index].quantity > 1)
        {
            ctx.player.inventory[index].quantity -= 1;
            addItem(ctx.player, ctx.player.inventory[index].item_id, 1, false, "");
        }
        else
        {
            ctx.player.inventory[index].equipped = false;
            ctx.player.inventory[index].slot.clear();
        }

        mergeInventory(ctx.player);
        refreshPlayerResources(ctx);
        cout << "Equipment berhasil dilepas.\n";
        return true;
    }

    void useConsumable(GameContext &ctx, size_t index)
    {
        if (index >= ctx.player.inventory.size())
            return;

        const auto *item = getItemById(ctx, ctx.player.inventory[index].item_id);
        if (!item)
            return;
        if (item->value("category", string()) != "consumable")
        {
            cout << "Item ini bukan consumable.\n";
            return;
        }

        const string nameLower = toLower(item->value("name", string()));
        int healHp = 0;
        int healMp = 0;
        if (nameLower.find("mana") != string::npos)
            healMp = max(20, ctx.player.max_mp / 4);
        else
            healHp = max(30, ctx.player.max_hp / 4);

        ctx.player.hp = min(ctx.player.max_hp, ctx.player.hp + healHp);
        ctx.player.mp = min(ctx.player.max_mp, ctx.player.mp + healMp);
        removeItem(ctx.player, ctx.player.inventory[index].item_id, 1);
        cout << item->value("name", string()) << " digunakan. ";
        if (healHp > 0)
            cout << "HP +" << healHp;
        if (healMp > 0)
            cout << "MP +" << healMp;
        cout << '\n';
    }

    void inventoryMenu(GameContext &ctx)
    {
        int page = 1;
        while (true)
        {
            clearScreen();
            mergeInventory(ctx.player);
            refreshPlayerResources(ctx);

            const int totalItems = static_cast<int>(ctx.player.inventory.size());
            const int totalPages = max(1, (totalItems + PAGE_SIZE - 1) / PAGE_SIZE);
            page = clampInt(page, 1, totalPages);

            const int start = (page - 1) * PAGE_SIZE;
            const int end = min(totalItems, start + PAGE_SIZE);

            printStateHeader(ctx, "INVENTORY");
            cout << colorText("Page", Color::Cyan, true) << ": " << page << "/" << totalPages << '\n';
            cout << colorText("Gold", Color::Yellow, true) << ": " << ctx.player.gold << "\n\n";
            if (totalItems == 0)
                cout << "(Kosong)\n";

            for (int i = start; i < end; ++i)
            {
                const auto &entry = ctx.player.inventory[static_cast<size_t>(i)];
                const auto *item = getItemById(ctx, entry.item_id);
                const string name = item ? item->value("name", entry.item_id) : entry.item_id;
                const string category = item ? item->value("category", string()) : "-";

                cout << (i - start + 1) << ". " << name << " x" << entry.quantity
                     << " [" << category << "]";
                if (entry.equipped)
                    cout << " [Equipped:" << entry.slot << "]";
                cout << '\n';
            }

            cout << "Input nomor item, q prev, e next, c kembali: ";
            const string input = toLower(readLine());
            if (input == "c")
                return;
            if (input == "q")
            {
                page = max(1, page - 1);
                continue;
            }
            if (input == "e")
            {
                page = min(totalPages, page + 1);
                continue;
            }

            int number = 0;
            if (!tryParseInt(input, number))
            {
                cout << "Input tidak valid.\n";
                waitForEnter();
                continue;
            }

            if (number >= 1 && number <= (end - start))
            {
                const size_t selectedIndex = static_cast<size_t>(start + number - 1);
                const auto *item = getItemById(ctx, ctx.player.inventory[selectedIndex].item_id);
                if (!item)
                {
                    cout << "Data item tidak ditemukan.\n";
                    waitForEnter();
                    continue;
                }

                clearScreen();
                printStateHeader(ctx, item->value("name", string()));

                int equippedCount = 0;
                for (const auto &entry : ctx.player.inventory)
                {
                    if (entry.item_id == ctx.player.inventory[selectedIndex].item_id && entry.equipped)
                        equippedCount += entry.quantity;
                }

                showItemDetails(ctx, *item, inventoryCount(ctx.player, ctx.player.inventory[selectedIndex].item_id), equippedCount);

                cout << "1. Equip/Unequip\n";
                cout << "2. Use Consumable\n";
                cout << "3. Back\n";
                cout << "Choose: ";
                const string action = readLine();

                if (action == "1")
                {
                    if (ctx.player.inventory[selectedIndex].equipped)
                        unequipInventoryEntry(ctx, selectedIndex);
                    else
                        equipInventoryEntry(ctx, selectedIndex);
                    waitForEnter();
                }
                else if (action == "2")
                {
                    useConsumable(ctx, selectedIndex);
                    waitForEnter();
                }

                continue;
            }

            if (number >= 1 && number <= totalPages)
            {
                page = number;
                continue;
            }

            cout << "Nomor tidak valid.\n";
            waitForEnter();
        }
    }
} // namespace state_helpers
