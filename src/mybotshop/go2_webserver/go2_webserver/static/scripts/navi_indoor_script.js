class RobotMapViewer {
    constructor(canvasId) {
        this.canvas = document.getElementById(canvasId);
        this.ctx = this.canvas.getContext('2d');
        this.mapData = null;
        this.robotPose = null;
        this.scale = 2;
        this.offsetX = 0;
        this.offsetY = 0;
        this.robotTracking = true;
        this.updateInterval = null;
        this.mapUpdateInterval = null;
        this.mapAutoUpdate = false;

        this.lastFrameTime = Date.now();
        this.fps = 0;
        this.isDragging = false;
        this.lastMouseX = 0;
        this.lastMouseY = 0;

        // Proximity warning settings
        this.closestObstacle = null;
        this.proximityThreshold = 0.5; // meters
        this.isNearObstacle = false;

        this.setupCanvas();
        this.setupInteraction();
    }

    setupCanvas() {
        // Set canvas size to match container
        const container = this.canvas.parentElement;
        this.canvas.width = container.clientWidth;
        this.canvas.height = container.clientHeight;
    }

    async loadMap() {
        const loader = document.getElementById('mapLoader');
        loader.style.display = 'block';

        try {
            const response = await fetch('/get_map_data', {
                method: 'POST'
            });
            const data = await response.json();

            if (data.error) {
                console.error('Map error:', data.error);
                return;
            }

            this.mapData = data;

            // Calculate initial scale to fit map
            const padding = 40;
            const scaleX = (this.canvas.width - padding * 2) / data.metadata.width;
            const scaleY = (this.canvas.height - padding * 2) / data.metadata.height;
            this.scale = Math.min(scaleX, scaleY);

            this.drawMap();
            this.startRobotUpdate();
            this.setMapAutoUpdate(true);

            loader.style.display = 'none';

        } catch (error) {
            console.error('Failed to load map:', error);
            loader.style.display = 'none';
        }
    }

    setMapAutoUpdate(enabled, periodMs = 3000) {
        this.mapAutoUpdate = enabled;

        if (this.mapUpdateInterval) {
            clearInterval(this.mapUpdateInterval);
            this.mapUpdateInterval = null;
        }

        if (enabled) {
            this.mapUpdateInterval = setInterval(() => {
                this.reloadMapData();
            }, periodMs);
        }
    }

    async reloadMapData() {
        try {
            const response = await fetch('/get_map_data', {
                method: 'POST'
            });
            const data = await response.json();

            if (!data.error) {
                this.mapData = data;
                this.drawMap();
            }
        } catch (error) {
            console.error('Failed to reload map:', error);
        }
    }


    drawMap() {
        if (!this.mapData) return;

        const ctx = this.ctx;
        const meta = this.mapData.metadata;

        // Clear canvas
        ctx.fillStyle = 'white';
        ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);

        ctx.save();
        ctx.translate(this.offsetX, this.offsetY);

        // Calculate center offset to center the map
        const mapWidth = meta.width * this.scale;
        const mapHeight = meta.height * this.scale;
        const centerX = (this.canvas.width - mapWidth) / 2;
        const centerY = (this.canvas.height - mapHeight) / 2;

        ctx.translate(centerX, centerY);

        // Draw grid
        this.drawGrid(meta);

        // Draw free space (white)
        ctx.fillStyle = 'white';
        for (let i = 0; i < this.mapData.free.x.length; i++) {
            const x = this.mapData.free.x[i] * this.scale;
            const y = (meta.height - this.mapData.free.y[i]) * this.scale;
            ctx.fillRect(x, y, this.scale, this.scale);
        }

        // Draw occupied space (dark gray)
        ctx.fillStyle = '#2c3e50';
        for (let i = 0; i < this.mapData.occupied.x.length; i++) {
            const x = this.mapData.occupied.x[i] * this.scale;
            const y = (meta.height - this.mapData.occupied.y[i]) * this.scale;
            ctx.fillRect(x, y, this.scale, this.scale);
        }

        // Draw origin
        this.drawOrigin(meta);

        // Draw robot
        if (this.robotPose) {
            this.drawRobot(meta);
        }

        // Draw proximity warning at obstacle
        if (this.isNearObstacle && this.closestObstacle) {
            this.drawProximityWarningAtObstacle(meta);
        }

        ctx.restore();

        // Draw legend
        this.drawLegend();

        this.updateStats();
    }

    drawProximityWarningAtObstacle(meta) {
        const ctx = this.ctx;
        const obs = this.closestObstacle;

        if (!obs) return;

        // Convert obstacle world (meters) -> map pixels
        const obsPx = ((obs.x - meta.origin.x) / meta.resolution) * this.scale;
        const obsPy = (meta.height - ((obs.y - meta.origin.y) / meta.resolution)) * this.scale;

        ctx.save();
        ctx.translate(obsPx, obsPy);

        // Size scales a bit with zoom, but stays compact
        const baseSize = 9 * Math.max(1.0, this.scale / 1.5);
        const triSize = baseSize;
        const triHeight = triSize * 0.866; // equilateral triangle

        // --- Glow / shadow ---
        ctx.shadowColor = 'rgba(255, 140, 0, 0.75)';
        ctx.shadowBlur = 12;
        ctx.shadowOffsetX = 0;
        ctx.shadowOffsetY = 0;

        // --- Outer triangle with gradient fill ---
        const grad = ctx.createLinearGradient(0, -triHeight * 0.5, 0, triHeight * 0.5);
        grad.addColorStop(0, '#fff7c2');  // light at the top
        grad.addColorStop(0.5, '#ffcc4d');
        grad.addColorStop(1, '#ff9800');  // deeper at the bottom

        ctx.beginPath();
        ctx.moveTo(0, -triHeight * 0.6);          // top
        ctx.lineTo(-triSize * 0.55, triHeight * 0.55); // bottom-left
        ctx.lineTo(triSize * 0.55, triHeight * 0.55);  // bottom-right
        ctx.closePath();

        ctx.fillStyle = grad;
        ctx.strokeStyle = '#b35a00';
        ctx.lineWidth = 1.5;
        ctx.fill();
        ctx.stroke();

        // Reset shadow for sharp icon
        ctx.shadowBlur = 0;

        // --- Inner triangle to give a “bordered” look ---
        ctx.beginPath();
        ctx.moveTo(0, -triHeight * 0.45);
        ctx.lineTo(-triSize * 0.42, triHeight * 0.45);
        ctx.lineTo(triSize * 0.42, triHeight * 0.45);
        ctx.closePath();

        ctx.fillStyle = 'rgba(255, 255, 255, 0.35)';
        ctx.fill();

        // --- Exclamation mark (icon-like, centered) ---
        ctx.fillStyle = '#3b1900';
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';

        // Use separate font sizes so it looks balanced
        const markSize = baseSize * 0.95;
        ctx.font = `${markSize}px Arial`;
        ctx.fillText('!', 0, triHeight * 0.05);

        ctx.restore();
    }

    drawGrid(meta) {
        const ctx = this.ctx;
        ctx.strokeStyle = 'darkgray';
        ctx.lineWidth = 1;

        const numTiles = 12;
        const tileWidth = (meta.width / numTiles) * this.scale;
        const tileHeight = (meta.height / numTiles) * this.scale;

        for (let i = 0; i <= numTiles; i++) {
            const x = i * tileWidth;
            const y = i * tileHeight;

            ctx.beginPath();
            ctx.moveTo(x, 0);
            ctx.lineTo(x, meta.height * this.scale);
            ctx.stroke();

            ctx.beginPath();
            ctx.moveTo(0, y);
            ctx.lineTo(meta.width * this.scale, y);
            ctx.stroke();
        }
    }

    drawOrigin(meta) {
        const ctx = this.ctx;
        const originPx = Math.abs(meta.origin.x / meta.resolution) * this.scale;
        const originPy = (meta.height - Math.abs(meta.origin.y / meta.resolution)) * this.scale;

        const axisLength = 20;

        // X-axis (red)
        ctx.strokeStyle = '#e74c3c';
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.moveTo(originPx, originPy);
        ctx.lineTo(originPx + axisLength, originPy);
        ctx.stroke();

        // Y-axis (green)
        ctx.strokeStyle = '#2ecc71';
        ctx.beginPath();
        ctx.moveTo(originPx, originPy);
        ctx.lineTo(originPx, originPy - axisLength);
        ctx.stroke();

        // Label
        ctx.fillStyle = '#2ecc71';
        ctx.font = 'bold 11px Arial';
        ctx.fillText('Origin', originPx + 5, originPy + 15);
    }

    drawRobot(meta) {
        const ctx = this.ctx;
        const robotPx = ((this.robotPose.x - meta.origin.x) / meta.resolution) * this.scale;
        const robotPy = (meta.height - (this.robotPose.y - meta.origin.y) / meta.resolution) * this.scale;

        // Check proximity to obstacles
        this.checkProximity(meta);

        // Auto-center on robot if tracking is enabled
        if (this.robotTracking) {
            const mapWidth = meta.width * this.scale;
            const mapHeight = meta.height * this.scale;
            const centerX = (this.canvas.width - mapWidth) / 2;
            const centerY = (this.canvas.height - mapHeight) / 2;

            // Calculate offset to center robot in viewport
            this.offsetX = this.canvas.width / 2 - robotPx - centerX;
            this.offsetY = this.canvas.height / 2 - robotPy - centerY;
        }

        ctx.save();
        ctx.translate(robotPx, robotPy);
        ctx.rotate(-this.robotPose.theta);

        // Robot size scales better when zoomed out
        const size = 10 * Math.max(1.2, this.scale / 1.0);

        // --- Main robot body ---
        ctx.fillStyle = '#000000da';
        ctx.strokeStyle = '#3d4a56';
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.roundRect(-size * 0.5, -size * 0.5, size, size * 1.0, size * 0.15);
        ctx.fill();
        ctx.stroke();

        // Center mounting plate for LiDAR
        ctx.fillStyle = '#4a5866';
        ctx.strokeStyle = '#2c3e50';
        ctx.lineWidth = 1.5;
        ctx.beginPath();
        ctx.arc(0, 0, size * 0.25, 0, Math.PI * 2);
        ctx.fill();
        ctx.stroke();

        // LiDAR unit (cylindrical housing)
        ctx.fillStyle = '#2c3e50';
        ctx.strokeStyle = '#1a252f';
        ctx.lineWidth = 1.5;
        ctx.beginPath();
        ctx.arc(0, 0, size * 0.18, 0, Math.PI * 2);
        ctx.fill();
        ctx.stroke();

        // LiDAR lens/window
        ctx.fillStyle = '#1a1a2e';
        ctx.beginPath();
        ctx.arc(0, 0, size * 0.12, 0, Math.PI * 2);
        ctx.fill();

        // LiDAR reflective surface
        const gradient = ctx.createRadialGradient(0, -size * 0.03, 0, 0, 0, size * 0.12);
        gradient.addColorStop(0, 'rgba(100, 200, 255, 0.6)');
        gradient.addColorStop(0.6, 'rgba(50, 150, 255, 0.3)');
        gradient.addColorStop(1, 'rgba(20, 80, 150, 0.1)');
        ctx.fillStyle = gradient;
        ctx.beginPath();
        ctx.arc(0, 0, size * 0.12, 0, Math.PI * 2);
        ctx.fill();

        // --- Wheels/Tires (4 total) ---
        const tireWidth = size * 0.35;
        const tireHeight = size * 0.15;

        // Front left tire
        ctx.fillStyle = '#1a1a1a';
        ctx.strokeStyle = '#333333';
        ctx.lineWidth = 1.5;
        ctx.beginPath();
        ctx.roundRect(-size * 0.40, -size * 0.60, tireWidth, tireHeight, size * 0.1);
        ctx.fill();
        ctx.stroke();

        // Front right tire
        ctx.beginPath();
        ctx.roundRect(size * 0.10, -size * 0.60, tireWidth, tireHeight, size * 0.1);
        ctx.fill();
        ctx.stroke();

        // Rear left tire
        ctx.beginPath();
        ctx.roundRect(-size * 0.40, size * 0.38, tireWidth, tireHeight, size * 0.1);
        ctx.fill();
        ctx.stroke();

        // Rear right tire
        ctx.beginPath();
        ctx.roundRect(size * 0.10, size * 0.38, tireWidth, tireHeight, size * 0.1);
        ctx.fill();
        ctx.stroke();

        // Tire treads for all 4 tires
        ctx.strokeStyle = '#444444';
        ctx.lineWidth = 1.2;
        const tirePositions = [
            { x: -size * 0.40, y: -size * 0.60 },  // Front left
            { x: size * 0.10, y: -size * 0.60 },   // Front right
            { x: -size * 0.40, y: size * 0.38 },   // Rear left
            { x: size * 0.10, y: size * 0.38 }     // Rear right
        ];

        for (const pos of tirePositions) {
            for (let i = 0; i < 4; i++) {
                const x = pos.x + (i + 0.5) * (tireWidth / 4);
                ctx.beginPath();
                ctx.moveTo(x, pos.y + 2);
                ctx.lineTo(x, pos.y + tireHeight - 2);
                ctx.stroke();
            }
        }

        // Front headlights (thin white rectangles)
        ctx.fillStyle = '#ffffff';
        ctx.strokeStyle = '#cccccc';
        ctx.lineWidth = 0.5;

        // Left headlight
        ctx.beginPath();
        ctx.roundRect(size * 0.48, -size * 0.35, size * 0.08, size * 0.25, size * 0.02);
        ctx.fill();
        ctx.stroke();

        // Right headlight
        ctx.beginPath();
        ctx.roundRect(size * 0.48, size * 0.10, size * 0.08, size * 0.25, size * 0.02);
        ctx.fill();
        ctx.stroke();

        // Rear lights (thin red rectangles)
        ctx.fillStyle = '#ff0000';
        ctx.strokeStyle = '#cc0000';
        ctx.lineWidth = 0.5;

        // Left rear light
        ctx.beginPath();
        ctx.roundRect(-size * 0.56, -size * 0.35, size * 0.08, size * 0.25, size * 0.02);
        ctx.fill();
        ctx.stroke();

        // Right rear light
        ctx.beginPath();
        ctx.roundRect(-size * 0.56, size * 0.10, size * 0.08, size * 0.25, size * 0.02);
        ctx.fill();
        ctx.stroke();


        // LiDAR scan cone (improved visualization)
        ctx.strokeStyle = 'rgba(46, 204, 113, 0.3)';
        ctx.fillStyle = 'rgba(46, 204, 113, 0.2)';
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.moveTo(0, 0);
        ctx.arc(0, 0, size * 1.8, -Math.PI / 4, Math.PI / 4);
        ctx.closePath();
        ctx.fill();
        ctx.stroke();

        ctx.restore();
    }

    checkProximity(meta) {
        if (!this.robotPose || !this.mapData) {
            this.isNearObstacle = false;
            this.closestObstacle = null;
            return;
        }

        const robotX = this.robotPose.x;
        const robotY = this.robotPose.y;

        this.isNearObstacle = false;
        this.closestObstacle = null;

        let minDist = Infinity;
        let closest = null;

        for (let i = 0; i < this.mapData.occupied.x.length; i++) {
            const occX = this.mapData.occupied.x[i] * meta.resolution + meta.origin.x;
            const occY = this.mapData.occupied.y[i] * meta.resolution + meta.origin.y;

            const dx = robotX - occX;
            const dy = robotY - occY;
            const distance = Math.sqrt(dx * dx + dy * dy);

            if (distance <= this.proximityThreshold && distance < minDist) {
                minDist = distance;
                closest = { x: occX, y: occY }; // world coordinates of obstacle
            }
        }

        if (closest) {
            this.isNearObstacle = true;
            this.closestObstacle = closest;
        }
    }

    drawLegend() {
        const ctx = this.ctx;
        const padding = 10;
        const itemHeight = 25;
        const swatchSize = 18;
        const legendX = padding;
        const legendY = padding;

        // Legend items (extensible array)
        const items = [
            { label: 'Free Space', color: 'white', strokeColor: '#cccccc' },
            { label: 'Occupied', color: '#2c3e50', strokeColor: '#2c3e50' },
            { label: 'Origin', color: '#2ecc71', strokeColor: '#2ecc71' },
            { label: 'Robot', color: '#000000da', strokeColor: '#3d4a56' }
        ];

        // Calculate legend background size
        ctx.font = '13px Arial';
        const maxTextWidth = Math.max(...items.map(item => ctx.measureText(item.label).width));
        const legendWidth = swatchSize + 10 + maxTextWidth + padding * 2;
        const legendHeight = items.length * itemHeight + padding * 2;

        // Draw legend background
        ctx.fillStyle = 'rgba(255, 255, 255, 0.95)';
        ctx.strokeStyle = '#cccccc';
        ctx.lineWidth = 1;
        ctx.fillRect(legendX, legendY, legendWidth, legendHeight);
        ctx.strokeRect(legendX, legendY, legendWidth, legendHeight);

        // Draw legend items
        items.forEach((item, index) => {
            const y = legendY + padding + index * itemHeight;

            // Draw color swatch
            ctx.fillStyle = item.color;
            ctx.strokeStyle = item.strokeColor;
            ctx.lineWidth = 1.5;
            ctx.fillRect(legendX + padding, y, swatchSize, swatchSize);
            ctx.strokeRect(legendX + padding, y, swatchSize, swatchSize);

            // Draw label
            ctx.fillStyle = '#333333';
            ctx.font = '13px Arial';
            ctx.textAlign = 'left';
            ctx.textBaseline = 'top';
            ctx.fillText(item.label, legendX + padding + swatchSize + 10, y + 3);
        });
    }

    async updateRobotPose() {
        try {
            const response = await fetch('/get_robot_pose');
            const pose = await response.json();

            if (!pose.error) {
                this.robotPose = pose;
                this.drawMap();
            }
        } catch (error) {
            console.error('Robot pose update failed:', error);
        }
    }

    startRobotUpdate() {
        if (this.updateInterval) {
            clearInterval(this.updateInterval);
        }
        // Update at 10 Hz
        this.updateInterval = setInterval(() => {
            this.updateRobotPose();
        }, 100);
    }

    updateStats() {
        const now = Date.now();
        this.fps = Math.round(1000 / (now - this.lastFrameTime));
        this.lastFrameTime = now;

        document.getElementById('mapFPS').textContent = this.fps;
        document.getElementById('mapZoom').textContent = this.scale.toFixed(1) + 'x';

        if (this.robotPose) {
            document.getElementById('robotX').textContent = this.robotPose.x.toFixed(2) + 'm';
            document.getElementById('robotY').textContent = this.robotPose.y.toFixed(2) + 'm';
            document.getElementById('robotTheta').textContent =
                (this.robotPose.theta * 180 / Math.PI).toFixed(1) + '°';
        }
    }

    toggleRobotTracking() {
        this.robotTracking = !this.robotTracking;
        document.getElementById('trackingText').textContent =
            this.robotTracking ? 'Tracking ON' : 'Tracking OFF';
    }

    zoomIn() {
        this.scale *= 1.2;
        this.drawMap();
    }

    zoomOut() {
        this.scale /= 1.2;
        this.drawMap();
    }

    resetView() {
        this.offsetX = 0;
        this.offsetY = 0;
        if (this.mapData) {
            const padding = 40;
            const scaleX = (this.canvas.width - padding * 2) / this.mapData.metadata.width;
            const scaleY = (this.canvas.height - padding * 2) / this.mapData.metadata.height;
            this.scale = Math.min(scaleX, scaleY);
        }
        this.drawMap();
    }

    setupInteraction() {
        this.canvas.addEventListener('mousedown', (e) => {
            this.isDragging = true;
            this.lastMouseX = e.clientX;
            this.lastMouseY = e.clientY;
            // Disable tracking when user manually pans
            if (this.robotTracking) {
                this.toggleRobotTracking();
            }
        });

        this.canvas.addEventListener('mousemove', (e) => {
            if (this.isDragging) {
                this.offsetX += e.clientX - this.lastMouseX;
                this.offsetY += e.clientY - this.lastMouseY;
                this.lastMouseX = e.clientX;
                this.lastMouseY = e.clientY;
                this.drawMap();
            }
        });

        this.canvas.addEventListener('mouseup', () => {
            this.isDragging = false;
        });

        this.canvas.addEventListener('mouseleave', () => {
            this.isDragging = false;
        });

        this.canvas.addEventListener('wheel', (e) => {
            e.preventDefault();
            // Disable tracking when user manually zooms
            if (this.robotTracking) {
                this.toggleRobotTracking();
            }
            if (e.deltaY < 0) {
                this.zoomIn();
            } else {
                this.zoomOut();
            }
        });
    }
}

// Initialize map viewer
let robotMapViewer;
window.addEventListener('load', () => {
    robotMapViewer = new RobotMapViewer('mapCanvas');

    // Auto-load map after 1 second
    setTimeout(() => {
        robotMapViewer.loadMap();
    }, 1000);

    const autoMapCheckbox = document.getElementById('autoMapUpdate');
    autoMapCheckbox.addEventListener('change', (e) => {
        robotMapViewer.setMapAutoUpdate(e.target.checked, 3000);
    });
});
