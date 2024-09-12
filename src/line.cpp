#include "setup.h"
#include "line.h"
#include <Arduino.h>
#include <QTRSensors.h>

namespace line
{
    QTRSensors sensor;
    float enabled = 0;

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
            if (i % 20 == 0) 
            {
                enabled = static_cast<float>(i) / 400;
                setup_sequence::send_state();
            }
        }
        Serial.print("<-done ");

        digitalWrite(LED_BUILTIN, LOW);
        enabled = 1.0;
        Serial.println("Done");
    }

    void shutdown()
    {
        enabled = false;
    }
}
