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

    // if (net::wifi.connect("robotraffic.lhpsc.click", 443))
    // {
    //     Serial.println("Lmao");
    //     net::wifi.println("GET /ping HTTP/1.1");
    //     net::wifi.println("Host: robotraffic.lhpsc.click");
    //     net::wifi.println("Connection: close");
    //     net::wifi.println();
    // }
    // else
    // {
    //     Serial.println("How");
    // }
    api::setup();
    mqtt::setup();
    car::setup();
    // mqtt::on("control/mode", [&](String message)
    //          { car::setup_mode::current = message; car::setup(); });
}

void loop()
{
    // net::read_response();
    // if (!net::wifi.connected())
    // {
    //     Serial.println();
    //     Serial.println("disconnecting from server.");
    //     net::wifi.stop();

    //     // do nothing forevermore:
    //     while (true)
    //         ;
    // }
    // car::loop();
}