#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>
#include <Servo.h>

namespace steering
{
    extern const int pin;
    extern bool enabled;
    extern Servo servo;

    void setup();
    void turn(int angle);
    void shutdown();
}

#endif