#include <Arduino.h>

#include "net.h"
#include "mqtt.h"
#include "cloud.h"

#include "hall.h"
#include "line.h"
#include "motor.h"
#include "steering.h"

#include "controller.h"

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
    hall::setup(controller::update_hall);
    line::setup();
    motor::setup();
    steering::setup();
    controller::setup();
    cloud::setup();
}

void loop()
{
    net::loop();
    mqtt::loop();
    cloud::loop();
    controller::loop();
}

