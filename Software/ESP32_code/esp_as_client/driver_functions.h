#ifndef driver_functions_h
#define driver_functions_h

void setFrontLED(char* mode); // Sets front lights to: OFF, 60% duty, 90% duty

void setBackLED(char* mode); // Sets back lights to: OFF, 60% duty, 90% duty

void readDistance(); // Converts and display data read from HCSR04 distance sensor

void setMotor(char* motor,int duty); // Sets given motor's pwm width to duty

#endif