import * as net from "node:net";
import express from "express";
import * as bcrypt from 'bcrypt';
import {  JSONFilePreset } from "lowdb/node";
import fs from "fs";

// ======================= SERVER SETUP ===============================================
// Setup for user database using lowdb
const defaultData = {users: []};
const users_db = await JSONFilePreset("users_db.json", defaultData);
await users_db.read();

// Express app and HTTP settings
const http = express();
http.set('view engine','ejs');
http.set('views', 'static');

// HTTP settings - 8080 is for the website
const HTTP_PORT = 8080;

// TCP Server settings - 1313 is port for ESP32
const TCP_PORT = 1313;
const tcp = net.createServer();

const TCP_PORT_CAM = 1314;
const tcp_cam = net.createServer();


/** @type {net.Socket} */
let esp_socket = null;
let esp_cam_socket = null;

// Middleware to parse request body
http.use(express.urlencoded({ extended: true })); // Parse form data (application/x-www-form-urlencoded)
http.use(express.json()); // Parse JSON data

function isAuthenticated(req, res, next) {
    if (req.session.isAuthenticated) {
        return next();
    }
    res.redirect('/login');
}

// Routes

// Take user to login page
http.get("/login", (_, res) => {
    res.render('login');
});

// Take user to registration page
http.get("/register", (_, res) => {
    console.log("reg. Page working");
    res.render("registration");
});

// Take user to main page, protected by isAuthenticated middleware
http.get("/index", isAuthenticated, (_, res) => {
    res.render("index");
});

// Create new user and save their data
http.post("/register", async (req, res) => {
    try {
        const saltRounds = 12;
        const hashedPassword = await bcrypt.hash(req.body.password, saltRounds);
        console.log("hashing successful");

        const new_user = { username: req.body.username, password: hashedPassword };

        users_db.data = users_db.data || { users: [] };
        users_db.data.users.push(new_user);
        console.log("pushing successful");
        await users_db.write();

        res.redirect('/login');
    } catch (error) {
        console.error(error);
        res.status(500).send('An error occurred during registration.');
    }
});

// User login route
http.post('/login', async (req, res) => {
    try {
        const user = users_db.data.users.find((user) => user.username === req.body.username);
        if (!user) {
            return res.render("login");
        }

        const passwordMatch = await bcrypt.compare(req.body.password, user.password);
        if (passwordMatch) {
            req.session.isAuthenticated = true;
            return res.redirect('/index');
        } else {
            res.status(401).send('Incorrect password.');
        }
    } catch (error) {
        console.log(error);
        res.status(500).send('An error occurred during login.');
    }
});

// ================================ LED CONTROL SECTION =============================================

// Front LEDs turn off
http.post("/front_led_off", (_, res) => {
    esp_socket.write("1");
    res.send("LED off");
});

// Set front LEDs to low power
http.post("/front_led_default", (_, res) => {
    esp_socket.write("2");
    res.send("LED default");
});

// Set front LEDs to high power
http.post("/front_led_max", (_, res) => {
    esp_socket.write("3");
    res.send("LED max");
});

// ================================ MOTOR CONTROL ==========================================================
http.post("/move_up",(_,res) => {
    esp_socket.write("u");
    res.send("Motor up.");
});
http.post("/move_left",(_,res) => {
    esp_socket.write("l");
    res.send("Motor left.");
});

http.post("/move_right",(_,res) => {
    esp_socket.write("r");
    res.send("Motor right.");
});

http.post("/move_down",(_,res) => {
    esp_socket.write("d");
    res.send("Motor down.");
});

http.post("/stop_up",(_,res) => {
    esp_socket.write("s");
    res.send("Stop up.");
});

http.post("/stop_left",(_,res) => {
    esp_socket.write("h");
    res.send("Stop left.");
});

http.post("/stop_right",(_,res) => {
    esp_socket.write("h");
    res.send("Stop right.");
});

http.post("/stop_down",(_,res) => {
    esp_socket.write("s");
    res.send("Stop down.");
});


// ============================ CAMERA IMAGE RECEIVING SECTION ==========================

let video_buffer = Buffer.alloc(0);  // Initialize an empty buffer
const image_path = 'static/camera_image.jpg';  // Path to save the image
let cameraConnected = false;
const CAMERA_TIMEOUT = 2000;


tcp_cam.on("connection", (socket) => {
    esp_cam_socket = socket;
    cameraConnected = true;
    console.log("Communication with ESP32 Camera module established.");

    socket.on('data', (data) => {
        console.log("Received data chunk of size:", data.length);
        // Append new data to the buffer
        video_buffer = Buffer.concat([video_buffer, data]);
    });

    socket.on('end', () => {
        console.log("Camera TCP port connection closed. Writing image to file.");

        // Write the complete buffer to a file when the connection is closed
        fs.writeFile(image_path, video_buffer, 'binary', (err) => {
            if (err) {
                console.error("Error saving video buffer as image:", err);
            } else {
                console.log("Image saved successfully as camera_image.jpg");
            }
            // Reset the buffer after the image is saved
            video_buffer = Buffer.alloc(0);
        });
        cameraConnected = false;
    });

    socket.on('error', (err) => {
        console.error("Error on TCP connection:", err);
    });
});

// setInterval(() => {
//     if (cameraConnected && Date.now() - lastDataTimestamp > CAMERA_TIMEOUT) {
//         console.log("Camera is not sending data. Marking as disconnected.");
//         cameraConnected = false;  // Set as disconnected if no data recently
//     }
// }, CAMERA_TIMEOUT);

http.get('/is_camera_connected', (_, res) => {
    res.json({ connected: cameraConnected });
});


tcp_cam.listen(TCP_PORT_CAM, "0.0.0.0", () => {
    console.log(`Server listening on port ${TCP_PORT_CAM}`);
});



// ============================ DISTANCE SENSOR DATA CONVERSION ====================================

let distance_val = null;  // Variable to store the latest distance data
let distanceSensorConnected = false;  // Flag to track the connection status

http.get('/front-distance', (_,res) => {
    res.json({front_distance: distance_val,
        connected: distanceSensorConnected
    });
})


// ============================ MAIN BOARD TCP PORT LISTENING & DATA COLLETION  ==================================
// TCP server logic for handling connections and data
tcp.on("connection", (socket) => {
    esp_socket = socket;
    console.log("Communication with ESP32 main board established.");
    distanceSensorConnected = true;

    socket.on('data', (data) => {
        const message = data.toString().trim();
        // If incoming data is from a Front distance sensor
        if(message.startsWith('FD: ')){ 
            distance_val = parseFloat(message.slice(3));
        }
    })

    socket.on('end', () => {
        console.log("Connection with ESP32 main board ended.");
        distanceSensorConnected = false; 
    });

    socket.on('error', (err) => {
        console.error("Connection with ESP32 main board was broken", err);
        distanceSensorConnected = false;  // Mark as disconnected
    });

});


// Start the HTTP server
http.listen(HTTP_PORT, "0.0.0.0", async () => {
    console.log(`Hosting http on port ${HTTP_PORT}`);
});
// Start the TCP server
tcp.listen(TCP_PORT, "0.0.0.0", () => {
    console.log(`Server listening on port ${TCP_PORT}`);
});
