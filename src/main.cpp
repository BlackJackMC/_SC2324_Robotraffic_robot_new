#include <Arduino.h>
#include <map>

#include "net.h"
#include "mqtt.h"
#include "setup.h"
#include "parameter.h"
#include "motor.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ; // Wait for serial
    randomSeed(micros());
    Serial.println("Serial");
    net::setup();
    mqtt::client.publish("serial", "connected to internet");
    mqtt::setup();
    mqtt::client.publish("serial", "network setup completed");
    setup_sequence::setup();
}

void loop()
{
    if (!mqtt::client.connected())
        mqtt::connect();

    mqtt::client.loop();
    if (parameter::can_go.get())
    {
        parameter::update_angle();
        motor::go(parameter::speed.get(), parameter::direction.get());
    }
    else
        motor::stop();
}