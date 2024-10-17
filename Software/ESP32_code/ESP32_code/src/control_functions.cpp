#include <control_functions.h>
#include <Arduino.h>



int front_led_duty = 80;

int fl_motor_duty = 0;  // variables to store current pwm value sent to each motor
int fr_motor_duty = 0;
int bl_motor_duty = 0;
int br_motor_duty = 0;


// Variables to track current state of motors
const int motor_max_duty = 250;
const int motor_mid_duty = 190;
char* robot_dir_horizontal = "none";
char* robot_dir_vertical = "none";



// Sets the state of fron LEDs
void set_front_LEDs(char byte){
    if(byte == '1'){front_led_duty = 0; delay(5);} 
    if(byte == '2') {front_led_duty = 80;delay(5);}
    if(byte == '3'){front_led_duty = 240;delay(5);}

    ledcWrite(0,front_led_duty);
    ledcWrite(1,front_led_duty);

}

void set_back_LEDs(char byte){}

// ================== MOTOR CONTROLS ==========================================

// void control_motors(char byte){
//     delay(5);
//     if(byte == 'u'){
//         robot_dir = "forward";
//         set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);
//         set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);
//         set_motor(BL_MOTOR_CH_1,BL_MOTOR_CH_2,robot_dir,bl_motor_duty);
//         set_motor(BR_MOTOR_CH_1,BR_MOTOR_CH_2,robot_dir,br_motor_duty);
//     } else if(byte == 'd'){
//         robot_dir = "backwards";
//         set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);
//         set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);
//     } else if(byte == 'l'){
//         if(robot_dir == "forward"){
//             if(fl_motor_duty == fr_motor_duty == 0){
//                 set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);
//             }
//         }
//         else if(robot_dir == "backwards"){set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);}
//         else if(robot_dir == "stopped"){
//             robot_dir = "forward";
//             set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir,fr_motor_duty);
//         }
//     } else if(byte == 'r'){
//         if(robot_dir == "forward"){set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);}
//         else if(robot_dir == "backwards"){set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);}
//         else if(robot_dir == "stopped"){
//             robot_dir = "forward";
//             set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir,fl_motor_duty);
//         }
//     } else if(byte =='s'){
//         set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,"stop",fl_motor_duty);
//         set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,"stop",fr_motor_duty);
//         set_motor(BL_MOTOR_CH_1,BL_MOTOR_CH_2,"stop",bl_motor_duty);
//         set_motor(BR_MOTOR_CH_1,BR_MOTOR_CH_2,"stop",br_motor_duty);
//     } else if(byte == 'h'){
//         if(fl_motor_duty > fr_motor_duty){set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,"stop",fl_motor_duty);}
//         else if(fl_motor_duty < fr_motor_duty){set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,"stop",fr_motor_duty);}
//     }
//     Serial.print("left motor's duty: "); Serial.println(fl_motor_duty);
//     Serial.print("right motor's duty: "); Serial.println(fr_motor_duty);
//     Serial.println("============================================");
// }

// void set_motor(int motor_front_ch, int motor_back_ch, const char* direction, int &duty){
//     if(direction == "forward"){  // Rotate motor so that robot moves forward
//         if(duty == 0){duty = motor_max_duty;}
//         else if(duty == motor_max_duty){duty = motor_mid_duty;}
//         Serial.print("Inside set_motor f");Serial.println(duty);
//         ledcWrite(motor_front_ch,duty);
//         ledcWrite(motor_back_ch,0);
//     } else if(direction == "backwards") { // Rotate motor so that robot moves backwards
//         if(duty == 0){duty = motor_max_duty;}
//         else if(duty == motor_max_duty){duty = motor_mid_duty;}
//         Serial.print("Inside set_motor b");Serial.println(duty);
//         ledcWrite(motor_front_ch,0);
//         ledcWrite(motor_back_ch,duty);
//     } else if(direction == "stop"){
//         if(fl_motor_duty == 0 && fr_motor_duty == 0 && bl_motor_duty == 0 && br_motor_duty == 0){robot_dir = "stopped";}
//         if(duty == motor_mid_duty){duty = motor_max_duty;} else {duty = 0;}
//         Serial.print("Inside set_motor st");Serial.println(duty);
//         if(robot_dir == "forward"){
//             ledcWrite(motor_front_ch,duty);
//             ledcWrite(motor_back_ch,0);
//         } else if(robot_dir == "backwards"){
//             ledcWrite(motor_front_ch,0);
//             ledcWrite(motor_back_ch,duty);
//         }

//     }

// }

void control_motors(char byte){
    delay(10);
    if(fl_motor_duty == 0 && fr_motor_duty == 0){robot_dir_vertical = "none"; robot_dir_horizontal = "none";}
    if(byte == 'u' && robot_dir_vertical != "backwards"){
        robot_dir_vertical = "forward";
        if(robot_dir_horizontal == "none"){ // If Only ArrowUp is held down
            set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);
            set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);
        } else if(robot_dir_horizontal == "left"){
            set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);
        } else if(robot_dir_horizontal == "right"){
            set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);
        }
    } else if(byte == 'd' && robot_dir_vertical != "forward"){
        robot_dir_vertical = "backwards";
        if(robot_dir_horizontal == "none"){ // If Only ArrowDown is held down
            set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);
            set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);
        } else if(robot_dir_horizontal == "left"){
            set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);
        } else if(robot_dir_horizontal == "right"){
            set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);
        }
    } else if(byte == 'l'){
        if(robot_dir_vertical == "none"){ // If robot only ArrowLeft is pressed
            robot_dir_vertical = "forward"; // if we want to start movement  by turning it will default to going forward
            set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);
            robot_dir_horizontal = "left";
        } else { // If robot was already moving vertically 
            robot_dir_horizontal = "left";
            set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);
        } 
    } else if(byte == 'r'){
        if(robot_dir_vertical == "none"){ // If robot only ArrowLeft is pressed
            robot_dir_vertical = "forward"; // if we want to start movement  by turning it will default to going forward
            set_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);
            robot_dir_horizontal = "right";
        } else { // If robot was already moving vertically 
            robot_dir_horizontal = "right";
            set_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);
        } 
    } else if(byte == 's'){
        stop_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);
        stop_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);
    } else if(byte == 'h'){
        if(robot_dir_horizontal == "left"){ // ArrowUp/Down was not being held down
            robot_dir_horizontal = "none";
            if(fl_motor_duty ==  0){ stop_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);}
            else if(fl_motor_duty == motor_mid_duty){stop_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);}
        } else if(robot_dir_horizontal == "right"){
            robot_dir_horizontal = "none";
            if(fr_motor_duty ==  0){ stop_motor(FL_MOTOR_CH_1,FL_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fl_motor_duty);}
            else if(fr_motor_duty == motor_mid_duty){stop_motor(FR_MOTOR_CH_1,FR_MOTOR_CH_2,robot_dir_vertical,robot_dir_horizontal,fr_motor_duty);}
        }
    }
    Serial.print("Front left pwm: "); Serial.println(fl_motor_duty);
    Serial.print("Front right pwm: "); Serial.println(fr_motor_duty);
    if(fl_motor_duty == 0 && fr_motor_duty == 0){robot_dir_vertical = "none"; robot_dir_horizontal = "none";}
    Serial.print("Horizontal direction: "); Serial.println(robot_dir_horizontal);
    Serial.print("Vertical direction: "); Serial.println(robot_dir_vertical);
    Serial.println("==========================================");
}

void set_motor(int motor_front_ch, int motor_back_ch, const char* dir_vertical, const char* dir_horizontal, int &duty){ // used for ArrowKey pressed events
    if(dir_horizontal != "none"){duty = motor_mid_duty;} // if ArrowLeft/Right was pressed before this call
    else if(dir_horizontal == "none"){duty = motor_max_duty;} // if ArrowLeft/Right was not pressed before this call
    
    if(dir_vertical == "forward"){
        ledcWrite(motor_front_ch,duty);
        ledcWrite(motor_back_ch,0);
    } else if(dir_vertical == "backwards"){
        ledcWrite(motor_front_ch,0);
        ledcWrite(motor_back_ch,duty);
    }
}

void stop_motor(int motor_front_ch, int motor_back_ch, const char* dir_vertical, const char* dir_horizontal, int &duty){ // used for ArrowKey released events
    if(dir_horizontal == "none"){ // if this call is a result of releasing ArrowLeft/Right || releasing ArrowUp/Down when it was the only key held down
        if(duty == motor_max_duty){duty = 0;}
        else if(duty == motor_mid_duty){duty = motor_max_duty;}
    } else if(dir_horizontal != "none"){ // if this call is a result of releasing ArrowUp/Down while keeping ArrowLeft/Right held down
        if(duty == motor_mid_duty){duty = 0;}     
    }

    if(dir_vertical == "forward"){
        ledcWrite(motor_front_ch,duty);
        ledcWrite(motor_back_ch,0);
    } else if(dir_vertical == "backwards"){
        ledcWrite(motor_front_ch,0);
        ledcWrite(motor_back_ch,duty);
    }
}