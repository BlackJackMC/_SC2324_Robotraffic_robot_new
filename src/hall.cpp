#include "hall.h"

namespace hall
{
    int port = 2;
    CloudVar<bool> enabled("hall");

    void start(voidFuncPtr callback)
    {
        Serial.print("[hall]: ");
        pinMode(port, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(port), callback, RISING); //pretty much it
        enabled = true;
        Serial.println("Done");
    }

    void setup(voidFuncPtr callback)
    {
        enabled.set_callback([&](){
            Serial.println("[hall]: Received");
            Serial.println("[hall]: enabled = " + String(enabled.get()));
            //After enabled has changed, meaning "1" is not enabled and the command is to enable it
            if (enabled) start(callback);
            else shutdown();
        });
        start(callback);
        return;
    }

    void shutdown()
    {
        detachInterrupt(digitalPinToInterrupt(port));
        enabled = false;
    }
}
