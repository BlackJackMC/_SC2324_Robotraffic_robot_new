#include <Arduino.h>

#include "net.h"
#include "mqtt.h"
#include "cloud.h"
#include "setup.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ; // Wait for serial
    Serial.println();
    Serial.println(F("Serial"));
    randomSeed(micros());
    net::setup();
    mqtt::setup();
    mqtt::publish("serial", "network setup completed");
    setup_sequence::setup();
}

void loop()
{
    if (!net::wifi.connected())
        net::setup();
    if (!mqtt::client.connected())
        mqtt::setup();

    cloud::loop();
    mqtt::client.loop();
}

