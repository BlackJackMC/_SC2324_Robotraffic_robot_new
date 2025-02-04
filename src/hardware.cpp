#include "hardware.h"

namespace servo
{
    const int pin = 3;
    Servo servo;

    void setup()
    {
        Serial.print("[servo]: ");
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

namespace line
{
    QTRSensors sensor;

    const int count = 5;
    const uint8_t emitterPin = 13;
    const uint8_t pin[] = {8, 9, 10, 11, 12};
    uint16_t value[5];

    void setup()
    {
        Serial.print("[line sensor]: ");
        sensor.setTypeRC();
        sensor.setSensorPins(pin, count);
        sensor.setEmitterPin(emitterPin);

        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print("calibrating ");
        for (int i = 0; i < 400; i++)
            sensor.calibrate();

        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("Done");
    }
}

namespace motor
{
    const int PWMA = 5;
    const int AIN1 = 6;
    const int AIN2 = 7;

    void setup()
    {
        Serial.print("[motor]: ");
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

namespace hall
{
    int pin = 2;

    void setup(voidFuncPtr callback)
    {
        pinMode(pin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin), callback, RISING);
        return;
    }

    void shutdown()
    {
        detachInterrupt(digitalPinToInterrupt(pin));
    }
}