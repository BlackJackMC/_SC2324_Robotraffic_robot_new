#include "motor.h"

namespace motor
{
    int PWMA = 5;
    int AIN1 = 6;
    int AIN2 = 7;

    void setup()
    {
        Serial.print("Motor: ");
        pinMode(PWMA, OUTPUT);
        pinMode(AIN1, OUTPUT);
        pinMode(AIN2, OUTPUT);
        Serial.println("Done");
    }

    void stop() { analogWrite(PWMA, 0); }

    void shutdown()
    {
        stop();
    }

    void go(int speed, int direction = 1)
    {
        bool pin1 = (direction == 1);
        bool pin2 = !(direction == 1);

        speed = constrain(speed, 0, 255);

        digitalWrite(AIN1, pin1);
        digitalWrite(AIN2, pin2);
        analogWrite(PWMA, speed);
    }
}