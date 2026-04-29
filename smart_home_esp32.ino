#include <WiFi.h>
#include <WebServer.h>

/* WiFi Credentials */
const char* ssid = "wifi name ";
const char* password = "your pass";

/* Web Server */
WebServer server(80);

/* Relay GPIOs */
#define RELAY1 26
#define RELAY2 25
#define RELAY3 33
#define RELAY4 32

/* WiFi Status LED */
#define WIFI_LED 2   // Built-in LED

unsigned long previousMillis = 0;
bool ledState = false;

/* ---------- HTML PAGE ---------- */
String webPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Smart Switch</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial;
      background: #020617;
      color: white;
      text-align: center;
      padding: 20px;
    }
    h2 { color: #38bdf8; }

    .card {
      background: #0f172a;
      padding: 18px;
      margin: 12px auto;
      border-radius: 12px;
      max-width: 330px;
    }

    .row {
      display: flex;
      justify-content: space-between;
      align-items: center;
    }

    .dot {
      width: 12px;
      height: 12px;
      border-radius: 50%;
      background: red;
      display: inline-block;
    }

    button {
      padding: 10px 16px;
      font-size: 14px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
    }

    .on { background: #22c55e; }
    .off { background: #ef4444; color: white; }

    .all {
      width: 150px;
      margin: 10px;
      font-size: 16px;
    }
  </style>
</head>

<body>
  <h2>ESP32 Smart Home</h2>

  <button class="on all" onclick="allOn()">ALL ON</button>
  <button class="off all" onclick="allOff()">ALL OFF</button>

  <div class="card">
    <div class="row">
      <span>Light 1</span>
      <span id="d1" class="dot"></span>
      <button onclick="toggle(1)">TOGGLE</button>
    </div>
  </div>

  <div class="card">
    <div class="row">
      <span>Light 2</span>
      <span id="d2" class="dot"></span>
      <button onclick="toggle(2)">TOGGLE</button>
    </div>
  </div>

  <div class="card">
    <div class="row">
      <span>Light 3</span>
      <span id="d3" class="dot"></span>
      <button onclick="toggle(3)">TOGGLE</button>
    </div>
  </div>

  <div class="card">
    <div class="row">
      <span>Light 4</span>
      <span id="d4" class="dot"></span>
      <button onclick="toggle(4)">TOGGLE</button>
    </div>
  </div>

<script>
  let state = [0,0,0,0];

  function toggle(n) {
    let i = n - 1;
    fetch(state[i] ? `/r${n}off` : `/r${n}on`);
    state[i] = !state[i];
    document.getElementById("d"+n).style.background =
      state[i] ? "lime" : "red";
  }

  function allOn() {
    for (let i=1;i<=4;i++) {
      fetch(`/r${i}on`);
      state[i-1] = 1;
      document.getElementById("d"+i).style.background = "lime";
    }
  }

  function allOff() {
    for (let i=1;i<=4;i++) {
      fetch(`/r${i}off`);
      state[i-1] = 0;
      document.getElementById("d"+i).style.background = "red";
    }
  }
</script>

</body>
</html>
)rawliteral";
}

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  // Relays OFF (LOW trigger relay)
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", webPage());
  });

  server.on("/r1on",  [](){ digitalWrite(RELAY1, LOW);  server.send(200,"text/plain","R1 ON"); });
  server.on("/r1off", [](){ digitalWrite(RELAY1, HIGH); server.send(200,"text/plain","R1 OFF"); });

  server.on("/r2on",  [](){ digitalWrite(RELAY2, LOW);  server.send(200,"text/plain","R2 ON"); });
  server.on("/r2off", [](){ digitalWrite(RELAY2, HIGH); server.send(200,"text/plain","R2 OFF"); });

  server.on("/r3on",  [](){ digitalWrite(RELAY3, LOW);  server.send(200,"text/plain","R3 ON"); });
  server.on("/r3off", [](){ digitalWrite(RELAY3, HIGH); server.send(200,"text/plain","R3 OFF"); });

  server.on("/r4on",  [](){ digitalWrite(RELAY4, LOW);  server.send(200,"text/plain","R4 ON"); });
  server.on("/r4off", [](){ digitalWrite(RELAY4, HIGH); server.send(200,"text/plain","R4 OFF"); });

  server.begin();
}

/* ---------- LOOP ---------- */
void loop() {
  server.handleClient();

  // WiFi connected LED blink
  if (WiFi.status() == WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(WIFI_LED, ledState);
    }
  } else {
    digitalWrite(WIFI_LED, LOW);
  }
}



//or you can check your servo by this code 



#include <Servo.h>

Servo myServo;  // create servo object

void setup() {
  myServo.attach(9);  // connect servo signal pin to digital pin 9
}

void loop() {
  myServo.write(0);   // move to 0°
  delay(1000);

  myServo.write(90);  // move to 90°
  delay(1000);

  myServo.write(180); // move to 180°
  delay(1000);
}


//mow code to check rain sensoor //
int rainDigitalPin = 2;   // DO pin
int rainAnalogPin = A0;   // AO pin

void setup() {
  Serial.begin(9600);
  pinMode(rainDigitalPin, INPUT);
}

void loop() {
  int digitalValue = digitalRead(rainDigitalPin);
  int analogValue = analogRead(rainAnalogPin);

  Serial.print("Digital Output: ");
  Serial.print(digitalValue);
  
  Serial.print(" | Analog Value: ");
  Serial.println(analogValue);

  if (digitalValue == LOW) {
    Serial.println("Rain Detected 🌧️");
  } else {
    Serial.println("No Rain ☀️");
  }

  delay(1000);
}
 // do connection by your self and set pin in code according to you //

