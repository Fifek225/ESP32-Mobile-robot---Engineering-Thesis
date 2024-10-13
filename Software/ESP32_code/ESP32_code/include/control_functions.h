#ifndef CONTROL_FUNCTIONS_H
#define CONTROL_FUNCTIONS_H


#define FRONT_LED_PIN_1 14  //Definitions of front LED pins and basic parameters
#define FRONT_LED_PIN_2 15
#define LED_FREQUENCY 100

#define FL_MOTOR_PIN_1 26     // Definitions of pins that will send PWM to their respective motor 
#define FL_MOTOR_CH_1 4
#define FL_MOTOR_PIN_2 27
#define FL_MOTOR_CH_2 5

#define FR_MOTOR_PIN_1 12    // Definitions of pins that will send PWM to their respective motor 
#define FR_MOTOR_CH_1 6
#define FR_MOTOR_PIN_2 13
#define FR_MOTOR_CH_2 7

#define BL_MOTOR_PIN_1      // Definitions of pins that will send PWM to their respective motor 
#define BL_MOTOR_CH_1 8
#define BL_MOTOR_PIN_2
#define BL_MOTOR_CH_2 9

#define BR_MOTOR_PIN_1      // Definitions of pins that will send PWM to their respective motor 
#define BR_MOTOR_CH_1 10
#define BR_MOTOR_PIN_2
#define BR_MOTOR_CH_2 11



extern int front_led_duty;

void set_front_LEDs(char byte); // Sets the state of front LEDs

void set_back_LEDs(char byte); // Sets the state of back LEDs
 
void control_motors(char byte); // Checks for arrow key inputs that control motors and uses set_motor function accordingly to user's inputs

void set_motor(int motor_front_ch, int motor_back_ch, const char* direction, int &duty); // Sets given motor's direction and speed (duty)



#endif