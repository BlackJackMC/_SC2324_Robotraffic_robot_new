#ifndef PARAM_H
#define PARAM_H
#include <ArduinoJson.h>
#include <Arduino.h>
#include <PID_v1.h>
#include <map>

#include "api.h"
#include "mqtt.h"
#include "line.h"
#include "steering.h"
namespace parameter
{
    extern bool canGo, enabled;
    extern int current_checkpoint;
    extern const String checkpoint[];
    extern std::map<String, bool> decision;
    extern int speed, direction, angle;

    extern double setpoint, input, output;
    extern double Kp, Ki, Kd;
    extern PID controller;

    void get_all();
    void update_data(String message);
    void update_angle();
    void update_traffic();
    void update_checkpoint();
    void setup();
    void shutdown();
}

#endif