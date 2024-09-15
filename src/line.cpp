#include "line.h"
#include "mqtt.h"

namespace line
{
    QTRSensors sensor;
    CloudVar<float> enabled("line");

    const int count = 5;
    const uint8_t emitterPin = 13;
    const uint8_t pin[] = {8, 9, 10, 11, 12};
    uint16_t value[5];

    void setup()
    {
        if (enabled.get() == 1.0)
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
                enabled.set(static_cast<float>(i) / 400);
            }
        }
        Serial.print("<-done ");

        digitalWrite(LED_BUILTIN, LOW);
        enabled.set(1.0);
        Serial.println("Done");
    }

    void shutdown()
    {
        enabled = false;
    }
}
