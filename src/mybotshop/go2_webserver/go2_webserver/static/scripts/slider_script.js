// Function to update slider values and send data to the backend
// Exposed globally for reconnection re-sync (safety feature)
window.sendSliderData = function sendSliderData() {
    const linearVelocity = document.getElementById('slider1').value;
    const angularVelocity = document.getElementById('slider2').value;

    // Update the displayed values with units
    document.getElementById('slider1-value').textContent = `${linearVelocity / 100} m/s`;
    document.getElementById('slider2-value').textContent = `${angularVelocity / 100} rad/s`;

    // Send the data to the Python backend
    fetch('/slider_control', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            linear_velocity: linearVelocity,
            angular_velocity: angularVelocity,
        }),
    })
        .then(response => response.json())
        .then(data => {
            console.log('Slider Data Sent Successfully:', data);
        })
        .catch((error) => {
            console.error('Error:', error);
        });
}

function postRig(url, value) {
    return fetch(url, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ value: Number(value) }),
    })
        .then(r => r.json())
        .then(data => console.log(`${url} ok:`, data))
        .catch(err => console.error(`${url} error:`, err));
}

// Individual handlers
function onRigBaseInput(e) {
    const v = e.target.value;
    document.getElementById('VSlider1-value').textContent = `${v}mm`;
    postRig('/rig_control/1', v);
}

function onRigLeftInput(e) {
    const v = e.target.value;
    document.getElementById('VSlider2-value').textContent = `${v}mm`;
    postRig('/rig_control/2', v);
}

function onRigRightInput(e) {
    const v = e.target.value;
    document.getElementById('VSlider3-value').textContent = `${v}mm`;
    postRig('/rig_control/3', v);
}

// Rig sliders
document.getElementById('VSlider1').addEventListener('input', onRigBaseInput);
document.getElementById('VSlider2').addEventListener('input', onRigLeftInput);
document.getElementById('VSlider3').addEventListener('input', onRigRightInput);

// Twistmux sliders
document.getElementById('slider1').addEventListener('input', window.sendSliderData);
document.getElementById('slider2').addEventListener('input', window.sendSliderData);

// Send initial slider values on page load
document.addEventListener('DOMContentLoaded', window.sendSliderData);