#pragma once
#include <vector>

namespace Entities {

    struct MaterialRequirement {
        int itemId;
        int quantity;
    };

    struct Recipe {
        int targetItemId;
        std::vector<MaterialRequirement> materials;
    };

    std::vector<Recipe> getAllRecipes();
    Recipe getRecipe(int targetItemId);
    bool hasRecipe(int targetItemId);

}