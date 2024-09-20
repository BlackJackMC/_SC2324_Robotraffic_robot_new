#include "hall.h"

namespace hall
{
    int port = 2;
    CloudVar<bool> enabled("hall");
    CloudVar<bool> command_enable("hall_enable");

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
        enabled.set_type(enabled.READ_ONLY);
        command_enable.set_type(command_enable.READ_WRITE);

        command_enable.set_callback([&](){
            Serial.println("[hall]: Received");
            Serial.println("[hall]: enabled = " + String(command_enable.get()));
            //After enabled has changed, meaning "1" is not enabled and the command is to enable it
            if (command_enable.get() xor enabled.get()) start(callback);
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
