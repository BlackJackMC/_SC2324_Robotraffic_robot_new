//For hall sensor
#ifndef HALL_H
#define HALL_H

#include <Arduino.h>
#include <functional>

namespace hall
{
    int port = 2;
    bool enabled = false;

    void setup(voidFuncPtr callback)
    {
        pinMode(port, INPUT);
        attachInterrupt(digitalPinToInterrupt(2), callback, RISING); //pretty much it
        enabled = true;
    }
}

#endif