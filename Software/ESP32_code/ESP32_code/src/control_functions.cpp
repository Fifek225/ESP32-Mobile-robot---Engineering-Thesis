#include <control_functions.h>
#include <Arduino.h>

std::map<int,char*> esp32_commands_map = {

};

int front_led_duty = 80;


// Sets the state of fron LEDs
void set_front_LEDs(char byte){
    if(byte == '1'){front_led_duty = 0;} 
    if(byte == '2') {front_led_duty = 80;}
    if(byte == '3'){front_led_duty = 200;}

    ledcWrite(0,front_led_duty);
    ledcWrite(1,front_led_duty);
    delay(50);
    Serial.print("Setting front LED duty cycle to: ");
    Serial.println(front_led_duty);  // Debug print
}

void set_back_LEDs(char byte){}

void read_motor_inputs(char byte){
    if(byte == 0){}
}

void set_motor(int motor_pin,char direction, int duty){}