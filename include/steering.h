#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>
#include <Servo.h>

namespace steering
{
    const int pin = 6;
    bool enabled = false;
    Servo servo;

    bool setup()
    {
        if (enabled) return false;

        Serial.print("Servo: ");
        servo.attach(pin);
        enabled = true;
        Serial.println("Done");

        return enabled;
    }

    void turn(int angle)
    {
        Serial.println("[servo]: turning " + String(angle) + " degrees");
        servo.write(angle);
    }

    void shutdown()
    {
        servo.detach();
        enabled = false;
    }
}

#endif