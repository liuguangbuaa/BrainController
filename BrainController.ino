
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#include "my_super_secret_passwords.h"


const char* ssid = SUPER_SECRET_SSID; //Enter SSID
const char* password = SUPER_SECRET_PASSWORD; //Enter Password
const char* websockets_server = SUPER_SECRET_SERVER; //server adress and port

using namespace websockets;


WebsocketsClient client;
Servo left;
Servo right;

void connectWiFi() {
  // Connect to wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  // Wait some time to connect to wifi
  while(true) {
    Serial.print(".");
    delay(100);
    if(WiFi.status() == WL_CONNECTED) {
      break;
    }
  }
  Serial.println("Connected!");
}

void connectWebsocket() {
  Serial.print("Connecting Websocket");
  while(true) {
    Serial.print(".");
    client.connect(websockets_server);
    delay(100);  
    if(client.available()) {
      break;
    }
  }
  Serial.println("Connected!");
}

const char* rot_type = "MEDITATION: ";

void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  const char* msg = message.c_str();
  Serial.println(msg);
  size_t len = strlen(msg);
  if(!strncmp(msg, rot_type, strlen(rot_type))) {
    long val = atol(strchr(msg, ' '));
    set(&left, (val - 50)/100.0);
  }
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed, attempting reopen");
    client.connect(websockets_server);
  } else if (event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
    client.pong();
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}

void set(Servo* s, double val) {
  if(val > 1.0) {
    val = 1.0;
  } else if(val < -1.0) {
    val  = -1.0;
  }
  s->writeMicroseconds(1500 + 500*val);
}

void setup() {
  Serial.begin(9600);
  
  connectWiFi();
  connectWebsocket();
  left.attach(4);
  
  // Setup Callbacks
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
}

void loop() {
  client.poll();
}
