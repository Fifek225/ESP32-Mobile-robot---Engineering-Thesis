#include <WiFi.h>

char ssid[] = "Filip wifi";
char password[] = "tadeuszsznuk";

int status = WL_IDLE_STATUS;
IPAddress server(20,215,192,11);

WiFiClient client;

const int led_pin = 14;
bool led_state = true;

void setup() {
  // put your setup code here, to run once:

  pinMode(led_pin,OUTPUT);

  Serial.begin(115200);
  disableCore1WDT();
  Serial.println("Attempting to connect to Network.");
  Serial.print("SSID: ");Serial.println(ssid);
  
  status = WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
    }

  Serial.print("Connected to WiFi Network: "); Serial.println(ssid);
  while(!client.connect(server,1313)){
    Serial.println(".");
    delay(1000);
  }
  client.print("Pichal kinda cute");
  client.flush();
}

void loop() {
  if(client.available()){
    char byte = client.read();
    Serial.print(byte);
    if(byte == 'l'){
      led_state = !led_state;
      digitalWrite(led_pin,led_state);
    }
  }
}


