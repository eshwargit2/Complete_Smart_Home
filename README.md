# Smart Home Automation Using NodeMCU

![Dashboard](frontend/dashboard.png)

![Circuit Connection](frontend/circuit.png)

## Overview
This project is a complete smart home automation system using NodeMCU (ESP8266), featuring:
- Web dashboard for controlling lights, gate, and monitoring sensors
- Voice and chatbot control for lights, gate, and water level
- Real-time fire alert and water tank monitoring
- Circuit diagram for hardware setup

## Features
- Control Room, Bedroom, Kitchen, and Living Room lights
- Open/Close gate via dashboard or voice/chatbot
- Monitor water tank level
- Fire alert system
- Voice and chatbot commands (text/voice)

## Circuit Diagram
See the image above for wiring details. Connect:
- NodeMCU to 4-channel relay module
- Ultrasonic sensors for water tank and room
- IR sensor for fire detection
- Servo motor for gate

## Dashboard
The dashboard provides:
- Real-time status of all devices
- Voice command button
- Fire alert popup
- Water level visualization

## Getting Started

### 1. Clone the Repository
```sh
git clone https://github.com/eshwargit2/Nodemcu_Home_Automation.git
cd Nodemcu_Home_Automation
```

### 2. Install Backend Dependencies
```sh
cd backend
npm install
```

### 3. Upload Firmware to NodeMCU
- Open `nodemcu/Smart_Home.ino` in Arduino IDE
- Install required libraries: ESP8266WiFi, ESP8266HTTPClient, ArduinoJson, Servo
- Set your WiFi credentials in the code
- Upload to NodeMCU

### 4. Start the Backend Server
```sh
node server.js
```

### 5. Open the Dashboard
- Open your browser and go to `http://<your-pc-ip>:3000`
- Use the dashboard to control devices and monitor sensors

## API Endpoints
- `GET /status` — Get current status
- `GET /toggle/:id` — Toggle relay (light)
- `GET /gate/toggle` — Open/close gate
- `GET /water?level=XX` — Update water level
- `GET /fire?status=on|off` — Update fire status
- `POST /fire` — Update fire status via POST
- `POST /api/light` — Chatbot/voice control for lights/gate/level

## Voice & Chatbot Commands
- "Turn on bedroom lights"
- "Kitchen lights off"
- "Open gate"
- "What is the water level?"
- "Fire alert"

## Images
- Dashboard: `frontend/dashboard.png`
- Circuit: `frontend/circuit.png`

## License
MIT

---
> Made by Eshwar Creativity
