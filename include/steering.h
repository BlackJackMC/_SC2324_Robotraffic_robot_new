#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>
#include <Servo.h>

namespace steering
{
    const int pin = 7;
    bool enabled = false;
    Servo servo;

    void setup()
    {
        Serial.print("Servo: ");
        servo.attach(pin);
        enabled = true;
        Serial.println("Done");
    }

    void turn(int angle)
    {
        servo.write(angle);
    }

    void shutdown()
    {
        servo.detach();
        enabled = false;
    }
}

#endif