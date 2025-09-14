const express = require('express');
const cors = require('cors');
const path = require('path');

const app = express();
const port = 3000;//port 

app.use(cors());
app.use(express.static(path.join(__dirname, '../frontend')));

//realy json data LED
let relays = { 1: 'off', 2: 'off', 3: 'off', 4: 'off' };
let waterLevel = 0;
let fire = "off";
let gate = "closed";
// Status API
app.get('/status', (req, res) => {
  res.json({
    ...relays,
    water: waterLevel,
    fire,
    gate
  });
});

// Relay toggle
app.get('/toggle/:id', (req, res) => {
  const id = parseInt(req.params.id);
  if (relays[id]) {
    relays[id] = relays[id] === 'on' ? 'off' : 'on';
    res.send(`Relay ${id} is now ${relays[id]}`);
  } else {
    res.status(400).send('Invalid Relay ID');
  }
});

// Update water level
app.get('/water', (req, res) => {
  const level = parseInt(req.query.level);
  if (!isNaN(level)) {
    waterLevel = level;
    res.send(`Water level updated: ${waterLevel}%`);
  } else {
    res.status(400).send('Invalid water level');
  }
});

// Fire sensor update
app.get('/fire', (req, res) => {
  fire = req.query.status === "on" ? "on" : "off";
  res.send(`Fire status updated: ${fire}`);
});

// Gate toggle
app.get('/gate/toggle', (req, res) => {
  gate = (gate === "closed") ? "open" : "closed";
  res.send(`Gate is now ${gate}`);
});


//Server running port
app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});
