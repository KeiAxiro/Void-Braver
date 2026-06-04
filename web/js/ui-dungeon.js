import { getPlayer } from './game.js';
import { getData, getDungeonById } from './data.js';
import { showHubScreen } from './ui.js';
import { startBattle } from './ui-battle.js';

export function showDungeonMenu() {
    const player = getPlayer();
    const gameData = getData();
    const appDiv = document.getElementById('app');

    let dungeonsHtml = '';
    
    // Default unlocked dungeons fallback
    const unlocked = player.progress.unlocked_dungeons || {"forest_of_beginnings": 1};

    Object.keys(unlocked).forEach(dungeonId => {
        const dungeon = getDungeonById(dungeonId);
        if(dungeon) {
            dungeonsHtml += `
                <div class="glass-panel class-card" data-id="${dungeon.id}" style="margin-bottom: 1rem;">
                    <div class="space-between">
                        <div>
                            <h3 style="color: var(--accent-hover); margin-bottom: 0.5rem;">${dungeon.name}</h3>
                            <p style="font-size: 0.9rem; color: var(--text-secondary)">${dungeon.description || "A mysterious place."}</p>
                        </div>
                        <div style="text-align: right;">
                            <span style="color: var(--exp-color); font-weight: bold;">Max Depth: ${unlocked[dungeonId]}</span><br>
                            <button class="btn btn-primary mt-1 btn-enter-dungeon" data-id="${dungeon.id}">Enter Dungeon</button>
                        </div>
                    </div>
                </div>
            `;
        }
    });

    appDiv.innerHTML = `
        <div class="screen active slide-up" style="max-width: 800px; margin: 0 auto; width: 100%;">
            <div class="space-between mb-2">
                <h2>Select Dungeon</h2>
                <button id="btn-back-hub" class="btn">Back to Hub</button>
            </div>
            
            <div class="dungeon-list">
                ${dungeonsHtml}
            </div>
        </div>
    `;

    document.getElementById('btn-back-hub').addEventListener('click', showHubScreen);
    
    const enterBtns = document.querySelectorAll('.btn-enter-dungeon');
    enterBtns.forEach(btn => {
        btn.addEventListener('click', (e) => {
            const dungeonId = e.target.getAttribute('data-id');
            startBattle(dungeonId);
        });
    });
}
