import * as net from "node:net";
import express from "express";
import * as bcrypt from 'bcrypt';
import {  JSONFilePreset } from "lowdb/node";
import open from "open";

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

let last_message = null;
/** @type {net.Socket} */
let esp_socket = null;
let esp_cam_socket = null;

// Middleware to parse request body
http.use(express.urlencoded({ extended: true })); // Parse form data (application/x-www-form-urlencoded)
http.use(express.json()); // Parse JSON data

// Serve static files
http.use(express.static("static"));

// Take user to login first
http.get("/",(_,res) => {
    res.render("login.ejs");
})

// Take user to login
http.get("/login",async (_,res) => {
    res.render('login');
})

// Take user to registration
http.get("/register", async (_,res) => {
    console.log("reg. Page working");
    res.render("registration");
})

// Take user to main page
http.get("/index",async(_,res) => {
    res.render("index");
})

// Create new user and put their data into users_db.json
http.post("/register", async (req, res) => {
    try {
        const saltRounds = 12;
        const hashedPassword = await bcrypt.hash(req.body.password, saltRounds);
        console.log("hashing successful");

        const new_user = { username: req.body.username, password: hashedPassword };

        // Ensure users_db.data exists and has a users array
        users_db.data = users_db.data || { users: [] }; // Initialize if empty
        users_db.data.users.push(new_user);
        console.log("pushing succesful");
        await users_db.write();

        console.log("pushing user successful");
        res.redirect('/login');
    } catch (error) {
        console.error(error);
        res.status(500).send('An error occurred during registration.');
    }
});

// =================================== USER VERIFICATION =====================================================
// Verify username and password types in login.ejs
http.post('/login', async (req, res) => {
    try {
        const user = users_db.data.users.find((user) => user.username === req.body.username);
        if (!user) {
            res.render("login");
        }

        const passwordMatch = await bcrypt.compare(req.body.password, user.password);
        if (passwordMatch) {
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

// ============== MOTOR CONTROL ==========================================================
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

// ============== PORT LISTENING SECTION =================================================
// TCP server logic for handling connections and data
tcp.on("connection", (socket) => {
    esp_socket = socket;
    console.log("Communication with ESP32 main board established.");
});

tcp_cam.on("connection",(socket) => {
    esp_cam_socket = socket;
    console.log("Communication with ESP32 Camera module established.");

})


// Start the HTTP server
http.listen(HTTP_PORT, "0.0.0.0", async () => {
    console.log(`Hosting http on port ${HTTP_PORT}`);
});
// Start the TCP server
tcp.listen(TCP_PORT, "0.0.0.0", () => {
    console.log(`Server listening on port ${TCP_PORT}`);
});

tcp_cam.listen(TCP_PORT_CAM, "0.0.0.0", () => {
    console.log(`Server listening on port ${TCP_PORT_CAM}`);
});