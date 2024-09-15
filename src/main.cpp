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
    mqtt::setup();
    mqtt::client.publish("serial", "Network setup completed");
    setup_sequence::setup();
    mqtt::client.publish("serial", "Car setup completed");
}

void loop()
{
    if (!mqtt::client.connected())
    {
        mqtt::connect();
    }
    mqtt::client.loop();
    
    if (parameter::can_go)
    {
        parameter::update_angle();
        motor::go(parameter::speed, parameter::direction);
    }
    else
        motor::stop();
}