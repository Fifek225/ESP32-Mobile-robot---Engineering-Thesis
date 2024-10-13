#include <control_functions.h>
#include <Arduino.h>

std::map<char*,char> esp32_commands_map = {

};

int front_led_duty = 80;

int fl_motor_duty = 0;  // variables to store current pwm value sent to each motor
int fr_motor_duty = 0;
int bl_motor_duty = 0;
int br_motor_duty = 0;

const int max_motor_duty = 200;



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

void control_motors(char byte){
    delay(20);
    if(byte == 'u'){
        fl_motor_duty = 150;
        set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,"forward",fl_motor_duty);
    } else if(byte == 'd'){
        fl_motor_duty = 150;
        set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,"backwards",fl_motor_duty);
    } else if(byte == 'l'){
        if(fl_motor_duty < max_motor_duty){fl_motor_duty += 50;} else {fl_motor_duty = max_motor_duty;}
        set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,"forwards",fl_motor_duty);
    } else if(byte =='s'){
        fl_motor_duty = 0;
        set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,"stop",fl_motor_duty);
    }
}

void set_motor(int motor_front_ch, int motor_back_ch, char* direction, int duty){
    if(direction == "forward"){  // Rotate motor so that robot moves forward
        delay(10);
        ledcWrite(motor_front_ch,duty);
        ledcWrite(motor_back_ch,0);
    } else if(direction == "backwards") { // Rotate motor so that robot moves backwards
        delay(10);
        ledcWrite(motor_front_ch,0);
        ledcWrite(motor_back_ch,duty);
    } else if(direction == "stop"){
        delay(10);
        ledcWrite(motor_front_ch,0);
        ledcWrite(motor_back_ch,0);
    }
}