#include "steering.h"

namespace steering
{
    const int pin = 3;
    CloudVar<bool> enabled("servo", Permission::Read);
    CloudVar<String> command("servo_enable");
    Servo servo;

    void start()
    {
        if (enabled)
            return;

        Serial.print("Servo: ");
        servo.attach(pin);
        enabled = true;
        Serial.println("Done");
    }

    void setup()
    {
        command.set_callback([&](){
            if (command == "ON" and !enabled) start();
            else if (command == "OFF" and enabled) shutdown();
        });
        start();
    }

    void turn(int angle)
    {
        // Serial.println("[servo]: turning " + String(angle) + " degrees");
        servo.write(angle);
    }

    void shutdown()
    {
        servo.detach();
        enabled = false;
    }
}