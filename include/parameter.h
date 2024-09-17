#ifndef PARAM_H
#define PARAM_H
#include <ArduinoJson.h>
#include <Arduino.h>
#include <PID_v1.h>
#include <map>
#include "cloud_var.h"

#include "mqtt.h"
#include "line.h"
#include "steering.h"
namespace parameter
{  
    extern CloudVar<bool> can_go;
    extern CloudVar<int> speed;
    extern CloudVar<int> direction;
    extern CloudVar<float> P;
    extern CloudVar<float> I;
    extern CloudVar<float> D;
    extern CloudVar<int> angle;
    extern CloudVar<float> input;
    extern CloudVar<float> output;
    extern CloudVar<int> setpoint;
    extern CloudVar<int> checkpoint;

    void update_angle();
    void update_magnetic();
    void setup();
    void shutdown();
}

#endif