#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#define ssid "ESP32-Access-Point"
#define password "Nine"
#define blueLedPin 2
#define greenLedPin 22
#define redLedPin 23

WebServer server(80);

bool LEDBlinkStatus = false;
bool LEDAlternateStatus = false;

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  pinMode(blueLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  server.on("/", handle_OnConnect);
  server.on("/button-blink", handle_ledBlink);
  server.on("/button-alternate", handle_ledAlternate);
  server.on("/button-off", handle_ledOff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  if (LEDBlinkStatus) {
    digitalWrite(blueLedPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    delay(500);
    digitalWrite(blueLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    delay(500);
  } else if (LEDAlternateStatus) {
    digitalWrite(blueLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    delay(500);
    digitalWrite(blueLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    delay(500);
  } else {
    digitalWrite(blueLedPin, LOW);
    digitalWrite(redLedPin, LOW);
  }
}

String getHTML() {
  String html = "<title>ESP32 Web Server</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>";
  html += "@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@200&display=swap');";
  html += "* { margin: 0; padding: 0; box-sizing: border-box; font-family: 'Poppins', sans-serif; }";
  html += "body { display: flex; justify-content: center; align-items: center; min-height: 100vh; background: #23242a; transition: .5s; flex-direction: column; }";
  html += ".box { position: relative; width: 380px; height: 420px; background: #1c1c1c; border-radius: 3px; overflow: hidden; }";
  html += ".box::before { content: ''; position: absolute; top: -50%; left: -50%; width: 380px; height: 420px; background: linear-gradient(0deg, transparent, #1e9bff, #ff1867, #6eff3e, transparent); transform-origin: bottom right; animation: animate 6s linear infinite; }";
  html += ".box::after { content: ''; position: absolute; top: -50%; left: -50%; width: 380px; height: 420px; background: linear-gradient(0deg, transparent, #1e9bff, #ff1867, #6eff3e, transparent); transform-origin: bottom right; animation: animate 6s linear infinite; animation-delay: -3s; }";
  html += "@keyframes animate{ 0%{ transform: rotate(0deg); } 100%{ transform: rotate(360deg); } }";
  html += ".form { position: absolute; inset: 2px; background: #28292d; z-index: 10; padding: 50px 40px; display: flex; flex-direction: column; gap: 8px; }";
  html += ".form h2{ color: #1e9bff; font-weight: 600; text-align: center; letter-spacing: 0.1em; }";
  html += ".form h3{ color: #FFFFFF; font-weight: 300; text-align: center; }";
  html += ".form p{ color: #FFFFFF; font-size: 15px; text-align: center; }";
  html += "a{ position: relative; background: #fff; color: #fff; text-decoration: none; text-transform: uppercase; text-align: center; font-size: 1.5em; letter-spacing: 0.1em; font-weight: 600; padding: 10px 30px; transition: 0.5s; }";
  html += "a:hover {background: var(--clr); color: var(--clr); letter-spacing: 0.25em; box-shadow: 0 0 35px var(--clr)}";
  html += "a:before{content: '';position: absolute;inset: 2px;background: #28292d;}";
  html += "a span{position: relative; z-index: 1;}";
  html += "a i { position: absolute; inset: 0; display: block; }";
  html += "a i::before { content: ''; position: absolute; top: 0; left: 80%; width: 10px; height: 4px; background: #28292d; transform: translateX(-50x) skewX(325deg); transition: 0.5s; }";
  html += "a:hover i::before { width: 20px; left: 20%; }";
  html += "a i::after { content: ''; position: absolute; bottom: 0; left: 20%; width: 10px; height: 4px; background: #28292d; transform: translateX(-50x) skewX(325deg); transition: 0.5s; };";
  html += "a:hover i::after { width: 20px; left: 80%; }";
  html += "</style>";

  html += "<body>";
  html += "<div class='box'>";
  html += "<div class='form'>";
  html += "<h2>ESP32 Web Server</h2>";
  html += "<h3>A simple demo using Access Point(AP) Mode</h3>";
  if(LEDBlinkStatus){
    html += "<p>LED Status: Blinking</p>";
    Serial.println("LED Status: Blinking");
  }
  else if(LEDAlternateStatus){
    html += "<p>LED Status: Alternating</p>";
    Serial.println("LED Status: Alternating");
  }
  else{
    html += "<p>LED Status: OFF</p>";
    Serial.println("LED Status: OFF");
  }
  html += "<a href=\"button-blink\" style=\"--clr: #1e9bff\"><span>Blink LED</span><i></i></a>";
  html += "<a href=\"button-alternate\" style=\"--clr: #6eff3e\"><span>Alternate LED</span><i></i></a>";
  html += "<a href=\"button-off\" style=\"--clr: #ff1867\"><span>Turn OFF</span><i></i></a>";
  html += "</div></div></body></html>";
  return html;
}

void handle_OnConnect() {
  digitalWrite(greenLedPin, HIGH);
  server.send(200, "text/html", getHTML());
}

void handle_ledBlink() {
  LEDBlinkStatus = true;
  LEDAlternateStatus = false;

  server.send(200, "text/html", getHTML());
}

void handle_ledAlternate() {
  LEDAlternateStatus = true;
  LEDBlinkStatus = false;
  server.send(200, "text/html", getHTML());
}

void handle_ledOff() {
  LEDBlinkStatus = false;
  LEDAlternateStatus = false;
  server.send(200, "text/html", getHTML());
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}