import express from "express";
import bodyParser from "body-parser"

const app = express();
app.use(bodyParser.json());

app.get("/", (req, res)=>{
    console.log("YES BACKEND IS HIT!!");
    res.sendStatus(200);
})

app.post("/", (req, res)=>{
    console.log(req.body.message);
    res.sendStatus(200);
})

app.listen(3000, ()=>{
    console.log("Server is up on port 3000");
})