import * as net from "node:net";
import express from "express";
import path from "path";
import * as bcrypt from 'bcrypt';
import {  JSONFilePreset } from "lowdb/node";



const defaultData = {users: []};
const users_db = await JSONFilePreset("users_db.json", defaultData);
users_db.data = { users: [] }; // Initialize if empty
await users_db.read();

// Express app and HTTP settings
const http = express();
http.set('view engine','ejs');
http.set('views', 'static');

const HTTP_PORT = 8080;

// TCP Server settings
const TCP_PORT = 1313;
const tcp = net.createServer();

let last_message = null;
/** @type {net.Socket} */
let esp_socket = null;

// Middleware to parse request body
http.use(express.urlencoded({ extended: false })); // Parse form data (application/x-www-form-urlencoded)
http.use(express.json()); // Parse JSON data

// Serve static files
http.use(express.static("static", { index: 'login.html' }));

// Route to fetch last message
http.get("/last_message", (_, res) => {
    res.send(`${last_message}`);
});

// Route to control LED state
http.post("/led_state", (_, res) => {
    esp_socket.write("l");
    res.send("LED");
});

// User registration route
http.get("/login",async (_,res) => {
    res.render('login.ejs');
})

http.get("/register", async (_,res) => {
    console.log("reg. Page working");
    res.render("registration.ejs");
})

http.get("/index",async(_,res) => {
    res.render("index.ejs");
})

http.post("/register", async (req, res) => {
    try {
        console.log(users_db.data);
        const saltRounds = 12;
        const hashedPassword = await bcrypt.hash(req.body.password, saltRounds);
        console.log("hashing successful");

        const new_user = { username: req.body.username, password: hashedPassword };

        // Ensure users_db.data exists and has a users array
        users_db.data = users_db.data || { users: [] }; // Initialize if empty
        users_db.data.users.push(new_user);
        console.log("pushing succesful");
        await users_db.write();
        console.log("writing succesful");
        console.log(users_db.data);
        res.redirect('/login');
    } catch (error) {
        console.error(error);
        res.status(500).send('An error occurred during registration.');
    }
});

// User login route
http.post('/login', async (req, res) => {
    const { username_, password_ } = req.body;
    try {
        const user = users_db.find((user) => user.username === username_);
        if (!user) {
            return res.status(404).send('User not found.');
        }

        const passwordMatch = await bcrypt.compare(password_, user.password);
        if (passwordMatch) {
            res.send('Login successful.');
            return res.redirect('/index');
        } else {
            res.status(401).send('Incorrect password.');
        }
    } catch (error) {
        console.log(error);
        res.status(500).send('An error occurred during login.');
    }
});

// TCP server logic for handling connections and data
tcp.on("connection", (socket) => {
    esp_socket = socket;
    socket.on("data", (data) => {
        console.log(data);
        last_message = data.toString();
    });
});

// Start the HTTP server
http.listen(HTTP_PORT, "0.0.0.0", () => {
    console.log(`Hosting http on port ${HTTP_PORT}`);
});

// Start the TCP server
tcp.listen(TCP_PORT, "0.0.0.0", () => {
    console.log(`Server listening on port ${TCP_PORT}`);
});