import * as net from "node:net";
import express from "express";
import bcrypt from "bcrypt";
import { JSONFilePreset } from 'lowdb/node'

// //password protection

// class User {
//     constructor(username,password){
//         this.username = username;
//         this.password = password;
//     }
// }

// const defaultData = { posts: [] };
// const password_db = await JSONFilePreset<User>('password_db.json', defaultData);
// const {posts} = password_db.data;

// http.post("/register", async (req,res) =>{
//     const {username_,password_} = req.body;
//     try{
//         const saltRounds=12;
//         const hashedPassword = await bcrypt.hash(password_,saltRounds);
//         const user_post = {username: username_, password: hashedPassword};
//         await password_db.update(({ posts }) => posts.push(user_post))

//         res.status(201).send('Registration successful.')
//     }catch(error){
//         console.error(error);
//         res.status(500).send('An error occured.')
//     }
//     try{
//         const data = password_db.read();
//         console.log(data);

//     }catch(error){
//         console.error(error);
//         res.status(500).send('An error occured.')
//     }
// })

// http.post('/login',async (req,res) => {
//     const {username_,password_} = req.body;
//     try{
//         const user = posts.find((user) => user.username === username_)
//         if(!user){return res.status(404).send('User not found.');}
        
//         const passwordMatch = await bcrypt.compare(password_, user.password);

//         if(passwordMatch){
//             res.send('Login successful.');
//         }else{
//             res.status(401).send('Incorrect password.')
//         }

//     }catch(error){
//         console.log(error);
//         res.status(500).send('An error occured.');
//     }
// })

const http = express();
const HTTP_PORT = 8080;

const TCP_PORT = 1313;
const tcp = net.createServer();

let last_message = null;
/** @type{net.Socket} */
let esp_socket = null;

http.use(express.static("static"));

http.get("/last_message", (_, res) => {
    res.send(`${last_message}`);
});

http.post("/led_state",(_,res) => {
    esp_socket.write("l");
    res.send("LED");
})


tcp.on("connection", (socket) => {
    esp_socket = socket;
    socket.on("data", (data) => {
        console.log(data);
        last_message = data.toString();
    });
});

http.listen(HTTP_PORT, "0.0.0.0", () => {
    console.log(`Hosting http on port ${HTTP_PORT}`);
});

tcp.listen(TCP_PORT, "0.0.0.0", () => {
    console.log(`server listening on port ${TCP_PORT}`);
});