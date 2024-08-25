#include <Arduino.h>
#include <map>

#include "net.h"
#include "api.h"
#include "mqtt.h"

#include "car.h"

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ; // Wait for serial

    net::setup();
    api::setup();
    mqtt::setup();
    mqtt::on("control/mode", [&](String message)
             { car::current_mode = message; car::setup(); });
}

void loop()
{
    if (car::enabled and parameter::enabled)
        car::loop();
}