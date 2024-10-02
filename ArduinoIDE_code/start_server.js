import * as net from "node:net";
import express from "express";
import * as bcrypt from 'bcrypt';
import { Low, JSONFile } from "lowdb";


// User class
class User {
    constructor(username, password) {
        this.username = username;
        this.password = password;
    }
}

// Initialize the database
const adapter = new JSONFile('users_db.json');
const users_db = new Low(adapter);
await users_db.read();

users_db.data ||= { posts: [] }; // Ensure posts array exists

const { posts } = users_db.data;

// Express app and HTTP settings
const http = express();
const HTTP_PORT = 8080;

// TCP Server settings
const TCP_PORT = 1313;
const tcp = net.createServer();

let last_message = null;
/** @type {net.Socket} */
let esp_socket = null;

// Middleware to parse request body
http.use(express.urlencoded({ extended: true })); // Parse form data (application/x-www-form-urlencoded)
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
http.post("/register", async (req, res) => {
    const { username_, password_ } = req.body;
    try {
        const saltRounds = 12;
        const hashedPassword = await bcrypt.hash(password_, saltRounds);
        const user_post = { username: username_, password: hashedPassword };
        posts.push(user_post); // Update the posts array
        await users_db.write(); // Write the update to the database

        res.status(201).send('Registration successful.');
        res.redirect('/login.html');
    } catch (error) {
        console.error(error);
        res.status(500).send('An error occurred.');
    }
});

// User login route
http.post('/login', async (req, res) => {
    const { username_, password_ } = req.body;
    try {
        const user = posts.find((user) => user.username === username_);
        if (!user) {
            return res.status(404).send('User not found.');
        }

        const passwordMatch = await bcrypt.compare(password_, user.password);
        if (passwordMatch) {
            res.send('Login successful.');
            return res.redirect('index.html');
        } else {
            res.status(401).send('Incorrect password.');
        }
    } catch (error) {
        console.log(error);
        res.status(500).send('An error occurred.');
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