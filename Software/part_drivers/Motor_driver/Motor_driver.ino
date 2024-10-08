#define MotorPin_A 27
#define MotorPin_B 26
int duty_logic = 1;

void setup() {
  Serial.begin(115200);

  //ledcAttachChannel(LED_PIN,LED_FREQ,LED_RES,LED_CH); <-- sketch
  ledcAttachChannel(MotorPin_A,60,8,0);
  ledcAttachChannel(MotorPin_B,60,8,1);
  pinMode(14,INPUT);

  Serial.println("Starting motor test.");

  delay(1000); // Delay 1sec

}

void loop() {
  duty_logic = digitalRead(14);
  if(duty_logic == 0){
    ledcWrite(MotorPin_A,70);
  }else{
    ledcWrite(MotorPin_A,160);
  }
  ledcWrite(MotorPin_B,0);
}
