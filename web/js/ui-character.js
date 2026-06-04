import { getPlayer } from './game.js';
import { getClassById, getItemById } from './data.js';
import { showHubScreen } from './ui.js';

export function showCharacterMenu() {
    const player = getPlayer();
    const cls = getClassById(player.class_id);
    const appDiv = document.getElementById('app');

    let inventoryHtml = '';
    if (player.inventory.length === 0) {
        inventoryHtml = '<p style="color: var(--text-secondary)">Inventory is empty.</p>';
    } else {
        player.inventory.forEach(entry => {
            const item = getItemById(entry.item_id);
            if(item) {
                inventoryHtml += `
                    <div class="glass-panel" style="padding: 1rem; margin-bottom: 0.5rem; display: flex; justify-content: space-between;">
                        <div>
                            <strong style="color: var(--accent-hover)">${item.name}</strong> x${entry.quantity}
                            <div style="font-size: 0.8rem; color: var(--text-secondary)">${item.description}</div>
                        </div>
                    </div>
                `;
            }
        });
    }

    appDiv.innerHTML = `
        <div class="screen active slide-up" style="max-width: 1000px; margin: 0 auto; width: 100%;">
            <div class="space-between mb-2">
                <h2>Character Details</h2>
                <button id="btn-back-hub" class="btn">Back to Hub</button>
            </div>
            
            <div class="grid-2">
                <div class="glass-panel">
                    <h3 style="color: var(--accent-color)">${player.name}</h3>
                    <p>Level ${player.level} ${cls.name}</p>
                    <hr style="border-color: var(--glass-border); margin: 1rem 0;">
                    
                    <div class="grid-2">
                        <div>
                            <p><strong>HP:</strong> <span style="color: var(--health-color)">${player.hp}/${player.max_hp}</span></p>
                            <p><strong>MP:</strong> <span style="color: var(--mana-color)">${player.mp}/${player.max_mp}</span></p>
                            <p><strong>Gold:</strong> <span class="text-gold">${player.gold}</span></p>
                            <p><strong>EXP:</strong> <span style="color: var(--exp-color)">${player.exp}</span></p>
                        </div>
                        <div>
                            <p><strong>STR:</strong> ${player.stats.str}</p>
                            <p><strong>AGI:</strong> ${player.stats.agi}</p>
                            <p><strong>INT:</strong> ${player.stats.int}</p>
                            <p><strong>VIT:</strong> ${player.stats.vit}</p>
                            <p><strong>LUK:</strong> ${player.stats.luk}</p>
                        </div>
                    </div>
                </div>

                <div class="glass-panel" style="max-height: 500px; overflow-y: auto;">
                    <h3 style="color: var(--accent-color)">Inventory</h3>
                    ${inventoryHtml}
                </div>
            </div>
        </div>
    `;

    document.getElementById('btn-back-hub').addEventListener('click', showHubScreen);
}
