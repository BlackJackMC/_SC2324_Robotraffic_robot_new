#ifndef PARAM_H
#define PARAM_H
#include <ArduinoJson.h>
#include <Arduino.h>
#include <PID_v1.h>
#include <map>

#include "mqtt.h"
#include "line.h"
#include "steering.h"
#include "motor.h"
#include "traffic_controller.h"

#include "properties.h"

namespace controller
{  
    void update_angle();
    void update_hall();
    void setup();
    void loop();
}

#endif