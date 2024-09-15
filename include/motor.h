#pragma once

#include <Arduino.h>
#include "cloud_variable.h"
namespace motor 
{
    extern int PWMA;
    extern int AIN1;
    extern int AIN2;
    extern int speed;
    extern CloudVar<bool> enabled;

    void setup();
    void stop();
    void shutdown();
    void go(int speed, int direction);
}