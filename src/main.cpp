#include <Arduino.h>
#include <map>

#include "net.h"
#include "api.h"
#include "mqtt.h"

#include "car.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ; // Wait for serial
    // randomSeed(micros());
    Serial.println("Serial");
    net::setup();
    api::setup();
    mqtt::setup();
    mqtt::client.publish("serial", "Network setup completed");
    car::setup();
    mqtt::client.publish("serial", "Car setup completed");
}

void loop()
{
    mqtt::client.loop();
    car::loop();
}