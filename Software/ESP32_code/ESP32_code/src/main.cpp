#include <WiFi.h>
#include <Arduino.h>

#include "control_functions.h"  // Assuming this file exists and is needed



// WiFi network data
char ssid[] = "Filip wifi";
char password[] = "tadeuszsznuk";
int status = WL_IDLE_STATUS;
IPAddress server(20, 215, 192, 11);

WiFiClient client;

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  disableCore1WDT();
  Serial.println("Attempting to connect to Network.");
  Serial.print("SSID: ");
  Serial.println(ssid);

  // Connect to WiFi
  status = WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected to WiFi Network: ");
  Serial.println(ssid);

  // Connect to server
  while (!client.connect(server, 1313)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("ESP32 connected to TCP port 1313");


  // Setup PWM for LED control
  ledcAttachPin(FRONT_LED_PIN_1,0);
  //ledcAttachPin(FRONT_LED_PIN_2,1);

  ledcSetup(0,LED_FREQUENCY,8);
  //ledcSetup(1,LED_FREQUENCY,8);

  // Setup PWM for motors
  ledcAttachPin(FL_MOTOR_PIN_1,FL_MOTOR_CH_1);
  ledcAttachPin(FL_MOTOR_PIN_2,FL_MOTOR_CH_2);
  ledcAttachPin(FR_MOTOR_PIN_1,FR_MOTOR_CH_1);
  ledcAttachPin(FR_MOTOR_PIN_2,FR_MOTOR_CH_2);
  ledcAttachPin(BL_MOTOR_PIN_1,BL_MOTOR_CH_1);
  ledcAttachPin(BL_MOTOR_PIN_2,BL_MOTOR_CH_2);
  ledcAttachPin(BR_MOTOR_PIN_1,BR_MOTOR_CH_1);
  ledcAttachPin(BR_MOTOR_PIN_2,BR_MOTOR_CH_2);

  ledcSetup(FL_MOTOR_CH_1,MOTOR_FREQUENCY,8);
  ledcSetup(FL_MOTOR_CH_2,MOTOR_FREQUENCY,8);
  ledcSetup(FR_MOTOR_CH_1,MOTOR_FREQUENCY,8);
  ledcSetup(FR_MOTOR_CH_2,MOTOR_FREQUENCY,8);
  ledcSetup(BL_MOTOR_CH_1,MOTOR_FREQUENCY,8);
  ledcSetup(BL_MOTOR_CH_2,MOTOR_FREQUENCY,8);
  ledcSetup(BR_MOTOR_CH_1,MOTOR_FREQUENCY,8);
  ledcSetup(BR_MOTOR_CH_2,MOTOR_FREQUENCY,8);


}

void loop() {
  // Check if data is available from server
  if (client.available()) {
    char byte = client.read();  // Read a byte (this returns an int, but we cast to char)
    Serial.print("Read byte:");
    Serial.println(byte);

    // CASE - FRONT LEDS
    set_front_LEDs(byte);

    // CASE - MOTOR CONTROL
    control_motors(byte);
  }
}