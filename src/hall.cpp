#include "hall.h"
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
        pinMode(port, INPUT_PULLUP);
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
