#include "hall.h"

namespace hall
{
    int port = 2;
    CloudVar<bool> enabled("hall", Permission::Read);
    CloudVar<String> command("hall_enable", Permission::ReadWrite);

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
        command.set_callback([&](){
            Serial.println("[hall]: Received " + String(command));
            if (command == "ON" and !enabled) start(callback);
            else if (command == "OFF" and enabled) shutdown();
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
