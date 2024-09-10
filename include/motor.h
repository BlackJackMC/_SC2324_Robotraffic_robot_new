//For motor
#ifndef MOTOR_H
#define MOTOR_H

namespace motor 
{
    extern int PWMA;
    extern int AIN1;
    extern int AIN2;
    extern int speed;
    extern bool enabled;

    void setup();
    void stop();
    void shutdown();
    void go(int speed, int direction);
}

#endif