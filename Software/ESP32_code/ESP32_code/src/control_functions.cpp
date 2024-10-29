#include <control_functions.h>


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

long front_duration, front_distance;
char front_distance_buff[40];
int front_distance_len = 0;

long back_duration, back_distance;
char back_distance_buff[40];
int back_distance_len = 0;


// Sets the state of fron LEDs
void set_front_LEDs(char byte){
    if(byte == '1'){front_led_duty = 0; delay(5);} 
    if(byte == '2') {front_led_duty = 80;delay(5);}
    if(byte == '3'){front_led_duty = 240;delay(5);}

    ledcWrite(0,front_led_duty);
    //ledcWrite(1,front_led_duty);

}

void set_back_LEDs(char byte){}

// ================== MOTOR CONTROLS ==========================================

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


void get_distance(WiFiClient client) {
    // Trigger the ultrasonic sensor
    //digitalWrite(FRONT_DISTANCE_SENSOR_TRIG_PIN, LOW);
    digitalWrite(BACK_DISTANCE_SENSOR_TRIG_PIN, LOW);
    delayMicroseconds(5);
    //digitalWrite(FRONT_DISTANCE_SENSOR_TRIG_PIN, HIGH);
    digitalWrite(BACK_DISTANCE_SENSOR_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    //digitalWrite(FRONT_DISTANCE_SENSOR_TRIG_PIN, LOW);
    digitalWrite(BACK_DISTANCE_SENSOR_TRIG_PIN, LOW);
    //pinMode(FRONT_DISTANCE_SENSOR_ECHO_PIN, INPUT);
    pinMode(BACK_DISTANCE_SENSOR_ECHO_PIN, INPUT);

    //front_duration = pulseIn(FRONT_DISTANCE_SENSOR_ECHO_PIN, HIGH);
    //front_distance = (front_duration / 2) * SOUND_SPEED;
    back_duration = pulseIn(BACK_DISTANCE_SENSOR_ECHO_PIN, HIGH);
    back_distance = (back_duration / 2) * SOUND_SPEED;

    // front_distance_len = snprintf(front_distance_buff, sizeof(front_distance_buff), "FD: %ld\n", front_distance);
    // if (client.connected() && front_distance_len > 0) {
    //     client.write((const uint8_t*)front_distance_buff, front_distance_len);
    // }

    back_distance_len = snprintf(back_distance_buff, sizeof(back_distance_buff), "BD: %ld\n", back_distance);
    if (client.connected() && back_distance_len > 0) {
       client.write((const uint8_t*)back_distance_buff, back_distance_len);
       Serial.print("Back sensor reading: "); Serial.println(back_distance_len);
    }
    
    delay(5);
}