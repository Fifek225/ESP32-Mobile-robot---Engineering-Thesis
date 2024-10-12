#ifndef CONTROL_FUNCTIONS_H
#define CONTROL_FUNCTIONS_H

#include <map>

#define FRONT_LED_PIN_1 14
#define FRONT_LED_PIN_2 15
#define LED_FREQUENCY 100

#define FL_MOTOR_PIN_1
#define FL_MOTOR_PIN_2
#define FR_MOTOR_PIN_1
#define FR_MOTOR_PIN_2


extern std::map<int,char*> esp32_commands_map;

extern int front_led_duty;

void set_front_LEDs(char byte); // Sets the state of front LEDs

void set_back_LEDs(char byte); // Sets the state of back LEDs
 
void read_motor_inputs(char byte); // Checks for arrow key inputs that control motors

void set_motor(int motor_pin,char direction, int duty); // Sets given motor's direction and speed (duty)



#endif