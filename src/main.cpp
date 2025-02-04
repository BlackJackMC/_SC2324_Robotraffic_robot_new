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

    // line::setup();
    // servo::setup();
    // hall::setup(controller::update_hall);
    // motor::setup();

    // traffic_controller::setup();
    controller::setup();
    properties::setup();
}

void loop()
{
    static bool state = 0;
    static unsigned long last = 0;
    unsigned long current = millis();
    if (current - last >= 500)
    {
        state = !state;
        last = current;
    }
    digitalWrite(LED_BUILTIN, state);
    net::loop();
    properties::loop();
    // controller::loop();
}

