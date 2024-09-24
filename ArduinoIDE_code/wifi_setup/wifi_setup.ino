#include <WiFi.h>
#include <WebServer.h>
#include "../Website.h"


#define LOCAL_SSID "Filip Hotspot"
#define LOCAL_PASSWORD "tadeuszsznuk"

#define AP_SSID "Mobile Robot"
#define AP_PASSWORD "TadeuszSznuk"
//---------------------------------------------------------------------------------

char XML[1024];
char buf[32];
IPAddress Actual_IP;
IPAddress PageIP(192, 168, 216, 1);
IPAddress gateway(192, 168, 216, 1);
IPAddress subnet(255, 255, 248, 0);
IPAddress ip;

WebServer server(80);

void setup() {
  Serial.begin(115200);
  //disableCore0WDT();
  disableCore1WDT();

  Serial.println("Starting server...");

  WiFi.begin(LOCAL_SSID,LOCAL_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.print("Connected to: ");Serial.println(LOCAL_SSID);
  Serial.print("IP Address: ");Serial.println(WiFi.localIP());
  Actual_IP = WiFi.localIP();
  printWifiStatus();

  server.on("/",SendWebsite);
  server.on("/xml",SendXML);
  server.begin();
}

void loop() {
  server.handleClient();
}


// FUNCTION DEFINITIONS ------------------------------------------------------------
void printWifiStatus() {

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("Open http://");
  Serial.println(ip);
}

void SendWebsite(){
  server.send(200, "text/html", WEB_PAGE);
}

void SendXML(){
  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");
  Serial.println(XML);
  server.send(300,"text/xml",XML);
}