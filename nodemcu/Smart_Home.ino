#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Servo.h>

const char* ssid = "Eshwar Creativity ";
const char* password = "666666555";

WiFiClient client;
#define RELAY1 D1
#define RELAY2 D2
#define RELAY3 D3
#define RELAY4 D4
#define TRIG_PIN D5
#define ECHO_PIN D6
#define IR_SENSOR D8
#define SERVO_PIN D7  // ✅ GPIO10 (SD3 pin for servo motor)

// ✅ Second ultrasonic sensor
#define ROOM_TRIG D0  
#define ROOM_ECHO D9  

Servo gateServo;
#define TANK_HEIGHT 100

String server = "http://192.168.33.38:3000";

// ✅ Relay states
bool roomRelayState = false;   // sensor based
bool webRelay1State = false;   // web based

// ✅ smooth servo movement function with speed control
void smoothMove(Servo &servo, int from, int to, int speedDelay) {
  if (from < to) {
    for (int pos = from; pos <= to; pos++) {
      servo.write(pos);
      delay(speedDelay);
    }
  } else {
    for (int pos = from; pos >= to; pos--) {
      servo.write(pos);
      delay(speedDelay);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_SENSOR, INPUT);

  pinMode(ROOM_TRIG, OUTPUT);
  pinMode(ROOM_ECHO, INPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // closed default

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

// ✅ NEW function for room ultrasonic
long readRoomDistanceCM() {
  digitalWrite(ROOM_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ROOM_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ROOM_TRIG, LOW);

  long duration = pulseIn(ROOM_ECHO, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, server + "/status");
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      DynamicJsonDocument doc(512);
      deserializeJson(doc, payload);

      // ✅ Save web control states
      webRelay1State = (doc["1"] == "on");
      digitalWrite(RELAY2, doc["2"] == "on" ? LOW : HIGH);
      digitalWrite(RELAY3, doc["3"] == "on" ? LOW : HIGH);
      digitalWrite(RELAY4, doc["4"] == "on" ? LOW : HIGH);

      // ✅ Servo gate control
      static int lastPos = 0;
      if (doc["gate"] == "open" && lastPos != 180) {
        smoothMove(gateServo, lastPos, 180, 20);
        lastPos = 180;
      } else if (doc["gate"] == "closed" && lastPos != 0) {
        smoothMove(gateServo, lastPos, 0, 20);
        lastPos = 0;
      }

      // ✅ Fire sensor
      int fireDetected = digitalRead(IR_SENSOR) == LOW ? 1 : 0;
      HTTPClient httpFire;
      httpFire.begin(client, server + "/fire?status=" + String(fireDetected ? "on" : "off"));
      httpFire.GET();
      httpFire.end();

      // ✅ Water level sensor
      long distance = readDistanceCM();
      int percentage;
      if (distance <= 0 || distance > TANK_HEIGHT) percentage = 0;
      else {
        int waterLevel = TANK_HEIGHT - distance;
        percentage = (waterLevel * 100) / TANK_HEIGHT;
        if (percentage < 0) percentage = 0;
        if (percentage > 100) percentage = 100;
      }

      HTTPClient httpWater;
      httpWater.begin(client, server + "/water?level=" + String(percentage));
      httpWater.GET();
      httpWater.end();

      // ✅ Room ultrasonic sensor
      long roomDistance = readRoomDistanceCM();
      HTTPClient httpRoom;
      httpRoom.begin(client, server + "/room?distance=" + String(roomDistance));
      httpRoom.GET();
      httpRoom.end();

      // ✅ Relay1 = web OR room sensor
      if (roomDistance > 0 && roomDistance < 20) {
        roomRelayState = true;
      } else {
        roomRelayState = false;
      }

      if (webRelay1State || roomRelayState) {
        digitalWrite(RELAY1, LOW);
        Serial.println("Relay1 = ON (Web/Sensor)");
      } else {
        digitalWrite(RELAY1, HIGH);
        Serial.println("Relay1 = OFF");
      }
    }
    http.end();
  }
  delay(50);
}
