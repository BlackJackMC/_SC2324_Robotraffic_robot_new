#pragma once

#include <Arduino.h>
namespace motor 
{
    extern int PWMA;
    extern int AIN1;
    extern int AIN2;
    extern int speed;

    void setup();
    void stop();
    void shutdown();
    void go(int speed, int direction);
}