#include "states.h"
#include "database.h"
#include "state_helpers.h"
#include "utils/console_ui.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void allocateStats(GameContext &ctx)
    {
        while (true)
        {
            clearScreen();
            refreshPlayerResources(ctx);
            printStateHeader(ctx, "ALLOCATE STAT POINTS");
            std::cout << colorText("Available Points", Color::Yellow, true) << ": " << ctx.player.stat_points << "\n\n";
            std::cout << colorText("1. STR", Color::Red) << " (" << ctx.player.stats.str << ")\n";
            std::cout << colorText("2. INT", Color::Cyan) << " (" << ctx.player.stats.intl << ")\n";
            std::cout << colorText("3. AGI", Color::Green) << " (" << ctx.player.stats.agi << ")\n";
            std::cout << colorText("4. VIT", Color::Yellow) << " (" << ctx.player.stats.vit << ")\n";
            std::cout << colorText("5. Back", Color::Magenta) << '\n';
            std::cout << "Choose stat: ";

            const std::string choice = readLine();
            if (choice == "5")
                return;

            if (ctx.player.stat_points <= 0)
            {
                std::cout << "Tidak ada stat point tersisa.\n";
                waitForEnter();
                return;
            }

            int statIndex = 0;
            if (!tryParseInt(choice, statIndex) || statIndex < 1 || statIndex > 4)
            {
                std::cout << "Pilihan tidak valid.\n";
                waitForEnter();
                continue;
            }

            std::cout << "Masukkan jumlah point yang ingin dialokasikan: ";
            const std::string amountInput = readLine();
            int amount = 0;
            if (!tryParseInt(amountInput, amount) || amount <= 0)
            {
                std::cout << "Jumlah tidak valid.\n";
                waitForEnter();
                continue;
            }

            amount = std::min(amount, ctx.player.stat_points);
            if (statIndex == 1)
                ctx.player.stats.str += amount;
            else if (statIndex == 2)
                ctx.player.stats.intl += amount;
            else if (statIndex == 3)
                ctx.player.stats.agi += amount;
            else if (statIndex == 4)
                ctx.player.stats.vit += amount;

            ctx.player.stat_points -= amount;
            refreshPlayerResources(ctx);
            std::cout << "Stat berhasil dialokasikan. HP/MP maksimum ikut diperbarui.\n";
            waitForEnter();
        }
    }

    bool equipInventoryEntry(GameContext &ctx, std::size_t index)
    {
        if (index >= ctx.player.inventory.size())
            return false;

        InventoryEntry selected = ctx.player.inventory[index];
        const auto *item = getItemById(ctx, selected.item_id);
        if (!item)
            return false;

        const std::string category = item->value("category", std::string());
        if (!isEquipmentCategory(category))
        {
            std::cout << "Item ini bukan equipment.\n";
            return false;
        }

        const std::string requiredClass = item->value("required_class_id", std::string());
        if (!requiredClass.empty() && requiredClass != ctx.player.class_id)
        {
            std::cout << "Class kamu tidak cocok untuk item ini.\n";
            return false;
        }

        const std::string slot = slotFromCategory(category);
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
        std::cout << item->value("name", selected.item_id) << " berhasil di-equip pada slot " << slot << ".\n";
        return true;
    }

    bool unequipInventoryEntry(GameContext &ctx, std::size_t index)
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
        std::cout << "Equipment berhasil dilepas.\n";
        return true;
    }

    void useConsumable(GameContext &ctx, std::size_t index)
    {
        if (index >= ctx.player.inventory.size())
            return;

        const auto *item = getItemById(ctx, ctx.player.inventory[index].item_id);
        if (!item)
            return;
        if (item->value("category", std::string()) != "consumable")
        {
            std::cout << "Item ini bukan consumable.\n";
            return;
        }

        const std::string nameLower = toLower(item->value("name", std::string()));
        int healHp = 0;
        int healMp = 0;
        if (nameLower.find("mana") != std::string::npos)
            healMp = std::max(20, ctx.player.max_mp / 4);
        else
            healHp = std::max(30, ctx.player.max_hp / 4);

        ctx.player.hp = std::min(ctx.player.max_hp, ctx.player.hp + healHp);
        ctx.player.mp = std::min(ctx.player.max_mp, ctx.player.mp + healMp);
        removeItem(ctx.player, ctx.player.inventory[index].item_id, 1);
        std::cout << item->value("name", std::string()) << " digunakan. ";
        if (healHp > 0)
            std::cout << "HP +" << healHp;
        if (healMp > 0)
            std::cout << "MP +" << healMp;
        std::cout << '\n';
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
            const int totalPages = std::max(1, (totalItems + PAGE_SIZE - 1) / PAGE_SIZE);
            page = clampInt(page, 1, totalPages);
            const int start = (page - 1) * PAGE_SIZE;
            const int end = std::min(totalItems, start + PAGE_SIZE);

            printStateHeader(ctx, "INVENTORY");
            std::cout << colorText("Page", Color::Cyan, true) << ": " << page << "/" << totalPages << '\n';
            std::cout << colorText("Gold", Color::Yellow, true) << ": " << ctx.player.gold << "\n\n";
            if (totalItems == 0)
                std::cout << "(Kosong)\n";

            for (int i = start; i < end; ++i)
            {
                const auto &entry = ctx.player.inventory[static_cast<std::size_t>(i)];
                const auto *item = getItemById(ctx, entry.item_id);
                const std::string name = item ? item->value("name", entry.item_id) : entry.item_id;
                const std::string category = item ? item->value("category", std::string()) : "-";
                std::cout << (i - start + 1) << ". " << name << " x" << entry.quantity
                          << " [" << category << "]";
                if (entry.equipped)
                    std::cout << " [Equipped:" << entry.slot << "]";
                std::cout << '\n';
            }

            std::cout << "Input nomor item, q prev, e next, c kembali: ";
            const std::string input = toLower(readLine());
            if (input == "c")
                return;
            if (input == "q")
            {
                page = std::max(1, page - 1);
                continue;
            }
            if (input == "e")
            {
                page = std::min(totalPages, page + 1);
                continue;
            }

            int number = 0;
            if (!tryParseInt(input, number))
            {
                std::cout << "Input tidak valid.\n";
                waitForEnter();
                continue;
            }

            if (number >= 1 && number <= (end - start))
            {
                const std::size_t selectedIndex = static_cast<std::size_t>(start + number - 1);
                const auto *item = getItemById(ctx, ctx.player.inventory[selectedIndex].item_id);
                if (!item)
                {
                    std::cout << "Data item tidak ditemukan.\n";
                    waitForEnter();
                    continue;
                }

                clearScreen();
                printStateHeader(ctx, item->value("name", std::string()));

                int equippedCount = 0;
                for (const auto &entry : ctx.player.inventory)
                {
                    if (entry.item_id == ctx.player.inventory[selectedIndex].item_id && entry.equipped)
                        equippedCount += entry.quantity;
                }

                showItemDetails(ctx, *item, inventoryCount(ctx.player, ctx.player.inventory[selectedIndex].item_id), equippedCount);

                std::cout << "1. Equip/Unequip\n";
                std::cout << "2. Use Consumable\n";
                std::cout << "3. Back\n";
                std::cout << "Choose: ";
                const std::string action = readLine();
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

            std::cout << "Nomor tidak valid.\n";
            waitForEnter();
        }
    }

    void skillMenu(const GameContext &ctx)
    {
        const auto *cls = getClassById(ctx, ctx.player.class_id);
        clearScreen();
        printStateHeader(ctx, "SKILL LIST");
        if (!cls || !cls->contains("skills") || !(*cls)["skills"].is_array())
        {
            std::cout << "Skill tidak tersedia.\n";
            waitForEnter();
            return;
        }

        int index = 1;
        for (const auto &skill : (*cls)["skills"])
        {
            std::cout << colorText(std::to_string(index++) + ". " + skill.value("name", std::string()), Color::Yellow, true) << '\n';
            std::cout << "   Mana: " << skill.value("mana_cost", 0)
                      << " | Cooldown: " << skill.value("cooldown_turns", 0)
                      << " | CD Saat Ini: " << getCooldown(ctx.player, skill.value("id", std::string())) << '\n';
            if (skill.contains("damage_formula"))
                std::cout << "   Formula: " << skill.value("damage_formula", std::string("-")) << '\n';
            std::cout << "   " << skill.value("mechanic_description", std::string("-")) << '\n';
            std::cout << '\n';
        }
        printLine('=');
        waitForEnter();
    }

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

    double normalizedRatio(int value, int minValue, int maxValue)
    {
        if (maxValue <= minValue)
            return 0.0;
        return static_cast<double>(value - minValue) / static_cast<double>(maxValue - minValue);
    }

    int interpolateStat(int level, int levelMin, int levelMax, int statMin, int statMax)
    {
        const double ratio = normalizedRatio(level, levelMin, levelMax);
        const double value = statMin + (statMax - statMin) * ratio;
        return std::max(1, static_cast<int>(std::round(value)));
    }

} // namespace state_helpers
