#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>
#include <Servo.h>

namespace wheel
{
    const int pin = 7;
    bool enabled = false;
    Servo servo;

    void setup()
    {
        servo.attach(pin);
        enabled = true;
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