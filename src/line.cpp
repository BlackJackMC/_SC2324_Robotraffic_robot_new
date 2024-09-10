#include "line.h"
#include <Arduino.h>
#include <QTRSensors.h>

namespace line
{
    QTRSensors sensor;
    bool enabled = false;

    const int count = 5;
    const uint8_t emitterPin = 13;
    const uint8_t pin[] = {8, 9, 10, 11, 12};
    uint16_t value[5];

    void setup()
    {
        if (enabled)
            return;
        Serial.print("Line: ");
        sensor.setTypeRC();
        sensor.setSensorPins(pin, count);
        sensor.setEmitterPin(emitterPin);

        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print("calibrating ");
        for (int i = 0; i < 400; i++)
        {
            sensor.calibrate();
        }
        Serial.print("<-done ");

        digitalWrite(LED_BUILTIN, LOW);
        enabled = true;
        Serial.println("Done");
    }

    void shutdown()
    {
        enabled = false;
    }
}
