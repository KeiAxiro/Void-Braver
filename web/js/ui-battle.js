import { getPlayer, saveGameData } from './game.js';
import { getDungeonById, getEnemyById, getData } from './data.js';
import { showHubScreen } from './ui.js';

let battleState = null;

export function startBattle(dungeonId) {
    const player = getPlayer();
    const dungeon = getDungeonById(dungeonId);
    
    // Pick a random enemy from the dungeon depths
    // For simplicity, we just pick the first enemy in depth 1
    let enemyId = "slime";
    if (dungeon && dungeon.depths && dungeon.depths.length > 0) {
        const depthEnemies = dungeon.depths[0].enemies;
        if(depthEnemies && depthEnemies.length > 0) {
            enemyId = depthEnemies[Math.floor(Math.random() * depthEnemies.length)];
        }
    }
    
    const enemyTemplate = getEnemyById(enemyId);
    
    battleState = {
        dungeon: dungeon,
        enemy: {
            name: enemyTemplate.name,
            max_hp: enemyTemplate.base_hp,
            hp: enemyTemplate.base_hp,
            attack: enemyTemplate.stats.str * 2, // Basic scaling
            exp: enemyTemplate.exp_reward || 10,
            gold: enemyTemplate.gold_reward || 5
        },
        log: ["A wild " + enemyTemplate.name + " appeared!"]
    };
    
    renderBattleScreen();
}

function renderBattleScreen() {
    const player = getPlayer();
    const enemy = battleState.enemy;
    const appDiv = document.getElementById('app');
    
    const playerHpPercent = Math.max(0, (player.hp / player.max_hp) * 100);
    const enemyHpPercent = Math.max(0, (enemy.hp / enemy.max_hp) * 100);

    let logHtml = battleState.log.slice(-5).map(msg => `<div style="margin-bottom: 0.5rem; color: var(--text-secondary);">${msg}</div>`).join('');

    appDiv.innerHTML = `
        <div class="screen active fade-in" style="max-width: 900px; margin: 0 auto; width: 100%;">
            <div class="space-between mb-2">
                <h2>Battle - ${battleState.dungeon ? battleState.dungeon.name : 'Unknown'}</h2>
                <button id="btn-run" class="btn">Run Away</button>
            </div>
            
            <div class="grid-2 mb-2">
                <!-- Player Side -->
                <div class="glass-panel" style="border-color: var(--accent-color)">
                    <h3 style="color: var(--accent-hover)">${player.name}</h3>
                    <div class="space-between" style="font-weight: bold; margin-bottom: 0.5rem;">
                        <span style="color: var(--health-color)">HP: ${player.hp}/${player.max_hp}</span>
                    </div>
                    <div class="bar-container mb-1"><div class="bar-fill hp" style="width: ${playerHpPercent}%"></div></div>
                    
                    <div class="space-between" style="font-weight: bold; margin-bottom: 0.5rem;">
                        <span style="color: var(--mana-color)">MP: ${player.mp}/${player.max_mp}</span>
                    </div>
                    <div class="bar-container"><div class="bar-fill mp" style="width: Math.max(0, (player.mp / player.max_mp) * 100)%"></div></div>
                </div>
                
                <!-- Enemy Side -->
                <div class="glass-panel" style="border-color: var(--health-color)">
                    <h3 style="color: #ef4444">${enemy.name}</h3>
                    <div class="space-between" style="font-weight: bold; margin-bottom: 0.5rem;">
                        <span style="color: var(--health-color)">HP: ${enemy.hp}/${enemy.max_hp}</span>
                    </div>
                    <div class="bar-container"><div class="bar-fill hp" style="width: ${enemyHpPercent}%"></div></div>
                </div>
            </div>
            
            <div class="glass-panel mb-2" style="height: 150px; overflow-y: auto; background: rgba(0,0,0,0.5);" id="battle-log">
                ${logHtml}
            </div>
            
            <div class="grid-3" id="battle-actions">
                <button class="btn btn-primary" id="btn-attack">Attack</button>
                <button class="btn" id="btn-skill">Skills</button>
                <button class="btn" id="btn-item">Items</button>
            </div>
        </div>
    `;

    // Scroll log to bottom
    const logDiv = document.getElementById('battle-log');
    if(logDiv) logDiv.scrollTop = logDiv.scrollHeight;

    document.getElementById('btn-run').addEventListener('click', () => {
        alert("You ran away safely!");
        showHubScreen();
    });
    
    const attackBtn = document.getElementById('btn-attack');
    if(attackBtn) {
        attackBtn.addEventListener('click', processTurn);
    }
}

function processTurn() {
    const player = getPlayer();
    const enemy = battleState.enemy;
    
    // Player attacks
    const pDamage = Math.max(1, player.stats.str * 2 + Math.floor(Math.random() * 5));
    enemy.hp -= pDamage;
    battleState.log.push(`You attacked ${enemy.name} for <span style="color:var(--health-color)">${pDamage}</span> damage!`);
    
    if (enemy.hp <= 0) {
        enemy.hp = 0;
        battleState.log.push(`${enemy.name} was defeated!`);
        battleState.log.push(`Gained ${enemy.exp} EXP and ${enemy.gold} Gold.`);
        
        player.exp += enemy.exp;
        player.gold += enemy.gold;
        saveGameData();
        
        renderBattleScreen();
        
        // Hide actions
        document.getElementById('battle-actions').innerHTML = `
            <button class="btn btn-primary" id="btn-victory" style="grid-column: span 3">Continue</button>
        `;
        document.getElementById('btn-victory').addEventListener('click', showHubScreen);
        return;
    }
    
    // Enemy attacks
    const eDamage = Math.max(1, enemy.attack - player.stats.vit + Math.floor(Math.random() * 3));
    player.hp -= eDamage;
    battleState.log.push(`${enemy.name} attacked you for <span style="color:var(--health-color)">${eDamage}</span> damage!`);
    
    if (player.hp <= 0) {
        player.hp = 0;
        battleState.log.push(`You were defeated...`);
        // Basic death penalty: heal up to 10% and return
        player.hp = Math.max(1, Math.floor(player.max_hp * 0.1));
        saveGameData();
        
        renderBattleScreen();
        document.getElementById('battle-actions').innerHTML = `
            <button class="btn btn-primary" id="btn-defeat" style="grid-column: span 3">Return to Hub</button>
        `;
        document.getElementById('btn-defeat').addEventListener('click', showHubScreen);
        return;
    }
    
    saveGameData();
    renderBattleScreen();
}
