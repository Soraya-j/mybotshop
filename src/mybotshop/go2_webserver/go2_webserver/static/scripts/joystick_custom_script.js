/*
Software License Agreement (BSD)

@author    Salman Omar Sohail <support@mybotshop.de>
@copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.
*/
document.addEventListener('DOMContentLoaded', () => {
    // ------------------------------------------------------------------------------ Joystick Setup for two joysticks

    function setupJoystick(joystickId, zoneId) {
        let joystickInterval = null;
        let lastAngle = 0;
        let lastDistance = 0;

        // Calculate responsive size based on container
        const container = document.getElementById(zoneId);
        const containerWidth = container.offsetWidth;
        const joystickSize = Math.min(containerWidth, 200);

        const joystick = nipplejs.create({
            zone: container,
            color: 'var(--project-color)',
            mode: 'dynamic',
            size: joystickSize,
        });

        joystick.on('move', (event, data) => {
            lastAngle = data.angle.degree;
            lastDistance = data.distance;

            if (!joystickInterval) {
                joystickInterval = setInterval(() => {
                    sendJoystickDataToBackend(joystickId, lastAngle, lastDistance);
                }, 100);
            }
        });

        joystick.on('end', () => {
            clearInterval(joystickInterval);
            joystickInterval = null;
            sendJoystickDataToBackend(joystickId, 0, 0); // stop movement for this joystick
        });
    }

    // Track connection status for user feedback
    let connectionErrorCount = 0;
    const maxRetries = 3;

    function sendJoystickDataToBackend(joystickId, angle, distance, retryCount = 0) {
        fetch('/web_joystick_control', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ joystickId, angle, distance }),
        })
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP ${response.status}`);
            }
            return response.json();
        })
        .then(data => {
            // Reset error count on success
            if (connectionErrorCount > 0) {
                connectionErrorCount = 0;
            }
        })
        .catch(error => {
            console.error(`Error sending joystick ${joystickId} data:`, error);
            connectionErrorCount++;

            // Retry with exponential backoff for non-zero movements
            if (retryCount < maxRetries && (angle !== 0 || distance !== 0)) {
                const delay = Math.pow(2, retryCount) * 100;
                setTimeout(() => {
                    sendJoystickDataToBackend(joystickId, angle, distance, retryCount + 1);
                }, delay);
            }
        });
    }

    // Initialize both joysticks with their respective zone IDs and IDs
    setupJoystick('joystick1', 'joystick1');
    setupJoystick('joystick2', 'joystick2');
    setupJoystick('joystick1', 'joystick1b');
    setupJoystick('joystick2', 'joystick2b');
});
