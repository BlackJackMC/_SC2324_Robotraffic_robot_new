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
    Serial.println(F("Serial"));
    net::setup();
    mqtt::setup();
    mqtt::publish("serial", "network setup completed");
    setup_sequence::setup();
}

void loop()
{
    cloud::push();
    if (!mqtt::client.connected())
        mqtt::setup();

    mqtt::client.loop();
}