
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>

const char* ssid = "ssid"; //Enter SSID
const char* password = "password"; //Enter Password
const char* websockets_server = "www.myserver.com:8080"; //server adress and port

using namespace websockets;


WebsocketsClient client;

void connectWiFi() {
  // Connect to wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  // Wait some time to connect to wifi
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
}

void connectWebsocket() {
  client.connect(websockets_server);
}

void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  Serial.println(message.data());
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

void setup() {
  Serial.begin(9600);
  
  connectWiFi();
  connectWebsocket();

  // Setup Callbacks
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
}

void loop() {
  client.poll();
}
