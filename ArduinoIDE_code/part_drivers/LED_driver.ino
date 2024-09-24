#define LED_PIN 4
#define LED_CH 0
#define LED_FREQ 1000 //Hz
#define LED_RES 8


void setup(){
  ledcAttachChannel(LED_PIN,LED_FREQ,LED_RES,LED_CH);
}

void loop(){
  LED_control();
}

void LED_control(){
  for(int duty = 0; duty <= 80; duty = duty + 10){
    ledcWrite(LED_PIN,duty);
    delay(100);
  }
  for(int duty = 80; duty > 0; duty = duty - 10){
    ledcWrite(LED_PIN,duty);
    delay(100);
  }
}