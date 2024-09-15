#include <Arduino.h>

#include "net.h"
#include "mqtt.h"
#include "setup.h"
#include "parameter.h"
#include "motor.h"

void setup()
{
    // digitalWrite(LED_BUILTIN, HIGH);
    // Serial.begin(115200);
    // while (!Serial)
    //     ; // Wait for serial
    // Serial.println("Serial");
    // net::setup();
    // mqtt::client.publish("serial", "connected to internet");
    // mqtt::setup();
    // mqtt::client.publish("serial", "network setup completed");
    // setup_sequence::setup();
    // digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    // if (!mqtt::client.connected())
    //     mqtt::connect();

    // mqtt::client.loop();
    // if (parameter::can_go.get())
    // {
    //     parameter::update_angle();
    //     motor::go(parameter::speed.get(), parameter::direction.get());
    // }
    // else
    //     motor::stop();
}