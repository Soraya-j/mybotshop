(() => {
    const $ = (id) => document.getElementById(id);
    const toDeg = (rad) => (rad == null ? null : (rad * 180 / Math.PI));

    let previousValues = {};
    const maxLinearVelocity = 4.0;
    const maxAngularVelocity = 4.0;
    const arcLength = 126; // SVG arc length

    function isInvalidValue(v) {
        return v == null || (typeof v === 'number' && !Number.isFinite(v));
    }

    function setText(id, val, fmt = (v) => v) {
        const el = $(id);
        if (!el) return;

        const newText = isInvalidValue(val) ? '—' : fmt(val);

        // Trigger animation if value changed
        if (previousValues[id] !== newText && newText !== '—') {
            el.classList.remove('updating');
            void el.offsetWidth; // Force reflow
            el.classList.add('updating');
            setTimeout(() => el.classList.remove('updating'), 500);
        }

        el.textContent = newText;
        previousValues[id] = newText;

        // Add critical class for low battery
        if (id === 'b_pct' && val != null && val < 20) {
            el.classList.add('critical');
        } else {
            el.classList.remove('critical');
        }
    }

    function setBar(id, pct) {
        const el = $(id);
        if (!el) return;
        const p = isInvalidValue(pct) ? 0 : Math.max(0, Math.min(100, pct));
        el.style.width = p + '%';
        el.style.background = p < 20 ? 'linear-gradient(90deg,#ef4444,#dc2626)'
            : p < 50 ? 'linear-gradient(90deg,#f59e0b,#d97706)'
                : 'linear-gradient(90deg,#22c55e,#16a34a)';
    }

    // Battery gauge (vertical fill)
    function setBatteryGauge(pct) {
        const fill = $('b_gauge_fill');
        const display = $('b_pct_display');
        if (!fill) return;

        const p = isInvalidValue(pct) ? 0 : Math.max(0, Math.min(100, pct));
        fill.style.height = p + '%';

        // Remove previous state classes and add current one
        fill.classList.remove('battery-low', 'battery-medium', 'battery-high');
        if (p < 20) {
            fill.style.background = '#ef4444';
            fill.classList.add('battery-low');
        } else if (p < 50) {
            fill.style.background = '#f59e0b';
            fill.classList.add('battery-medium');
        } else {
            fill.style.background = '#22c55e';
            fill.classList.add('battery-high');
        }

        if (display) {
            display.textContent = p > 0 ? Math.round(p) : '—';
        }
    }

    // Velocity arc gauges
    function setArcGauge(id, value, maxValue) {
        const el = $(id);
        if (!el) return;

        const safeValue = isInvalidValue(value) ? 0 : value;
        const percentage = Math.min(Math.abs(safeValue) / maxValue, 1);
        const offset = arcLength - (percentage * arcLength);
        el.style.strokeDashoffset = offset;
    }

    // Speed bar
    function setSpeedBar(vx) {
        const el = $('speed_fill');
        if (!el) return;

        const safeVx = isInvalidValue(vx) ? 0 : vx;
        const percentage = Math.min(Math.abs(safeVx) / maxLinearVelocity, 1) * 100;
        el.style.width = percentage + '%';
    }

    // Compass needle rotation
    function setCompassNeedle(yawDeg) {
        const needle = $('yaw_needle');
        if (!needle) return;

        const angle = isInvalidValue(yawDeg) ? 0 : yawDeg;
        needle.style.transform = `rotate(${angle}deg)`;
    }

    // IMU cube rotation
    function setIMUCube(roll, pitch, yaw) {
        const cube = $('imu_cube');
        if (!cube) return;

        const r = isInvalidValue(roll) ? 0 : roll;
        const p = isInvalidValue(pitch) ? 0 : pitch;
        const y = isInvalidValue(yaw) ? 0 : yaw;
        cube.style.transform = `rotateX(${-p}deg) rotateY(${y}deg) rotateZ(${r}deg)`;
    }

    async function pull() {
        try {
            const r = await fetch('/telemetry_current', { cache: 'no-store' });
            const d = await r.json();

            // Battery
            setText('b_state', d?.battery?.status ?? 'Unknown');
            setText('b_pct', d?.battery?.pct, v => `${v.toFixed(0)}%`);
            setText('b_volt', d?.battery?.volt, v => v.toFixed(2) + ' V');
            setBar('b_bar', d?.battery?.pct);
            setBatteryGauge(d?.battery?.pct);

            // GPS
            setText('g_lat', d?.gps?.lat, v => v.toFixed(7));
            setText('g_lon', d?.gps?.lon, v => v.toFixed(7));

            // Odom
            setText('o_x', d?.odom?.x, v => v.toFixed(3));
            setText('o_y', d?.odom?.y, v => v.toFixed(3));
            const yawDeg = toDeg(d?.odom?.yaw);
            setText('o_yaw', yawDeg, v => v.toFixed(1) + '°');
            setText('o_vx', d?.odom?.vx, v => v.toFixed(3));
            setText('o_wz', d?.odom?.wz, v => v.toFixed(3));

            // Update visual elements
            setCompassNeedle(yawDeg);
            setArcGauge('vx_arc', d?.odom?.vx, maxLinearVelocity);
            setArcGauge('wz_arc', d?.odom?.wz, maxAngularVelocity);
            setSpeedBar(d?.odom?.vx);

            // IMU
            const rollDeg = toDeg(d?.imu?.roll);
            const pitchDeg = toDeg(d?.imu?.pitch);
            const imuYawDeg = toDeg(d?.imu?.yaw);
            setText('i_roll', rollDeg, v => v.toFixed(1) + '°');
            setText('i_pitch', pitchDeg, v => v.toFixed(1) + '°');
            setText('i_yaw', imuYawDeg, v => v.toFixed(1) + '°');

            // Update IMU cube visualization
            setIMUCube(rollDeg, pitchDeg, imuYawDeg);

        } catch (e) {
            console.warn('Telemetry fetch failed:', e);
        }
    }

    // =========================================================================
    // Joint States & Diagnostics (separate endpoint, slower poll rate)
    // =========================================================================
    const DIAG_POLL_INTERVAL = 3000; // 3 seconds - slower for diagnostics
    let previousJointNames = [];

    // Diagnostic level mapping
    const DIAG_LEVELS = { 0: 'OK', 1: 'WARN', 2: 'ERROR', 3: 'STALE' };
    const DIAG_COLORS = { 0: '#22c55e', 1: '#f59e0b', 2: '#ef4444', 3: '#6b7280' };

    function formatValue(val, precision = 4) {
        if (val == null || !Number.isFinite(val)) return '—';
        return val.toFixed(precision);
    }

    function renderJointStates(data) {
        const container = $('joints_container');
        if (!container) return;

        if (!data || !data.name || data.name.length === 0) {
            container.innerHTML = '<div class="joints-loading">No joint data available</div>';
            return;
        }

        const names = data.name;
        const positions = data.position || [];
        const velocities = data.velocity || [];
        const efforts = data.effort || [];

        // Check if we need to rebuild the table (names changed)
        const namesChanged = JSON.stringify(names) !== JSON.stringify(previousJointNames);
        previousJointNames = [...names];

        // Determine which columns have data
        const hasPosition = positions.length > 0;
        const hasVelocity = velocities.length > 0;
        const hasEffort = efforts.length > 0;

        if (namesChanged) {
            // Build table structure
            let html = '<table class="joints-table"><thead><tr><th>Joint</th>';
            if (hasPosition) html += '<th>Position</th>';
            if (hasVelocity) html += '<th>Velocity</th>';
            if (hasEffort) html += '<th>Effort</th>';
            html += '</tr></thead><tbody>';

            names.forEach((name, i) => {
                html += `<tr>`;
                html += `<td class="joint-name">${name}</td>`;
                if (hasPosition) html += `<td class="joint-val" id="jp_${i}">${formatValue(positions[i])}</td>`;
                if (hasVelocity) html += `<td class="joint-val" id="jv_${i}">${formatValue(velocities[i])}</td>`;
                if (hasEffort) html += `<td class="joint-val" id="je_${i}">${formatValue(efforts[i])}</td>`;
                html += `</tr>`;
            });

            html += '</tbody></table>';
            container.innerHTML = html;
        } else {
            // Just update values
            names.forEach((_, i) => {
                if (hasPosition) {
                    const el = $(`jp_${i}`);
                    if (el) el.textContent = formatValue(positions[i]);
                }
                if (hasVelocity) {
                    const el = $(`jv_${i}`);
                    if (el) el.textContent = formatValue(velocities[i]);
                }
                if (hasEffort) {
                    const el = $(`je_${i}`);
                    if (el) el.textContent = formatValue(efforts[i]);
                }
            });
        }
    }

    function renderDiagnostics(data) {
        const container = $('diagnostics_container');
        if (!container) return;

        // Handle null/undefined data or empty statuses array
        const statuses = data?.statuses;
        if (!statuses || !Array.isArray(statuses) || statuses.length === 0) {
            container.innerHTML = '<div class="diagnostics-loading">Waiting for diagnostics...</div>';
            return;
        }

        // Group by hardware_id for better organization
        const groups = {};
        data.statuses.forEach(status => {
            const hw = status.hardware_id || 'General';
            if (!groups[hw]) groups[hw] = [];
            groups[hw].push(status);
        });

        let html = '<div class="diag-groups">';

        Object.entries(groups).forEach(([hwId, statuses]) => {
            html += `<div class="diag-group">`;
            if (hwId !== 'General' && hwId.trim()) {
                html += `<div class="diag-group-header">${hwId}</div>`;
            }

            statuses.forEach(status => {
                const levelName = DIAG_LEVELS[status.level] || 'UNKNOWN';
                const levelColor = DIAG_COLORS[status.level] || '#6b7280';
                const values = status.values || {};
                const valueKeys = Object.keys(values);

                html += `<div class="diag-item">`;
                html += `<div class="diag-header">`;
                html += `<span class="diag-level" style="background:${levelColor}">${levelName}</span>`;
                html += `<span class="diag-name">${status.name}</span>`;
                html += `</div>`;

                if (status.message && status.message.trim()) {
                    html += `<div class="diag-message">${status.message}</div>`;
                }

                if (valueKeys.length > 0) {
                    html += `<div class="diag-values">`;
                    valueKeys.slice(0, 6).forEach(key => { // Limit to 6 values to avoid clutter
                        html += `<div class="diag-kv"><span class="diag-key">${key}:</span> <span class="diag-val">${values[key]}</span></div>`;
                    });
                    if (valueKeys.length > 6) {
                        html += `<div class="diag-kv diag-more">+${valueKeys.length - 6} more...</div>`;
                    }
                    html += `</div>`;
                }

                html += `</div>`;
            });

            html += `</div>`;
        });

        html += '</div>';
        container.innerHTML = html;
    }

    async function pullJointDiag() {
        try {
            const r = await fetch('/telemetry_joint_diag', { cache: 'no-store' });
            const d = await r.json();

            renderJointStates(d?.joint_states);
            renderDiagnostics(d?.diagnostics);

        } catch (e) {
            console.warn('Joint/Diagnostics fetch failed:', e);
            // Show error in UI
            const diagContainer = $('diagnostics_container');
            if (diagContainer) {
                diagContainer.innerHTML = '<div class="diagnostics-loading">Error fetching diagnostics</div>';
            }
        }
    }

    document.addEventListener('DOMContentLoaded', () => {
        pull();
        setInterval(pull, 1000);

        // Joint states and diagnostics at slower rate
        pullJointDiag();
        setInterval(pullJointDiag, DIAG_POLL_INTERVAL);
    });
})();