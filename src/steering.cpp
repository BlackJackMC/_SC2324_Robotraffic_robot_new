#include "steering.h"

namespace steering
{
    const int pin = 3;
    Servo servo;

    void setup()
    {
        Serial.print("Servo: ");
        servo.attach(pin);
        Serial.println("Done");
    }

    void turn(int angle)
    {
        servo.write(angle);
    }

    void shutdown()
    {
        servo.detach();
    }
}