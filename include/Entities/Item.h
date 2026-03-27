#pragma once
#include <string>
#include <vector>

namespace Entities {

    struct ItemTemplate {
        int id;
        int categoryId; // 1:Consumable, 2:Weapon, 3:Helmet, 4:Armor, 5:Boots, 6:Accessory, 7:Material
        std::string name;
        std::string description;
        int buyPrice;
        int sellPrice;
        int reqClass; // 0: All, 1: Knight, 2: Sorcerer, 3: Wanderer
        int tier;
        
        // Attributes (0 if not equipment)
        int atkBonus;
        int hpBonus;
        int defBonus;
        std::string specialEffect;
    };

    std::vector<ItemTemplate> getAllItemTemplates();
    ItemTemplate getItemTemplate(int id);
    ItemTemplate getItemTemplateByName(const std::string& name);
    std::string getCategoryName(int categoryId);

}