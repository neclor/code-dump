const CONFIG = {
    API_URL: '/WowServer',
    REFRESH_INTERVAL: 60000,
    RETRY_DELAY: 2000
};

const UI = {
    authStatus: document.getElementById('authStatus'),
    worldStatus: document.getElementById('worldStatus'),
    onlineCount: document.getElementById('onlineCount'),
    startBtn: document.getElementById('startBtn'),
    systemMessage: document.getElementById('systemMessage'),
    regUsername: document.getElementById('regUsername'),
    regPassword: document.getElementById('regPassword'),
    regMessage: document.getElementById('regMessage')
};

function setRegMessage(text, type = 'info') {
    if (!UI.regMessage) return;
    UI.regMessage.innerText = text;
    UI.regMessage.className = `reg-message msg-${type}`;
}

function updateStatusIndicator(element, isActive) {
    if (!element) return;
    element.innerText = isActive ? 'ONLINE' : 'OFFLINE';
    element.className = `value ${isActive ? 'status-on' : 'status-off'}`;
}

async function fetchStatus() {
    try {
        const response = await fetch(CONFIG.API_URL);
        if (!response.ok) throw new Error(`Server error: ${response.status}`);

        const data = await response.json();
        renderUI(data);
        UI.systemMessage.innerText = '';
    } catch (error) {
        console.error('Status check failed:', error);
        handleConnectionError();
    }
}

function renderUI(data) {
    const isAuthOnline = !!data.auth;
    const isWorldOnline = !!data.world;
    
    const anyServerRunning = isAuthOnline || isWorldOnline;

    updateStatusIndicator(UI.authStatus, isAuthOnline);
    updateStatusIndicator(UI.worldStatus, isWorldOnline);
    UI.onlineCount.innerText = Math.max(0, data.online || 0);

    UI.startBtn.disabled = anyServerRunning;
}

function handleConnectionError() {
    UI.systemMessage.innerText = 'Connection to API lost';
    updateStatusIndicator(UI.authStatus, false);
    updateStatusIndicator(UI.worldStatus, false);
    UI.startBtn.disabled = true;
}

async function handleStart() {
    UI.startBtn.disabled = true;
    UI.systemMessage.innerText = 'Starting servers...';

    try {
        const response = await fetch(`${CONFIG.API_URL}/start`, { method: 'POST' });
        
        if (response.ok) {
            UI.systemMessage.innerText = 'Start command sent successfully.';
        } else {
            UI.systemMessage.innerText = 'Error: Server rejected the command.';
            UI.startBtn.disabled = false;
        }
    } catch (error) {
        UI.systemMessage.innerText = 'Failed to reach the API.';
        UI.startBtn.disabled = false;
    }

    setTimeout(fetchStatus, CONFIG.RETRY_DELAY);
}

async function handleCreateAccount() {
    const name = UI.regUsername.value.trim();
    const password = UI.regPassword.value.trim();

    if (!name || !password) {
        setRegMessage('Fill in all fields!', 'error');
        return;
    }

    setRegMessage('Creating account...', 'info');

    try {
        const url = `${CONFIG.API_URL}/account/create?name=${encodeURIComponent(name)}&password=${encodeURIComponent(password)}`;
        const response = await fetch(url, { method: 'POST' });

        if (response.ok) {
            const result = await response.json();
            
            if (result.created) {
                setRegMessage('Account created successfully!', 'success');
                UI.regUsername.value = '';
                UI.regPassword.value = '';
            } else {
                setRegMessage('Failed: Account already exists or server issue.', 'warning');
            }
        } else {
            const errorText = await response.text();
            setRegMessage(`Server Error: ${response.status}`, 'error');
        }
    } catch (error) {
        setRegMessage('Connection error. Is API running?', 'error');
    }
}

document.addEventListener('DOMContentLoaded', () => {
    fetchStatus();
    setInterval(fetchStatus, CONFIG.REFRESH_INTERVAL);
    if (UI.startBtn) UI.startBtn.onclick = handleStart;
});