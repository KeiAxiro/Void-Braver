let gameData = null;

export async function initData() {
    try {
        const response = await fetch('./data/game_data.json');
        if (!response.ok) throw new Error('Failed to load game_data.json');
        gameData = await response.json();
        console.log("Game Data Loaded:", gameData);
        return true;
    } catch (error) {
        console.error("Error loading data:", error);
        return false;
    }
}

export function getData() {
    return gameData;
}

export function getClassById(id) {
    return gameData.classes.find(c => c.id === id);
}

export function getItemById(id) {
    return gameData.items.find(i => i.id === id);
}

export function getEnemyById(id) {
    return gameData.enemies.find(e => e.id === id);
}

export function getDungeonById(id) {
    return gameData.dungeons.find(d => d.id === id);
}

export function getSkillById(id) {
    return gameData.skills.find(s => s.id === id);
}
