const status_div = document.getElementById("status");
const f_led_off = document.getElementById("front_LEDs_off");
const f_led_default = document.getElementById("front_LEDs_default");
const f_led_max = document.getElementById("front_LEDs_max");
// Get references to the buttons
const upButton = document.getElementById('move_up');
const downButton = document.getElementById('move_down');
const leftButton = document.getElementById('move_left');
const rightButton = document.getElementById('move_right');

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


// Front LED control funcitons
function front_LEDs_off(){
    const request = new Request("/front_led_off",{method: "POST"});
    fetch(request).then(
        response => response.text()
    );
}
function front_LEDs_default(){
    const request = new Request("/front_led_default",{method: "POST"});
    fetch(request).then(
        response => response.text()
    );
}
function front_LEDs_max(){
    const request = new Request("/front_led_max",{method: "POST"});
    fetch(request).then(
        response => response.text()
    );
}

f_led_off.addEventListener('click',front_LEDs_off);
f_led_default.addEventListener('click',front_LEDs_default);
f_led_max.addEventListener('click',front_LEDs_max);

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

        const request = new Request(`/move_${direction}`, {method : "POST"});
        fetch(request).then(
            response => response.text()
        );
        return direction;  // Return the active direction
    }
    return activeTracker;  // Return the already active direction (no change)
}

// Function to reset the button state and enable the opposite button when key is released
function releaseButton(button, oppositeButton, activeTracker) {
    if (activeTracker !== null) {  // Only reset if this is the active button
        button.classList.remove('active');  // Remove the active class to restore default color
        oppositeButton.disabled = false;  // Re-enable the opposite button

        const request = new Request(`/stop_${direction}`, {method : "POST"});
        fetch(request).then(
            response => response.text()
        );
        return null;  // Clear the active state
    }
    return activeTracker;  // If this button wasn't active, no change
}

// Event listener for keyboard arrows - press
document.addEventListener('keydown', function(event) {
    switch (event.key) {
        case 'ArrowUp':
            activeVertical = pressButton(upButton, downButton, 'up', activeVertical);
            break;
        case 'ArrowDown':
            activeVertical = pressButton(downButton, upButton, 'down', activeVertical);
            break;
        case 'ArrowLeft':
            activeHorizontal = pressButton(leftButton, rightButton, 'left', activeHorizontal);
            break;
        case 'ArrowRight':
            activeHorizontal = pressButton(rightButton, leftButton, 'right', activeHorizontal);
            break;
    }
});

// Event listener for keyboard arrows - release
document.addEventListener('keyup', function(event) {
    switch (event.key) {
        case 'ArrowUp':
            activeVertical = releaseButton(upButton, downButton, activeVertical);
            break;
        case 'ArrowDown':
            activeVertical = releaseButton(downButton, upButton, activeVertical);
            break;
        case 'ArrowLeft':
            activeHorizontal = releaseButton(leftButton, rightButton, activeHorizontal);
            break;
        case 'ArrowRight':
            activeHorizontal = releaseButton(rightButton, leftButton, activeHorizontal);
            break;
    }
});



