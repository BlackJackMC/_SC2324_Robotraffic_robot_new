#include <Arduino.h>
#include "properties.h"
#include "hardware.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    randomSeed(micros());

    net::setup();
    mqtt::setup();

    hardware::setup();
    controller::setup();
    properties::setup();
}

void loop()
{
    properties::loop();
    controller::loop();
}

