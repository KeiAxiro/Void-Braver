import { getData, getClassById } from './data.js';
import { getPlayer, createNewCharacter, hasSavedGames, getCharactersList, setActiveCharacter } from './game.js';
import { showCharacterMenu } from './ui-character.js';
import { showDungeonMenu } from './ui-dungeon.js';

const appDiv = document.getElementById('app');

export function showLoading() {
    appDiv.innerHTML = `
        <div class="loading-screen fade-in">
            <h2>Loading Game Data...</h2>
            <div class="spinner"></div>
        </div>
    `;
}

export function showMainMenu() {
    const hasSaves = hasSavedGames();
    
    appDiv.innerHTML = `
        <div class="screen active main-menu fade-in">
            <h1>Void-Braver</h1>
            <p style="color: var(--text-secondary); margin-bottom: 2rem;">Web Edition</p>
            
            <div class="glass-panel menu-options">
                <button id="btn-new-game" class="btn btn-primary">New Game</button>
                ${hasSaves ? `<button id="btn-load-game" class="btn">Load Game</button>` : ''}
            </div>
        </div>
    `;

    document.getElementById('btn-new-game').addEventListener('click', showNewGameScreen);
    if (hasSaves) {
        document.getElementById('btn-load-game').addEventListener('click', showLoadGameScreen);
    }
}

export function showNewGameScreen() {
    const gameData = getData();
    
    let html = `
        <div class="screen active fade-in" style="justify-content: center;">
            <div class="glass-panel" style="max-width: 800px; width: 100%; margin: 0 auto;">
                <h2>Create Character</h2>
                <input type="text" id="char-name" class="input-field" placeholder="Enter Character Name" maxlength="20">
                
                <h3 class="mt-1 mb-1">Select Class</h3>
                <div class="grid-3" id="class-selection">
    `;

    gameData.classes.forEach((cls, index) => {
        const baseHp = cls.base_hp || 500;
        const baseMp = cls.base_mp || 100;
        const stats = cls.stats || { str: 10, agi: 10, int: 10, vit: 10, luk: 10 };
        
        html += `
            <div class="glass-panel class-card ${index === 0 ? 'selected' : ''}" data-class-id="${cls.id}">
                <h3 style="color: var(--accent-hover)">${cls.name}</h3>
                <p style="font-size: 0.9rem; color: var(--text-secondary); margin-bottom: 1rem;">${cls.description || 'A valiant hero.'}</p>
                <div style="font-size: 0.85rem;">
                    <div>HP: ${baseHp} | MP: ${baseMp}</div>
                    <div>STR: ${stats.str} | AGI: ${stats.agi}</div>
                    <div>INT: ${stats.int} | VIT: ${stats.vit}</div>
                    <div>LUK: ${stats.luk}</div>
                </div>
            </div>
        `;
    });

    html += `
                </div>
                <div class="space-between mt-2">
                    <button id="btn-cancel" class="btn">Back</button>
                    <button id="btn-create" class="btn btn-primary">Begin Journey</button>
                </div>
            </div>
        </div>
    `;

    appDiv.innerHTML = html;

    let selectedClassId = gameData.classes[0].id;
    const cards = document.querySelectorAll('.class-card');
    cards.forEach(card => {
        card.addEventListener('click', () => {
            cards.forEach(c => c.classList.remove('selected'));
            card.classList.add('selected');
            selectedClassId = card.getAttribute('data-class-id');
        });
    });

    document.getElementById('btn-cancel').addEventListener('click', showMainMenu);
    document.getElementById('btn-create').addEventListener('click', () => {
        const name = document.getElementById('char-name').value.trim() || 'Hero';
        createNewCharacter(name, selectedClassId);
        showHubScreen();
    });
}

export function showLoadGameScreen() {
    const chars = getCharactersList();
    
    let html = `
        <div class="screen active fade-in" style="justify-content: center;">
            <div class="glass-panel" style="max-width: 600px; width: 100%; margin: 0 auto;">
                <h2>Load Game</h2>
                <div class="flex-col mt-1 mb-2">
    `;

    chars.forEach((char, index) => {
        const cls = getClassById(char.class_id);
        html += `
            <div class="glass-panel class-card" style="display:flex; justify-content:space-between; align-items:center;" data-index="${index}">
                <div>
                    <h3 style="margin:0; color: var(--accent-hover)">${char.name}</h3>
                    <div style="font-size: 0.9rem; color: var(--text-secondary)">Lv. ${char.level} ${cls ? cls.name : 'Unknown'}</div>
                </div>
                <div style="text-align: right; font-size: 0.9rem;">
                    <div class="text-gold">${char.gold} G</div>
                    <div>Dungeon: Depth ${char.progress.current_depth}</div>
                </div>
            </div>
        `;
    });

    html += `
                </div>
                <button id="btn-cancel-load" class="btn">Back</button>
            </div>
        </div>
    `;

    appDiv.innerHTML = html;

    document.getElementById('btn-cancel-load').addEventListener('click', showMainMenu);
    
    const cards = document.querySelectorAll('.class-card');
    cards.forEach(card => {
        card.addEventListener('click', () => {
            const index = parseInt(card.getAttribute('data-index'));
            setActiveCharacter(index);
            showHubScreen();
        });
    });
}

export function showHubScreen() {
    const player = getPlayer();
    if (!player) return showMainMenu();
    
    const cls = getClassById(player.class_id);
    const hpPercent = (player.hp / player.max_hp) * 100;
    const mpPercent = (player.mp / player.max_mp) * 100;

    appDiv.innerHTML = `
        <div class="screen active slide-up" style="max-width: 1000px; margin: 0 auto; width: 100%;">
            <div class="glass-panel status-bar">
                <div>
                    <h2>${player.name}</h2>
                    <div style="color: var(--text-secondary)">Lv. ${player.level} ${cls.name}</div>
                </div>
                <div class="status-stats" style="flex: 1; margin-left: 2rem; max-width: 400px;">
                    <div style="flex: 1;">
                        <div class="space-between" style="font-size: 0.85rem; font-weight: bold;">
                            <span style="color: var(--health-color)">HP</span>
                            <span>${player.hp}/${player.max_hp}</span>
                        </div>
                        <div class="bar-container"><div class="bar-fill hp" style="width: ${hpPercent}%"></div></div>
                    </div>
                    <div style="flex: 1;">
                        <div class="space-between" style="font-size: 0.85rem; font-weight: bold;">
                            <span style="color: var(--mana-color)">MP</span>
                            <span>${player.mp}/${player.max_mp}</span>
                        </div>
                        <div class="bar-container"><div class="bar-fill mp" style="width: ${mpPercent}%"></div></div>
                    </div>
                </div>
                <div style="margin-left: 2rem; text-align: right;">
                    <div class="text-gold stat-value">${player.gold} G</div>
                </div>
            </div>

            <div class="grid-3">
                <div class="glass-panel" style="display:flex; flex-direction:column; justify-content:center; align-items:center; cursor:pointer;" id="hub-dungeon">
                    <h3 style="color: var(--health-color)">Enter Dungeon</h3>
                    <p style="color: var(--text-secondary); text-align: center; font-size: 0.9rem;">Explore and battle monsters</p>
                </div>
                <div class="glass-panel" style="display:flex; flex-direction:column; justify-content:center; align-items:center; cursor:pointer;" id="hub-character">
                    <h3 style="color: var(--accent-hover)">Character</h3>
                    <p style="color: var(--text-secondary); text-align: center; font-size: 0.9rem;">View stats and inventory</p>
                </div>
                <div class="glass-panel" style="display:flex; flex-direction:column; justify-content:center; align-items:center; cursor:pointer;" id="hub-shop">
                    <h3 class="text-gold">Shop</h3>
                    <p style="color: var(--text-secondary); text-align: center; font-size: 0.9rem;">Buy and sell items</p>
                </div>
                <div class="glass-panel" style="display:flex; flex-direction:column; justify-content:center; align-items:center; cursor:pointer;" id="hub-craft">
                    <h3 style="color: #a78bfa">Crafting</h3>
                    <p style="color: var(--text-secondary); text-align: center; font-size: 0.9rem;">Combine materials</p>
                </div>
                <div class="glass-panel" style="display:flex; flex-direction:column; justify-content:center; align-items:center; cursor:pointer;" id="hub-save">
                    <h3 style="color: var(--exp-color)">Save Game</h3>
                    <p style="color: var(--text-secondary); text-align: center; font-size: 0.9rem;">Save your progress</p>
                </div>
                <div class="glass-panel" style="display:flex; flex-direction:column; justify-content:center; align-items:center; cursor:pointer;" id="hub-quit">
                    <h3 style="color: var(--text-secondary)">Main Menu</h3>
                    <p style="color: var(--text-secondary); text-align: center; font-size: 0.9rem;">Return to title</p>
                </div>
            </div>
        </div>
    `;

    document.getElementById('hub-dungeon').addEventListener('click', showDungeonMenu);
    document.getElementById('hub-character').addEventListener('click', showCharacterMenu);
    document.getElementById('hub-shop').addEventListener('click', showShopScreen);
    document.getElementById('hub-craft').addEventListener('click', showCraftingScreen);
    
    document.getElementById('hub-save').addEventListener('click', () => {
        const btn = document.getElementById('hub-save');
        btn.style.boxShadow = "0 0 20px var(--exp-color)";
        setTimeout(() => btn.style.boxShadow = "", 500);
    });
    
    document.getElementById('hub-quit').addEventListener('click', showMainMenu);
}

// Temporary stubs for shop and crafting
function showShopScreen() {
    appDiv.innerHTML = `
        <div class="screen active slide-up" style="max-width: 600px; margin: 0 auto; width: 100%; text-align: center;">
            <div class="glass-panel">
                <h2 class="text-gold">The Merchant</h2>
                <p style="margin: 2rem 0; color: var(--text-secondary)">"Ah, welcome! My shop is currently being renovated.<br>Come back later!"</p>
                <button id="btn-back-hub" class="btn">Back to Hub</button>
            </div>
        </div>
    `;
    document.getElementById('btn-back-hub').addEventListener('click', showHubScreen);
}

function showCraftingScreen() {
    appDiv.innerHTML = `
        <div class="screen active slide-up" style="max-width: 600px; margin: 0 auto; width: 100%; text-align: center;">
            <div class="glass-panel">
                <h2 style="color: #a78bfa">The Forge</h2>
                <p style="margin: 2rem 0; color: var(--text-secondary)">"The forge fires are currently unlit.<br>Check back soon to combine your materials."</p>
                <button id="btn-back-hub" class="btn">Back to Hub</button>
            </div>
        </div>
    `;
    document.getElementById('btn-back-hub').addEventListener('click', showHubScreen);
}
