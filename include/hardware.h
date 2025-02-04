#pragma once
#include <QTRSensors.h>
#include <Servo.h>
#include <Arduino.h>


namespace servo
{
    void setup();
    void turn(int angle);
    void shutdown();
}

namespace line
{
    extern QTRSensors sensor;

    extern const int count;
    extern const uint8_t emitterPin;
    extern const uint8_t pin[];
    extern uint16_t value[5];

    void setup();
    void shutdown();
}

namespace hall
{   
    void setup(voidFuncPtr callback);
    void shutdown();
}

namespace motor 
{
    extern int speed;

    void setup();
    void stop();
    void shutdown();
    void go(int speed, int direction);
}