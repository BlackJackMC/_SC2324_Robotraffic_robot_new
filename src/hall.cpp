#include "hall.h"
#include "mqtt.h"

namespace hall
{
    int port = 2;
    CloudVar<bool> enabled("hall");
    void setup(voidFuncPtr callback)
    {
        if (enabled.get()) return;
        Serial.print("Hall: ");
        pinMode(port, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(port), callback, RISING); //pretty much it
        enabled.set(true);
        Serial.println("Done");
        return;
    }

    void shutdown()
    {
        detachInterrupt(digitalPinToInterrupt(port));
        enabled.set(false);
    }
}
