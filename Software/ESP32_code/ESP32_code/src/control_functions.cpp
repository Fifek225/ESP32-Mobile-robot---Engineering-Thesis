#include <control_functions.h>
#include <Arduino.h>



int front_led_duty = 80;

int fl_motor_duty = 0;  // variables to store current pwm value sent to each motor
int fr_motor_duty = 0;
int bl_motor_duty = 0;
int br_motor_duty = 0;

int motor_default_duty = 190;
const int motor_max_duty = 250;
char* robot_dir = "stopped";



// Sets the state of fron LEDs
void set_front_LEDs(char byte){
    if(byte == '1'){front_led_duty = 0;} 
    if(byte == '2') {front_led_duty = 80;}
    if(byte == '3'){front_led_duty = 240;}

    ledcWrite(0,front_led_duty);
    ledcWrite(1,front_led_duty);
    delay(50);
    Serial.print("Setting front LED duty cycle to: ");
    Serial.println(front_led_duty);  // Debug print
}

void set_back_LEDs(char byte){}

// ================== MOTOR CONTROLS ==========================================

void control_motors(char byte){
    delay(20);
    if(byte == 'u'){
        robot_dir = "forward";
        set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);
        set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);
    } else if(byte == 'd'){
        robot_dir = "backwards";
        set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);
        set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);
    } else if(byte == 'l'){
        if(robot_dir == "forward"){set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);}
        else if(robot_dir == "backwards"){set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);}
        else if(robot_dir == "stopped"){
            robot_dir = "forward";
            set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);
        }
    } else if(byte == 'r'){
        if(robot_dir == "forward"){set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);}
        else if(robot_dir == "backwards"){set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);}
        else if(robot_dir == "stopped"){
            robot_dir = "forward";
            set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);
        }
    } else if(byte =='s'){
        set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,"stop",fl_motor_duty);
        set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,"stop",fr_motor_duty);

    } else if(byte == 'h'){
        if(fl_motor_duty > fr_motor_duty){set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,"stop",fl_motor_duty);}
        else if(fl_motor_duty < fr_motor_duty){set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,"stop",fr_motor_duty);}
    }
    Serial.print("left motor's duty: "); Serial.println(fl_motor_duty);
    Serial.print("right motor's duty: "); Serial.println(fr_motor_duty);
    Serial.println("============================================");
}

void set_motor(int motor_front_ch, int motor_back_ch, const char* direction, int &duty){
    delay(5);
    if(direction == "forward"){  // Rotate motor so that robot moves forward
        if(duty == 0){duty = motor_default_duty;}
        else if(duty == motor_default_duty){duty = motor_max_duty;}
        ledcWrite(motor_front_ch,duty);
        ledcWrite(motor_back_ch,0);
    } else if(direction == "backwards") { // Rotate motor so that robot moves backwards
        if(duty == 0){duty = motor_default_duty;}
        else if(duty == motor_default_duty){duty = motor_max_duty;}
        ledcWrite(motor_front_ch,0);
        ledcWrite(motor_back_ch,duty);
    } else if(direction == "stop"){
        if(fl_motor_duty == 0 && fr_motor_duty == 0 && bl_motor_duty == 0 && br_motor_duty == 0){robot_dir = "stopped";}
        if(duty == motor_max_duty){duty = motor_default_duty;} else {duty = 0;}
        ledcWrite(motor_front_ch,duty);
        ledcWrite(motor_back_ch,duty);
    }

}