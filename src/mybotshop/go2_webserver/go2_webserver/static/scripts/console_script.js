/*
Software License Agreement (BSD)

@author    Salman Omar Sohail <support@mybotshop.de>
@copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.
*/
document.addEventListener('DOMContentLoaded', () => {
    const terminalInput = document.getElementById('terminal_input');

    // =============================================
    // Live Velocity Display - Fetches from telemetry
    // =============================================
    const velocityElements = {
        linearValue: document.getElementById('linearVelocityValue'),
        angularValue: document.getElementById('angularVelocityValue'),
        linearGauge: document.getElementById('linearGaugeFill'),
        angularGauge: document.getElementById('angularGaugeFill'),
        vx: document.getElementById('vxValue'),
        vy: document.getElementById('vyValue'),
        vz: document.getElementById('vzValue'),
        wz: document.getElementById('wzValue'),
        linearMax: document.getElementById('linearMaxLabel'),
        angularMax: document.getElementById('angularMaxLabel')
    };

    // Max velocity settings - fetched from backend config
    let maxLinearVelocity = 2.0;  // m/s (default)
    let maxAngularVelocity = 2.5; // rad/s (default)

    // Store interval ID for cleanup
    let velocityIntervalId = null;

    // SVG arc length for gauge (half circle)
    const gaugeArcLength = 157;

    function updateGauge(gaugeEl, value, maxValue) {
        if (!gaugeEl) return;
        const percentage = Math.min(Math.abs(value) / maxValue, 1);
        const offset = gaugeArcLength - (percentage * gaugeArcLength);
        gaugeEl.style.strokeDashoffset = offset;
    }

    function updateVelocityDisplay(data) {
        if (!data || !data.odom) return;

        const vx = data.odom.vx ?? 0;
        const vy = data.odom.vy ?? 0;
        const vz = data.odom.vz ?? 0;
        const wz = data.odom.wz ?? 0;

        // Calculate linear velocity magnitude (for 2D: sqrt(vx^2 + vy^2))
        const linearMagnitude = Math.sqrt(vx * vx + vy * vy);
        const angularMagnitude = Math.abs(wz);

        // Update numeric displays
        if (velocityElements.linearValue) {
            velocityElements.linearValue.textContent = linearMagnitude.toFixed(2);
        }
        if (velocityElements.angularValue) {
            velocityElements.angularValue.textContent = angularMagnitude.toFixed(2);
        }

        // Update gauges
        updateGauge(velocityElements.linearGauge, linearMagnitude, maxLinearVelocity);
        updateGauge(velocityElements.angularGauge, angularMagnitude, maxAngularVelocity);

        // Update vector components
        if (velocityElements.vx) velocityElements.vx.textContent = vx.toFixed(2);
        if (velocityElements.vy) velocityElements.vy.textContent = vy.toFixed(2);
        if (velocityElements.vz) velocityElements.vz.textContent = vz.toFixed(2);
        if (velocityElements.wz) velocityElements.wz.textContent = wz.toFixed(2);

        // Update max labels if needed
        if (velocityElements.linearMax) velocityElements.linearMax.textContent = maxLinearVelocity.toFixed(1);
        if (velocityElements.angularMax) velocityElements.angularMax.textContent = maxAngularVelocity.toFixed(1);
    }

    async function fetchVelocityData() {
        try {
            const response = await fetch('/telemetry_current', { cache: 'no-store' });
            const data = await response.json();
            updateVelocityDisplay(data);
        } catch (error) {
            console.warn('Velocity fetch failed:', error);
        }
    }

    // Fetch max velocity config from backend
    async function fetchVelocityConfig() {
        try {
            const response = await fetch('/get_velocity_config', { cache: 'no-store' });
            if (response.ok) {
                const config = await response.json();
                maxLinearVelocity = config.max_linear || maxLinearVelocity;
                maxAngularVelocity = config.max_angular || maxAngularVelocity;
            }
        } catch (error) {
            console.warn('Using default velocity config:', error);
        }
    }

    // Start fetching velocity data if elements exist
    if (velocityElements.linearValue || velocityElements.vx) {
        fetchVelocityConfig();
        fetchVelocityData();
        velocityIntervalId = setInterval(fetchVelocityData, 500);
    }

    // Cleanup interval on page unload to prevent memory leaks
    window.addEventListener('beforeunload', () => {
        if (velocityIntervalId) {
            clearInterval(velocityIntervalId);
            velocityIntervalId = null;
        }
    });

    // Function for terminal submission
    window.sendTerminalData = function () {
        const audio = document.getElementById('buttonClickSound');
        if (audio) {
            audio.currentTime = 0;  // Rewind to start
            audio.play();
        }

        const data = {
            terminal_input: terminalInput.value
        };

        fetch('/terminal_submission', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        })
            .then(response => response.json())
            .then(responseData => {
                const responseMessage = document.getElementById('responseMessage');
                responseMessage.innerText = `${responseData.message}`;
                responseMessage.style.color = 'var(--project-color)';
            })
            .catch(() => {
                const responseMessage = document.getElementById('responseMessage');
                responseMessage.innerText = 'Error sending data';
                responseMessage.style.color = 'var(--project-error-color)';
            });
    };

    window.sendClearData = function () {
        const responseMessage = document.getElementById('responseMessage');
        responseMessage.innerText = 'Logs Cleared';
        responseMessage.style.color = '';
    };

    // Function for WebButton
    window.webButton = function (action) {

        const customAudio = document.getElementById(action + 'Sound');
        const defaultAudio = document.getElementById('buttonClickSound');

        const audio = customAudio || defaultAudio; // Prefer custom, fallback to default

        if (audio) {
            audio.currentTime = 0;  // Rewind to start
            audio.play();
        }

        fetch('/web_button', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                action: action,
            }),
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                return response.json();
            })
            .then(data => {
                console.log('Server Response:', data);

                // Display success message dynamically
                const responseMessage = document.getElementById('responseMessage');
                responseMessage.innerText = data.message || `Action "${action}" performed successfully.`;
                responseMessage.style.color = 'var(--project-color)';
            })
            .catch(error => {
                console.error('Error:', error);

                // Display error message dynamically
                const responseMessage = document.getElementById('responseMessage');
                responseMessage.innerText = `Failed to perform action "${action}".`;
                responseMessage.style.color = 'var(--project-error-color)';
            });
    };
});
