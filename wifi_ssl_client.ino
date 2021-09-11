/*
    EFFICIENT SSL CLIENT WITH TWEAKLY
    Board : Nano 33 IoT
    by Mirko Pacioni
    (c) Filo Connesso
*/

//Include libs
#include <Tweakly.h>
#include <SPI.h>
#include <WiFiNINA.h>

//Define pins
#define LED 13

//Params
char ssid[] = "";
char pass[] = "";
char server[] = "www.filoconnesso.it";

//Objects
WiFiSSLClient client;
TickTimer blinker;
TickTimer requester;
Pad led(LED);

void setup() {

  Serial.begin(115200);

  //Wait for the Serial Monitor
  while (!Serial);

  //Check Wireless Module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  //Check Wi-Fi firmware version
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  //Connect to Wi-Fi
  Serial.print("Attempting to connect to ");
  Serial.print(ssid);
  Serial.print("...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(100);
  }
  Serial.print("[CONNECTED]");
  Serial.println();

  //Blink timer
  blinker.attach(200, [] { led.toggle(); });

  //Request timer
  requester.attach(2000, [] {
    if (client.connected()) {
      client.println("GET /tweakly/test.php HTTP/1.1");
      client.println("Host: www.filoconnesso.it");
      client.println("Keep-Alive: timeout=1, max=100");
      client.println("Connection: Keep-Alive");
      client.println();
      client.flush();
      Serial.println();
      Serial.println();
    }
  });

}

void loop() {

  TweaklyRun();

  //Print client response
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  //Reconnect client
  if (!client.connected()) {
    client.stop();
    client.connect(server, 443);
  }

  //Put your code here :-)

}
