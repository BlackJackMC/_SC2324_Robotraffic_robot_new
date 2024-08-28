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
        if (enabled) return;
        Serial.print("Hall: ");
        pinMode(port, INPUT);
        attachInterrupt(digitalPinToInterrupt(port), callback, RISING); //pretty much it
        enabled = true;
        Serial.println("Done");
        return;
    }

    void shutdown()
    {
        detachInterrupt(digitalPinToInterrupt(port));
        enabled = false;
    }
}

#endif