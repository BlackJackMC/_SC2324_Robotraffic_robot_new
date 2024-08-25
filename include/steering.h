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