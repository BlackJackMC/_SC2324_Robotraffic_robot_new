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

namespace controller
{  
    extern int speed;
    extern int direction;
    extern float P;
    extern float I;
    extern float D;
    extern int angle;
    extern float input;
    extern float output;
    extern int setpoint;

    void update_angle();
    void update_hall();
    void setup();
    void shutdown();
    void loop();
}

#endif