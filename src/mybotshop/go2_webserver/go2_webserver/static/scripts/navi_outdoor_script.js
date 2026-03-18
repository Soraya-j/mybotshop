/*
Software License Agreement (BSD)

@author    Salman Omar Sohail <support@mybotshop.de>
@copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.
*/

// Default coordinates (NRW, Germany)
var defaultLat = 50.95359;
var defaultLon = 6.60174;
var defaultZoom = 16;

var coordinatesList = [];
var markers = [];
var lines = [];
var robotMarker;
var map;
var polylineDecoratorReady = false;
var arrowAnimationInterval;
var arrowPattern;

// Waypoint tool state
var waypointToolActive = false;

// Drag-to-heading state
var waypointDragStart = null;
var waypointDragLine = null;
var isDraggingWaypoint = false;

// No-go zones
var noGoZones = [];
var noGoPolygons = [];
var currentNoGoPoints = [];
var isCreatingNoGoZone = false;
var noGoTempMarkers = [];
var noGoTempLines = [];

let tileLayers;
let currentTileLayer;
var robotPositionIntervalId = null;

// Ensure map container has valid size
var mapContainer = document.getElementById("map");
if (!mapContainer) {
    console.error("Map container is missing");
}

// === Marker icon helpers ===

// Robot: SVG vector icon indicating heading (matches indoor navigation style)
function createRobotIcon(headingDeg) {
    var angle = isNaN(headingDeg) ? 0 : Number(headingDeg);

    // SVG robot matching indoor navigation vector design
    var robotSVG = `
        <svg width="48" height="48" viewBox="-24 -24 48 48" xmlns="http://www.w3.org/2000/svg">
            <!-- LiDAR scan cone -->
            <path d="M0,0 L14,-14 A20,20 0 0,1 14,14 Z" fill="rgba(46, 204, 113, 0.2)" stroke="rgba(46, 204, 113, 0.3)" stroke-width="1"/>

            <!-- Front left tire -->
            <rect x="-8" y="-12" width="7" height="3" rx="1" fill="#1a1a1a" stroke="#333333" stroke-width="0.5"/>
            <!-- Front right tire -->
            <rect x="1" y="-12" width="7" height="3" rx="1" fill="#1a1a1a" stroke="#333333" stroke-width="0.5"/>
            <!-- Rear left tire -->
            <rect x="-8" y="9" width="7" height="3" rx="1" fill="#1a1a1a" stroke="#333333" stroke-width="0.5"/>
            <!-- Rear right tire -->
            <rect x="1" y="9" width="7" height="3" rx="1" fill="#1a1a1a" stroke="#333333" stroke-width="0.5"/>

            <!-- Main robot body -->
            <rect x="-10" y="-10" width="20" height="20" rx="3" fill="#000000da" stroke="#3d4a56" stroke-width="1.5"/>

            <!-- Center mounting plate -->
            <circle cx="0" cy="0" r="5" fill="#4a5866" stroke="#2c3e50" stroke-width="1"/>

            <!-- LiDAR unit -->
            <circle cx="0" cy="0" r="3.5" fill="#2c3e50" stroke="#1a252f" stroke-width="1"/>

            <!-- LiDAR lens -->
            <circle cx="0" cy="0" r="2.5" fill="#1a1a2e"/>

            <!-- LiDAR reflective highlight -->
            <circle cx="0" cy="-0.5" r="2" fill="rgba(100, 200, 255, 0.4)"/>

            <!-- Front headlights -->
            <rect x="10" y="-7" width="2" height="5" rx="0.5" fill="#ffffff" stroke="#cccccc" stroke-width="0.3"/>
            <rect x="10" y="2" width="2" height="5" rx="0.5" fill="#ffffff" stroke="#cccccc" stroke-width="0.3"/>

            <!-- Rear lights -->
            <rect x="-12" y="-7" width="2" height="5" rx="0.5" fill="#ff0000" stroke="#cc0000" stroke-width="0.3"/>
            <rect x="-12" y="2" width="2" height="5" rx="0.5" fill="#ff0000" stroke="#cc0000" stroke-width="0.3"/>
        </svg>
    `;

    return L.divIcon({
        className: 'robot-icon',
        html: '<div class="robot-svg" style="transform: translate(-50%, -50%) rotate(' + angle + 'deg);">' + robotSVG + '</div>',
        iconSize: [48, 48],
        iconAnchor: [24, 24]
    });
}

// Waypoint: filled circle + FA arrow indicating heading
function createWaypointIcon(headingDeg) {
    var angle = isNaN(headingDeg) ? 0 : Number(headingDeg);
    return L.divIcon({
        className: 'waypoint-icon',
        html:
            '<div class="wp-circle"></div>' +
            '<i class="fas fa-long-arrow-alt-up wp-arrow" style="transform: translate(-50%, -50%) rotate(' + angle + 'deg);"></i>',
        iconSize: [24, 24],
        iconAnchor: [12, 12]
    });
}

// === Map UI helpers ===

function switchMapType(mapType) {
    if (!map) return;

    if (currentTileLayer) {
        map.removeLayer(currentTileLayer);
    }

    if (tileLayers[mapType]) {
        currentTileLayer = tileLayers[mapType];
        currentTileLayer.addTo(map);
    }
}

// Toggle waypoint tool activation
function toggleWaypointTool() {
    waypointToolActive = !waypointToolActive;
    const button = document.getElementById('waypointToolToggle');
    if (!button) return;

    if (waypointToolActive) {
        button.innerHTML = '<i class="fas fa-times-circle"></i> Deactivate';
        button.classList.add('deactivate-btn');
        updateResponseMessage('Waypoint tool activated - Click and drag on map to add waypoints with heading');
    } else {
        button.innerHTML = '<i class="fas fa-plus-circle"></i> Activate Tool';
        button.classList.remove('deactivate-btn');
        updateResponseMessage('Waypoint tool deactivated');
    }
}

// Toggle no-go zone creation
function toggleNoGoZoneCreation() {
    isCreatingNoGoZone = !isCreatingNoGoZone;
    const button = document.getElementById('noGoZoneToggle');

    if (button) {
        if (isCreatingNoGoZone) {
            button.innerHTML = '<i class="fas fa-check-circle"></i> Finish Zone';
            button.classList.add('deactivate-btn');
            currentNoGoPoints = [];
            updateResponseMessage('No-go zone creation active - Click on map to add polygon points');
        } else {
            button.innerHTML = '<i class="fas fa-draw-polygon"></i> Create Zone';
            button.classList.remove('deactivate-btn');

            if (currentNoGoPoints.length >= 3) {
                createNoGoZone();
            }
            updateResponseMessage('No-go zone creation finished');
        }
    }
}

// Create no-go zone from current points
function createNoGoZone() {
    if (currentNoGoPoints.length < 3) {
        alert('A no-go zone needs at least 3 points to form a polygon');
        return;
    }

    const pointsCount = currentNoGoPoints.length;

    const polygon = L.polygon(currentNoGoPoints, {
        color: 'red',
        fillColor: 'red',
        fillOpacity: 0.3,
        weight: 2
    }).addTo(map);

    noGoZones.push({
        id: Date.now(),
        coordinates: currentNoGoPoints.map(point => ({ lat: point.lat, lng: point.lng }))
    });

    noGoPolygons.push(polygon);

    currentNoGoPoints = [];

    updateResponseMessage(`No-go zone created with ${pointsCount} points`);
    updateStats();
}

// Save no-go zones to backend
function saveNoGoZones() {
    if (noGoZones.length === 0) {
        alert('No no-go zones to save');
        return;
    }

    fetch('/save_no_go_zones', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(noGoZones)
    })
        .then(response => response.text())
        .then(msg => {
            alert("No-go zones saved successfully!");
            updateResponseMessage('No-go zones saved successfully');
        })
        .catch(err => {
            console.error("Failed to save no-go zones", err);
            updateResponseMessage('Failed to save no-go zones');
        });
}

// Load no-go zones from backend
function loadNoGoZones() {
    fetch('/load_no_go_zones')
        .then(response => response.json())
        .then(zones => {
            clearNoGoZones();
            zones.forEach(zone => {
                const coords = zone.coordinates.map(coord => L.latLng(coord.lat, coord.lng));
                const polygon = L.polygon(coords, {
                    color: 'red',
                    fillColor: 'red',
                    fillOpacity: 0.3,
                    weight: 2
                }).addTo(map);

                noGoZones.push(zone);
                noGoPolygons.push(polygon);
            });
            updateResponseMessage(`Loaded ${zones.length} no-go zones`);
            updateStats();
        })
        .catch(err => {
            console.error("Failed to load no-go zones", err);
            updateResponseMessage('Failed to load no-go zones');
        });
}

// Clear all no-go zones
function clearNoGoZones() {
    noGoPolygons.forEach(polygon => map.removeLayer(polygon));
    noGoTempMarkers.forEach(marker => map.removeLayer(marker));
    noGoTempLines.forEach(line => map.removeLayer(line));

    noGoZones = [];
    noGoPolygons = [];
    currentNoGoPoints = [];
    noGoTempMarkers = [];
    noGoTempLines = [];

    updateResponseMessage('No-go zones cleared');
    updateStats();
}

// Update response message
function updateResponseMessage(message) {
    const responseElement = document.getElementById('responseMessage');
    if (responseElement) {
        responseElement.textContent = message;
    }
}

// Add point to current no-go zone
function addNoGoZonePoint(latlng) {
    currentNoGoPoints.push(latlng);

    const marker = L.circleMarker(latlng, {
        radius: 5,
        weight: 1,
        color: 'red',
        fillColor: 'red',
        fillOpacity: 0.4
    }).addTo(map);

    noGoTempMarkers.push(marker);

    if (currentNoGoPoints.length > 1) {
        const tempLine = L.polyline(currentNoGoPoints, {
            color: 'red',
            weight: 2,
            dashArray: '5, 5'
        }).addTo(map);
        noGoTempLines.push(tempLine);
    }

    updateResponseMessage(`Added point ${currentNoGoPoints.length} to no-go zone`);
}

// === Robot position & marker ===

// Track current heading for smooth rotation
var currentRobotHeading = 0;

function updateRobotPosition(lat, lon, headingDeg) {
    if (!map) return;

    if (!isNaN(lat) && !isNaN(lon)) {
        const position = [lat, lon];
        const angle = isNaN(headingDeg) ? 0 : Number(headingDeg);

        if (robotMarker) {
            // Update position (CSS transition handles smooth movement)
            robotMarker.setLatLng(position);

            // Update rotation by modifying existing SVG transform (faster than recreating icon)
            const svgElement = robotMarker.getElement()?.querySelector('.robot-svg');
            if (svgElement) {
                svgElement.style.transform = `translate(-50%, -50%) rotate(${angle}deg)`;
            }
            currentRobotHeading = angle;
        } else {
            // First time - create the marker
            const icon = createRobotIcon(headingDeg);
            robotMarker = L.marker(position, { icon: icon }).addTo(map);
            currentRobotHeading = angle;
        }
    }
}

function fetchRobotPosition() {
    if (!map) return;

    const gpsStatusEl = document.getElementById('gpsStatus');

    fetch('/update_gps_position', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ lat: null, lon: null })
    })
        .then(response => response.json())
        .then(data => {
            const { lat, lon, heading } = data;
            if (lat != null && lon != null) {
                const numericHeading = isNaN(heading) ? 0 : Number(heading);
                updateRobotPosition(lat, lon, numericHeading);
                if (gpsStatusEl) {
                    gpsStatusEl.textContent = 'Active';
                    gpsStatusEl.style.color = 'var(--project-color)';
                }
            } else {
                if (gpsStatusEl) {
                    gpsStatusEl.textContent = 'No Fix';
                    gpsStatusEl.style.color = '#ff6b6b';
                }
            }
        })
        .catch(error => {
            console.error("Error fetching robot position:", error);
            if (gpsStatusEl) {
                gpsStatusEl.textContent = 'Error';
                gpsStatusEl.style.color = '#ff6b6b';
            }
        });
}

// === Waypoints (with heading) ===

function addWaypointWithHeading(latlng, headingDeg) {
    if (!waypointToolActive) return;

    const icon = createWaypointIcon(headingDeg || 0);

    var marker = L.marker(latlng, {
        icon: icon,
        draggable: false
    }).addTo(map);

    marker._headingDeg = headingDeg || 0;

    markers.push(marker);
    updateCoordinatesList();
    updateResponseMessage(`Added waypoint ${markers.length} (heading: ${marker._headingDeg.toFixed(1)}°)`);
}

// Save waypoints (send to backend)
function saveWaypoints() {
    if (coordinatesList.length === 0) {
        alert('No waypoints to save');
        return;
    }

    const coords = coordinatesList.map(p => ({
        lat: p.lat,
        lon: p.lng,
        heading: p.heading || 0
    }));

    fetch('/save_waypoints', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(coords)
    })
        .then(response => response.text())
        .then(msg => {
            alert("Waypoints saved successfully!");
            updateResponseMessage('Waypoints saved successfully');
        })
        .catch(err => {
            console.error("Failed to save waypoints", err);
            updateResponseMessage('Failed to save waypoints');
        });
}

// Load waypoints (request from backend)
function loadWaypoints() {
    fetch('/load_waypoints')
        .then(response => response.json())
        .then(waypoints => {
            clearWaypoints();
            waypoints.forEach(coord => {
                const latlng = [coord.lat, coord.lon];
                const heading = coord.heading || 0;

                const icon = createWaypointIcon(heading);

                const marker = L.marker(latlng, {
                    icon: icon,
                    draggable: false
                }).addTo(map);

                marker._headingDeg = heading;

                markers.push(marker);
            });
            updateCoordinatesList();
            updateResponseMessage(`Loaded ${waypoints.length} waypoints`);
        })
        .catch(err => {
            console.error("Failed to load waypoints", err);
            updateResponseMessage('Failed to load waypoints');
        });
}

// Update stats display
function updateStats() {
    const waypointCountEl = document.getElementById('waypointCount');
    const noGoCountEl = document.getElementById('noGoCount');

    if (waypointCountEl) {
        waypointCountEl.textContent = markers.length;
    }
    if (noGoCountEl) {
        noGoCountEl.textContent = noGoZones.length;
    }
}

// Maintain coordinates list and draw lines
function updateCoordinatesList() {
    coordinatesList = markers.map(marker => {
        const ll = marker.getLatLng();
        const heading = marker._headingDeg || 0;
        return {
            lat: ll.lat,
            lng: ll.lng,
            heading: heading
        };
    });
    drawLine();
    updateStats();
}

// Draw lines between waypoints
function drawLine() {
    if (!map) {
        console.error("Map is not initialized yet.");
        return;
    }

    lines.forEach(item => map.removeLayer(item));
    lines = [];

    if (arrowAnimationInterval) {
        clearInterval(arrowAnimationInterval);
        arrowAnimationInterval = null;
    }

    if (coordinatesList.length > 1) {
        const latlngs = coordinatesList.map(p => L.latLng(p.lat, p.lng));

        const polyline = L.polyline(latlngs, {
            color: 'var(--project-color)',
            weight: 5,
            opacity: 0.3
        }).addTo(map);
        lines.push(polyline);

        if (polylineDecoratorReady && typeof L.Symbol !== 'undefined' && typeof L.Symbol.arrowHead === 'function') {
            let offsetPx = 0;
            const repeatPx = 30;

            const decorator = L.polylineDecorator(polyline, {
                patterns: [
                    {
                        offset: offsetPx + 'px',
                        repeat: repeatPx + 'px',
                        symbol: L.Symbol.arrowHead({
                            pixelSize: 10,
                            polygon: false,
                            pathOptions: { stroke: true, color: 'var(--project-color)', weight: 2 }
                        })
                    }
                ]
            }).addTo(map);

            lines.push(decorator);

            arrowAnimationInterval = setInterval(() => {
                offsetPx = (offsetPx + 1) % repeatPx;
                decorator.setPatterns([
                    {
                        offset: offsetPx + 'px',
                        repeat: repeatPx + 'px',
                        symbol: L.Symbol.arrowHead({
                            pixelSize: 10,
                            polygon: false,
                            pathOptions: { stroke: true, color: 'var(--project-color)', weight: 2 }
                        })
                    }
                ]);
            }, 60);
        } else {
            console.warn("Arrow heads skipped: plugin not ready.");
        }
    }
}

// Clear waypoints
function clearWaypoints() {
    // Remove waypoint markers and route lines
    markers.forEach(marker => map.removeLayer(marker));
    lines.forEach(line => map.removeLayer(line));

    if (waypointDragLine) {
        map.removeLayer(waypointDragLine);
        waypointDragLine = null;
    }
    waypointDragStart = null;
    isDraggingWaypoint = false;

    // Stop arrow animation
    if (arrowAnimationInterval) {
        clearInterval(arrowAnimationInterval);
        arrowAnimationInterval = null;
    }

    // Reset state
    markers = [];
    lines = [];
    coordinatesList = [];
    updateResponseMessage('Waypoints cleared');
    updateStats();
}

// Center map to robot position
function centerToRobot() {
    if (!map) {
        console.error("Map is not initialized yet.");
        updateResponseMessage('Map not initialized');
        return;
    }

    if (robotMarker) {
        const robotPos = robotMarker.getLatLng();
        map.setView(robotPos, map.getZoom());
        updateResponseMessage('Centered to robot position');
    } else {
        updateResponseMessage('Robot position not available');
    }
}

// === Bootstrapping ===

document.addEventListener("DOMContentLoaded", function () {

    // Inject CSS for custom icons
    (function injectMarkerStyles() {
        const style = document.createElement('style');
        style.textContent = `
            .robot-icon, .waypoint-icon {
                position: relative;
            }
            .robot-svg {
                position: absolute;
                top: 50%;
                left: 50%;
                width: 48px;
                height: 48px;
                transform-origin: center center;
                transition: transform 0.1s linear;
            }
            .robot-svg svg {
                display: block;
            }
            /* Smooth marker movement on map */
            .leaflet-marker-icon.robot-icon {
                transition: transform 0.1s linear;
            }
            .wp-circle {
                position: absolute;
                top: 50%;
                left: 50%;
                width: 20px;
                height: 20px;
                transform: translate(-50%, -50%);
                border-radius: 50%;
                background: var(--project-special-color, #007bff);
                box-shadow: 0 0 3px rgba(0,0,0,0.4);
            }
            .wp-arrow {
                position: absolute;
                top: 50%;
                left: 50%;
                font-size: 12px;
                color: #fff;
                transform-origin: 50% 50%;
            }
        `;
        document.head.appendChild(style);
    })();

    function loadOpenStreetMap() {
        console.log("Loading OpenStreetMap...");

        var leafletCSS = document.createElement("link");
        leafletCSS.rel = "stylesheet";
        leafletCSS.href = "https://unpkg.com/leaflet@1.7.1/dist/leaflet.css";
        document.head.appendChild(leafletCSS);

        var leafletJS = document.createElement("script");
        leafletJS.src = "https://unpkg.com/leaflet@1.7.1/dist/leaflet.js";

        leafletJS.onload = function () {
            console.log("Leaflet loaded");

            var decoratorScript = document.createElement("script");
            decoratorScript.src = "https://cdnjs.cloudflare.com/ajax/libs/leaflet-polylinedecorator/1.1.0/leaflet.polylineDecorator.min.js";

            decoratorScript.onload = function () {
                console.log("PolylineDecorator loaded");
                polylineDecoratorReady = true;
                initMap();
            };

            decoratorScript.onerror = function () {
                console.error("Failed to load PolylineDecorator");
                initMap();
            };

            document.body.appendChild(decoratorScript);
        };

        leafletJS.onerror = function () {
            console.error("Failed to load Leaflet");
            loadFallbackMap();
        };

        document.body.appendChild(leafletJS);
    }

    function loadFallbackMap() {
        console.warn("No internet! Loading fallback.");
        if (mapContainer) {
            mapContainer.innerHTML = "<p>Offline mode: No map available.</p>";
        }
    }

    function initMap() {
        console.log("OpenStreetMap loaded successfully.");
        map = L.map('map').setView([defaultLat, defaultLon], defaultZoom);

        tileLayers = {
            // Google Maps - highest zoom levels available
            'google-satellite': L.tileLayer('https://mt1.google.com/vt/lyrs=s&x={x}&y={y}&z={z}', {
                attribution: '&copy; Google',
                maxZoom: 22,
                maxNativeZoom: 21
            }),
            'google-hybrid': L.tileLayer('https://mt1.google.com/vt/lyrs=y&x={x}&y={y}&z={z}', {
                attribution: '&copy; Google',
                maxZoom: 22,
                maxNativeZoom: 21
            }),
            'google-roads': L.tileLayer('https://mt1.google.com/vt/lyrs=m&x={x}&y={y}&z={z}', {
                attribution: '&copy; Google',
                maxZoom: 22,
                maxNativeZoom: 21
            }),
            'google-terrain': L.tileLayer('https://mt1.google.com/vt/lyrs=p&x={x}&y={y}&z={z}', {
                attribution: '&copy; Google',
                maxZoom: 22,
                maxNativeZoom: 15
            }),
            // ESRI layers - very high zoom
            'esri-satellite': L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', {
                attribution: 'Tiles &copy; Esri',
                maxZoom: 23,
                maxNativeZoom: 19
            }),
            'esri-topo': L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/{z}/{y}/{x}', {
                attribution: 'Tiles &copy; Esri',
                maxZoom: 23,
                maxNativeZoom: 19
            }),
            'esri-street': L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer/tile/{z}/{y}/{x}', {
                attribution: 'Tiles &copy; Esri',
                maxZoom: 23,
                maxNativeZoom: 19
            }),
            // OpenStreetMap
            'street': L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                attribution: '&copy; OpenStreetMap contributors',
                maxZoom: 22,
                maxNativeZoom: 19
            }),
            // OpenTopoMap - good for outdoor navigation
            'topo': L.tileLayer('https://{s}.tile.opentopomap.org/{z}/{x}/{y}.png', {
                attribution: '&copy; OpenTopoMap',
                maxZoom: 22,
                maxNativeZoom: 17
            }),
            // CARTO dark theme
            'dark': L.tileLayer('https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png', {
                attribution: '&copy; CARTO',
                subdomains: 'abcd',
                maxZoom: 22,
                maxNativeZoom: 20
            })
        };

        currentTileLayer = tileLayers['street'];
        currentTileLayer.addTo(map);

        const coordsDisplay = L.control({ position: 'bottomleft' });

        coordsDisplay.onAdd = function (map) {
            this._div = L.DomUtil.create('div', 'coords-display');
            this.update();
            return this._div;
        };

        coordsDisplay.update = function (latlng) {
            this._div.innerHTML = latlng
                ? `Lat: ${latlng.lat.toFixed(5)}<br>Lon: ${latlng.lng.toFixed(5)}`
                : 'Hover over map';
        };

        coordsDisplay.addTo(map);

        const centerControl = L.control({ position: 'topleft' });

        centerControl.onAdd = function (map) {
            const div = L.DomUtil.create('div', 'leaflet-control-center-robot');
            div.innerHTML = '<button onclick="centerToRobot()" title="Center to Robot"><i class="fas fa-crosshairs"></i></button>';
            L.DomEvent.disableClickPropagation(div);
            return div;
        };

        centerControl.addTo(map);

        map.on('mousemove', function (e) {
            coordsDisplay.update(e.latlng);
        });

        // Handle map clicks for no-go zones (waypoints use drag)
        map.on('click', function (e) {
            if (!waypointToolActive && isCreatingNoGoZone) {
                addNoGoZonePoint(e.latlng);
            }
        });

        // Waypoint drag gesture: mousedown → mousemove → mouseup
        map.on('mousedown', function (e) {
            if (!waypointToolActive) return;
            if (e.originalEvent.button !== 0) return;

            isDraggingWaypoint = true;
            waypointDragStart = e.latlng;

            map.dragging.disable();

            if (waypointDragLine) {
                map.removeLayer(waypointDragLine);
                waypointDragLine = null;
            }

            waypointDragLine = L.polyline([waypointDragStart, waypointDragStart], {
                color: 'var(--project-special-color)',
                weight: 2,
                dashArray: '5, 5'
            }).addTo(map);
        });

        map.on('mousemove', function (e) {
            if (!isDraggingWaypoint || !waypointDragLine || !waypointDragStart) return;
            waypointDragLine.setLatLngs([waypointDragStart, e.latlng]);
        });

        map.on('mouseup', function (e) {
            if (!isDraggingWaypoint) return;

            isDraggingWaypoint = false;
            map.dragging.enable();

            if (waypointDragLine) {
                map.removeLayer(waypointDragLine);
                waypointDragLine = null;
            }

            if (!waypointDragStart) return;

            const dragEnd = e.latlng;

            const p1 = map.latLngToLayerPoint(waypointDragStart);
            const p2 = map.latLngToLayerPoint(dragEnd);
            const dx = p2.x - p1.x;
            const dy = p2.y - p1.y;
            const dist = Math.sqrt(dx * dx + dy * dy);

            let headingDeg = 0;
            if (dist > 5) {
                const angleRad = Math.atan2(dx, -dy);
                headingDeg = (angleRad * 180 / Math.PI + 360) % 360;
            }

            addWaypointWithHeading(waypointDragStart, headingDeg);

            waypointDragStart = null;
        });

        const sidebarEl = document.querySelector('.map-sidebar');
        if (sidebarEl) {
            L.DomEvent.disableClickPropagation(sidebarEl);
        }

        // Update at 10 Hz (100ms) for smooth robot tracking - matches indoor navigation
        robotPositionIntervalId = setInterval(fetchRobotPosition, 100);
    }

    if (navigator.onLine) {
        loadOpenStreetMap();
    } else {
        loadFallbackMap();
    }

    window.addEventListener("online", () => location.reload());
    window.addEventListener("offline", loadFallbackMap);
});
