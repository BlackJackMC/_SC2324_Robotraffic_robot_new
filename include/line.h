#ifndef LINE_H
#define LINE_H

#include <Arduino.h>
#include <QTRSensors.h>

namespace line
{
    QTRSensors sensor;
    bool enabled = false;

    const int count = 5;
    const uint8_t pin[] = {9, 10, 11, 12, 13};
    uint16_t value[5];

    void setup()
    {
        sensor.setTypeRC();
        sensor.setSensorPins(pin, count);

        digitalWrite(LED_BUILTIN, HIGH);

        for (int i = 0; i < 400; i++)
        {
            sensor.calibrate();
        }

        digitalWrite(LED_BUILTIN, LOW);
        enabled = true;
    }
}

#endif