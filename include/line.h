#ifndef LINE_H
#define LINE_H

#include <Arduino.h>
#include <QTRSensors.h>
#include "cloud_var.h"

namespace line
{
    extern QTRSensors sensor;
    extern CloudVar<float> enabled;

    extern const int count;
    extern const uint8_t emitterPin;
    extern const uint8_t pin[];
    extern uint16_t value[5];

    void setup();
    void shutdown();
}
#endif