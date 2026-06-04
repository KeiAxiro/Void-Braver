import { initData } from './data.js';
import { loadSaveData } from './game.js';
import { showLoading, showMainMenu } from './ui.js';

async function bootstrap() {
    showLoading();
    
    // Load local storage saves
    loadSaveData();
    
    // Fetch JSON data
    const success = await initData();
    if (!success) {
        document.getElementById('app').innerHTML = `
            <div class="glass-panel" style="text-align: center; max-width: 400px; margin: 0 auto; margin-top: 100px;">
                <h2 style="color: var(--health-color)">Error</h2>
                <p>Failed to load game data.<br>Make sure you are running a local server, e.g. <code>python -m http.server</code></p>
            </div>
        `;
        return;
    }
    
    // Small delay for smooth transition
    setTimeout(() => {
        showMainMenu();
    }, 500);
}

// Start application
window.addEventListener('DOMContentLoaded', bootstrap);
