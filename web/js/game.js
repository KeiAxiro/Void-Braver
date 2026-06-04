import { getData, getClassById } from './data.js';

let gameState = {
    characters: [],
    activeCharacterIndex: -1
};

export function getPlayer() {
    if (gameState.activeCharacterIndex >= 0 && gameState.activeCharacterIndex < gameState.characters.length) {
        return gameState.characters[gameState.activeCharacterIndex];
    }
    return null;
}

export function loadSaveData() {
    const saved = localStorage.getItem('voidBraverSave');
    if (saved) {
        try {
            gameState = JSON.parse(saved);
        } catch (e) {
            console.error("Save corrupted, creating new", e);
            gameState = { characters: [], activeCharacterIndex: -1 };
        }
    }
}

export function saveGameData() {
    localStorage.setItem('voidBraverSave', JSON.stringify(gameState));
}

export function createNewCharacter(name, classId) {
    const cls = getClassById(classId);
    if (!cls) return null;

    const gameData = getData();
    const balance = gameData.balance || {};

    const baseHp = cls.base_hp || 500;
    const baseMp = cls.base_mp || 100;
    const stats = cls.stats || { str: 10, agi: 10, int: 10, vit: 10, luk: 10 };

    const newChar = {
        name: name,
        class_id: classId,
        level: 1,
        exp: 0,
        gold: 100, // starting gold
        unspent_stat_points: 0,
        
        base_hp: baseHp,
        max_hp: baseHp,
        hp: baseHp,
        
        base_mp: baseMp,
        max_mp: baseMp,
        mp: baseMp,
        
        stats: {
            str: stats.str,
            agi: stats.agi,
            int: stats.int,
            vit: stats.vit,
            luk: stats.luk
        },
        
        inventory: [],
        equipped: {
            weapon: "",
            armor: "",
            accessory: ""
        },
        
        progress: {
            current_dungeon: "",
            current_depth: 0,
            unlocked_dungeons: { "forest_of_beginnings": 1 }
        }
    };

    // Add starting items
    if (cls.starting_items) {
        cls.starting_items.forEach(itemId => {
            newChar.inventory.push({ item_id: itemId, quantity: 1 });
        });
    }

    gameState.characters.push(newChar);
    gameState.activeCharacterIndex = gameState.characters.length - 1;
    saveGameData();
    
    return newChar;
}

export function hasSavedGames() {
    return gameState.characters.length > 0;
}

export function getCharactersList() {
    return gameState.characters;
}

export function setActiveCharacter(index) {
    if (index >= 0 && index < gameState.characters.length) {
        gameState.activeCharacterIndex = index;
        saveGameData();
        return true;
    }
    return false;
}
