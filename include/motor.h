#pragma once

#include <Arduino.h>
#include "cloud_var.h"
namespace motor 
{
    extern int PWMA;
    extern int AIN1;
    extern int AIN2;
    extern int speed;
    extern CloudVar<bool> enabled;
    extern CloudVar<String> command;

    void setup();
    void stop();
    void shutdown();
    void go(int speed, int direction);
}