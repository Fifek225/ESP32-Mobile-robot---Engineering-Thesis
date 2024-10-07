#include <ESP32Servo.h>

#define CAMERA_SERVO_PIN 13

Servo cam_servo;
int servo_pos = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  cam_servo.attach(CAMERA_SERVO_PIN);
  cam_servo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  cam_servo.write(90);
  servo_pos = 90;
  delay(500);
  Serial.print("Servo position: ");Serial.println(servo_pos);
  cam_servo.write(160);
  servo_pos = 90;
  Serial.print("Servo position: ");Serial.println(servo_pos);
  delay(500);
}
