//For motor
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
namespace motor 
{
    int PWMA = 3;
    int AIN1 = 4;
    int AIN2 = 6;
    int speed = 0;
    bool enabled = false; //For debugging

    void setup()
    {
        if (enabled) return;
        Serial.print("Motor: ");
        pinMode(PWMA, OUTPUT);
        pinMode(AIN1, OUTPUT);
        pinMode(AIN2, OUTPUT);
        enabled = true;
        Serial.println("Done");
    }

    void stop() {analogWrite(PWMA, 0);}

    void shutdown()
    {
        stop();
        enabled = false;
    }

    void go(int speed, int direction = 1)
    {
        if (!enabled) return;
        bool pin1 = (direction == 1);
        bool pin2 = !(direction == 1);

        speed = constrain(speed, 0, 255);

        digitalWrite(AIN1, pin1);
        digitalWrite(AIN2, pin2);
        analogWrite(PWMA, speed);
    }
}

#endif