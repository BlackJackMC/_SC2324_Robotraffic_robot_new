#include <Arduino.h>

#include "net.h"
#include "mqtt.h"
#include "cloud.h"

#include "hall.h"
#include "line.h"
#include "motor.h"
#include "steering.h"

#include "parameter.h"

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
    
    hall::setup(parameter::update_magnetic);
    line::setup();
    motor::setup();
    steering::setup();

    mqtt::publish("serial", "car setup completed");
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

