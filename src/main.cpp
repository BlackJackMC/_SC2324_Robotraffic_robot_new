#include <Arduino.h>
#include "properties.h"
#include "hardware.h"
#include "traffic_controller.h"
#include "controller.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    randomSeed(micros());

    net::setup();
    mqtt::setup();

    line::setup();
    servo::setup();
    hall::setup(controller::update_hall);
    motor::setup();

    traffic_controller::setup();
    controller::setup();
    properties::setup();
}

void loop()
{
    properties::loop();
    controller::loop();
}

