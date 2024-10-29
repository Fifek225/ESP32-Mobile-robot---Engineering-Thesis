const status_div = document.getElementById("status");
const f_led_off = document.getElementById("front_LEDs_off");
const f_led_default = document.getElementById("front_LEDs_default");
const f_led_max = document.getElementById("front_LEDs_max");
// Get references to the buttons
const upButton = document.getElementById('move_up');
const downButton = document.getElementById('move_down');
const leftButton = document.getElementById('move_left');
const rightButton = document.getElementById('move_right');

let keyPressed = {}; // Object to track the state of keys

//Debug function - checks if there is constant communication between the webpage and ESP32
function fetch_messages() {
    console.log("Fetching messages from server");
    const request = new Request("/last_message", {method: "GET"});
    fetch(request).then(
        response => response.text()
    ).then(text => {
        status_div.innerText = text;
    });
}

// Front LED control functions
function front_LEDs_off() {
    const request = new Request("/front_led_off", { method: "POST" });
    fetch(request).then(
        response => response.text()
    );
}
function front_LEDs_default() {
    const request = new Request("/front_led_default", { method: "POST" });
    fetch(request).then(
        response => response.text()
    );
}
function front_LEDs_max() {
    const request = new Request("/front_led_max", { method: "POST" });
    fetch(request).then(
        response => response.text()
    );
}

// JavaScript to handle LED button active state
document.addEventListener('DOMContentLoaded', () => {
    const ledButtons = document.querySelectorAll('#led_control button');

    // Initially set the "ON" button (middle one) as active (green)
    let activeButton = document.querySelector('#front_LEDs_default');
    activeButton.classList.add('active');

    // Add click event listener to each LED button
    ledButtons.forEach(button => {
        button.addEventListener('click', () => {
            // Remove 'active' class from the currently active button
            activeButton.classList.remove('active');

            // Add 'active' class to the clicked button and set it as the active button
            button.classList.add('active');
            activeButton = button;
        });
    });
});

f_led_off.addEventListener('click', front_LEDs_off);
f_led_default.addEventListener('click', front_LEDs_default);
f_led_max.addEventListener('click', front_LEDs_max);

// --------------------------- ROBOT MOVEMENT BUTTONS LOGIC ----------------------------------------------------------------
// Track active buttons to prevent conflicting presses
let activeVertical = null;
let activeHorizontal = null;

// Function to simulate button press and highlight with green
function pressButton(button, oppositeButton, direction, activeTracker) {
    if (activeTracker === null) {  // Only allow pressing if no opposite is active
        oppositeButton.disabled = true;  // Disable the opposite button
        button.classList.add('active');  // Highlight the pressed button with green
        console.log(`Moving ${direction}`);
        const request = new Request(`/move_${direction}, { method: "POST" }`);
        fetch(request).then(
            response => response.text()
        );
        return direction;  // Return the active direction
    }
    return activeTracker;  // Return the already active direction (no change)
}

// Function to reset the button state and enable the opposite button when key is released
function releaseButton(button, oppositeButton, direction, activeTracker) {
    if (activeTracker === direction) {  // Only reset if this is the active button
        button.classList.remove('active');  // Remove the active class to restore default color
        oppositeButton.disabled = false;  // Re-enable the opposite button
        // Stop the movement by sending a request to the server
        const request = new Request(`/stop_${direction}, { method: "POST" }`);
        fetch(request).then(
            response => response.text()
        );
        return null;  // Clear the active state
    }
    return activeTracker;  // If this button wasn't active, no change
}

// Event listener for keyboard arrows - press
document.addEventListener('keydown', function(event) {
    if (!keyPressed[event.key]) { // Check if the key is already pressed
        keyPressed[event.key] = true; // Mark the key as pressed
        switch (event.key) {
            case 'ArrowUp':
                if (!activeVertical) { // Check if another vertical direction is active
                    console.log('ArrowUp pressed');
                    activeVertical = pressButton(upButton, downButton, 'up', activeVertical);
                }
                break;
            case 'ArrowDown':
                if (!activeVertical) {
                    console.log('ArrowDown pressed');
                    activeVertical = pressButton(downButton, upButton, 'down', activeVertical);
                }
                break;
            case 'ArrowLeft':
                if (!activeHorizontal) {
                    console.log('ArrowLeft pressed');
                    activeHorizontal = pressButton(leftButton, rightButton, 'left', activeHorizontal);
                }
                break;
            case 'ArrowRight':
                if (!activeHorizontal) {
                    console.log('ArrowRight pressed');
                    activeHorizontal = pressButton(rightButton, leftButton, 'right', activeHorizontal);
                }
                break;
        }
    }
});



// Event listener for keyboard arrows - release
document.addEventListener('keyup', function(event) {
    keyPressed[event.key] = false; // Mark the key as released
    switch (event.key) {
        case 'ArrowUp':
            console.log('ArrowUp released');
            activeVertical = releaseButton(upButton, downButton, 'up', activeVertical);
            break;
        case 'ArrowDown':
            console.log('ArrowDown released');
            activeVertical = releaseButton(downButton, upButton, 'down', activeVertical);
            break;
        case 'ArrowLeft':
            console.log('ArrowLeft released');
            activeHorizontal = releaseButton(leftButton, rightButton, 'left', activeHorizontal);
            break;
        case 'ArrowRight':
            console.log('ArrowRight released');
            activeHorizontal = releaseButton(rightButton, leftButton, 'right', activeHorizontal);
            break;
    }
});

// ======================= CAMERA IMAGE HANDLING ===============================
var camera_img = document.getElementById('camera_img');
var canvas = document.getElementById("canvas");
var context = canvas.getContext("2d");


function setCameraImage() {
    fetch('/is_camera_connected')
        .then(response => response.json())
        .then(data => {
            const timestamp = new Date().getTime();  // Timestamp to prevent caching
            if (data.connected) {
                // Camera is connected, load the latest image
                camera_img.src = `camera_image.jpg?${timestamp}`;
            } else {
                // Camera is disconnected, show black image
                camera_img.src = 'default_cam.jpg';
            }
        })
        .catch(error => {
            console.error("Error checking camera connection:", error);
            camera_img.src = 'default_cam.jpg';  // In case of error, fall back to black image
        });
}
setInterval(setCameraImage, 50);  // Update the image every 50 miliseconds

// ======================= DISTANCE SENSOR DATA HANDLING ===============================

function updateDistanceDisplay() {
    fetch('/distance')
        .then(response => response.json())
        .then(data => {
            const frontBar = document.getElementById('front-bar');
            const backBar = document.getElementById('back-bar');
            const frontText = document.getElementById('front-distance-value');
            const backText = document.getElementById('back-distance-value');
            const maxDistance = 50;
            
            // Update the labels
            if (data.front_connected) {
                const front_distance = data.front_distance !== null ? data.front_distance : 0;
                //const back_distance = data.front_distance !== null ? data.back_distance : 0;
                frontText.textContent = `Distance: ${front_distance} cm`;
                //backText.textContent = `Distance: ${back_distance} cm`;

                // Calculate the height of the cones based on the distance
                // Assuming max distance is 50 cm, so height will range from 0 to 100%
                const front_heightPercentage = Math.max(0, Math.min(1, (maxDistance - front_distance) / maxDistance)) * 100;
                //const back_heightPercentage = Math.max(0, Math.min(1, (maxDistance - back_distance) / maxDistance)) * 100;

                // Set the color and height for the front cone
                frontBar.style.height = `${front_heightPercentage}%`;
                if (front_heightPercentage < 60) {
                    frontBar.classList.add('green');
                    frontBar.classList.remove('orange', 'red');
                } else if (front_heightPercentage < 85) {
                    frontBar.classList.add('orange');
                    frontBar.classList.remove('green', 'red');
                } else {
                    frontBar.classList.add('red');
                    frontBar.classList.remove('green', 'orange');
                }

            } else if(!data.front_connected) {
                // Sensor is disconnected, show placeholder
                frontText.textContent = `Distance: No data`;
                //backText.textContent = `Distance: No data`;
                frontBar.style.height = '0%';
                //backBar.style.height = '0%';
                frontBar.classList.remove('green', 'orange', 'red');
                //backBar.classList.remove('green', 'orange', 'red');
            }

            if(data.back_connected){
                const back_distance = data.back_distance !== null ? data.back_distance : 0;
                backText.textContent = `Distance: ${back_distance} cm`;
                const back_heightPercentage = Math.max(0, Math.min(1, (maxDistance - back_distance) / maxDistance)) * 100;
                backBar.style.height = `${back_heightPercentage}%`;
                if (back_heightPercentage < 60) {
                    backBar.classList.add('green');
                    backBar.classList.remove('orange', 'red');
                } else if (back_heightPercentage < 85) {
                    backBar.classList.add('orange');
                    backBar.classList.remove('green', 'red');
                } else {
                    backBar.classList.add('red');
                    backBar.classList.remove('green', 'orange');
                }
            } else if(!data.back_connected) {
                // Sensor is disconnected, show placeholder
                backText.textContent = `Distance: No data`;
                backBar.style.height = '0%';
                backBar.classList.remove('green', 'orange', 'red');
            }
        })
        .catch(error => {
            console.error("Error fetching distance data:", error);
            document.getElementById('front-distance-value').textContent = 'Distance: Error';
            document.getElementById('back-distance-value').textContent = 'Distance: Error';
            document.getElementById('front-bar').style.height = '0%';
            document.getElementById('back-bar').style.height = '0%';
        });
}

// Update the distance display every 21 milliseconds
setInterval(updateDistanceDisplay, 21);