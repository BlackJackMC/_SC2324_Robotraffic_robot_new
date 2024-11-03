#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>
#include <Servo.h>

namespace steering
{
    void setup();
    void turn(int angle);
    void shutdown();
}

#endif