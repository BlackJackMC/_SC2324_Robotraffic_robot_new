#include "steering.h"
#include <Arduino.h>
#include <Servo.h>

namespace steering
{
    const int pin = 3;
    bool enabled = false;
    Servo servo;

    void setup()
    {
        if (enabled)
            return;

        Serial.print("Servo: ");
        servo.attach(pin);
        enabled = true;
        Serial.println("Done");
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