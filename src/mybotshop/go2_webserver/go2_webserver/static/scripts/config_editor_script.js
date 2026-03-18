/*
Software License Agreement (BSD)

@author    Salman Omar Sohail <support@mybotshop.de>
@copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.
*/

// Webserver Configuration Editor
let currentConfig = {};

// Config fields to display (key: label)
const configFields = {
    'robot_cmd_vel': { label: 'CMD Vel Topic', type: 'text' },
    'robot_odom_topic': { label: 'Odom Topic', type: 'text' },
    'robot_battery_topic': { label: 'Battery Topic', type: 'text' },
    'robot_gps_topic': { label: 'GPS Topic', type: 'text' },
    'robot_map_topic': { label: 'Map Topic', type: 'text' },
    'robot_e_stop': { label: 'E-Stop Topic', type: 'text' },
    'robot_max_linear_velocity': { label: 'Max Linear Vel', type: 'number', step: '0.1' },
    'robot_max_angular_velocity': { label: 'Max Angular Vel', type: 'number', step: '0.1' },
    'robot_twist_stamped': { label: 'Twist Stamped', type: 'checkbox' },
    'web_server_port': { label: 'Web Port', type: 'number' },
    'web_server_threads': { label: 'Web Threads', type: 'number' },
    'robot_rosbag_dir': { label: 'Rosbag Directory', type: 'text', fullWidth: true },
};

async function loadWebserverConfig() {
    const editor = document.getElementById('config-editor');
    const servicesEditor = document.getElementById('services-editor');

    if (!editor) return; // Not on settings page

    try {
        const response = await fetch('/api/webserver_config');
        if (!response.ok) throw new Error('Failed to load config');
        currentConfig = await response.json();

        // Render config fields
        let html = '';
        for (const [key, field] of Object.entries(configFields)) {
            const value = currentConfig[key] ?? '';
            const fullWidth = field.fullWidth ? ' full-width' : '';

            if (field.type === 'checkbox') {
                const checked = value ? 'checked' : '';
                html += `
                    <div class="config-item${fullWidth}">
                        <label class="config-label">${field.label}</label>
                        <input type="checkbox" class="config-input" data-key="${key}" ${checked}>
                    </div>`;
            } else {
                const step = field.step ? `step="${field.step}"` : '';
                html += `
                    <div class="config-item${fullWidth}">
                        <label class="config-label">${field.label}</label>
                        <input type="${field.type}" class="config-input" data-key="${key}" value="${value}" ${step}>
                    </div>`;
            }
        }
        editor.innerHTML = html;

        // Render services list
        if (servicesEditor) {
            renderServices();
        }

    } catch (error) {
        editor.innerHTML = `<div class="config-loading" style="color: var(--project-error-color);">Error: ${error.message}</div>`;
        if (servicesEditor) servicesEditor.innerHTML = '';
    }
}

async function saveWebserverConfig() {
    const statusEl = document.getElementById('config-status');
    if (!statusEl) return;

    statusEl.className = 'config-status';
    statusEl.textContent = 'Saving...';

    try {
        // Collect values from inputs
        const inputs = document.querySelectorAll('#config-editor .config-input');
        const updates = {};

        inputs.forEach(input => {
            const key = input.dataset.key;
            if (input.type === 'checkbox') {
                updates[key] = input.checked;
            } else if (input.type === 'number') {
                updates[key] = parseFloat(input.value) || 0;
            } else {
                updates[key] = input.value;
            }
        });

        // Include services
        updates.robot_services = currentConfig.robot_services || [];

        const response = await fetch('/api/webserver_config', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(updates)
        });

        const result = await response.json();
        if (!response.ok) throw new Error(result.error || 'Save failed');

        statusEl.className = 'config-status success';
        statusEl.textContent = 'Configuration saved! Restart webserver to apply changes.';
    } catch (error) {
        statusEl.className = 'config-status error';
        statusEl.textContent = `Error: ${error.message}`;
    }
}

function removeService(index) {
    if (!confirm('Remove this service?')) return;
    currentConfig.robot_services.splice(index, 1);
    renderServices();
}

function addService() {
    const input = document.getElementById('new-service-input');
    if (!input) return;

    const name = input.value.trim();
    if (!name) return;

    if (!currentConfig.robot_services) currentConfig.robot_services = [];
    if (!currentConfig.robot_services.includes(name)) {
        currentConfig.robot_services.push(name);
        renderServices();
    }
    input.value = '';
}

function renderServices() {
    const servicesEditor = document.getElementById('services-editor');
    if (!servicesEditor) return;

    const services = currentConfig.robot_services || [];
    let html = '';
    services.forEach((service, index) => {
        html += `
            <div class="service-item">
                <span class="service-name">${service}</span>
                <button class="service-delete" onclick="removeService(${index})" title="Remove">
                    <i class="fas fa-trash"></i>
                </button>
            </div>`;
    });
    servicesEditor.innerHTML = html || '<div class="config-loading">No services configured</div>';
}

// Load config on page load
document.addEventListener('DOMContentLoaded', loadWebserverConfig);
