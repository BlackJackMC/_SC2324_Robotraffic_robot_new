#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include "mqtt.h"
#include "hall.h"
#include "motor.h"
#include "line.h"
#include "steering.h"
#include "parameter.h"
#include "loop.h"

namespace setup_sequence
{
    int count = 0; // Track the setup progress, not the proper way but it works
    const int required = 5;
    bool enabled = false;

    void hall(String message)
    {
        if (message == "setup")
        {
            hall::setup(parameter::update_checkpoint);
            count += hall::enabled;
        }
        else if (message == "shutdown")
        {
            count -= hall::enabled;
            hall::shutdown();
        }

        mqtt::client.publish("output/state/hall", String(hall::enabled).c_str());
    }
    void motor(String message)
    {
        if (message == "setup")
        {
            motor::setup();
            count += motor::enabled;
        }
        else if (message == "shutdown")
        {
            count -= motor::enabled;
            motor::shutdown();
        }

        mqtt::client.publish("output/state/motor", String(motor::enabled).c_str());
    }
    void line(String message)
    {
        if (message == "setup")
        {
            line::setup();
            count += line::enabled;
        }
        else if (message == "shutdown")
        {
            count -= line::enabled;
            line::shutdown();
        }
        mqtt::client.publish("output/state/line", String(line::enabled).c_str());
    }
    void steering(String message)
    {
        if (message == "setup")
        {
            steering::setup();
            count += steering::enabled;
        }
        else if (message == "shutdown")
        {
            count -= steering::enabled;
            steering::shutdown();
        }

        mqtt::client.publish("output/state/steering", String(steering::enabled).c_str());
    }
    void parameter(String message)
    {
        if (message == "setup")
        {
            parameter::setup();
            count += parameter::enabled;
        }
        else if (message == "shutdown")
        {
            parameter::shutdown();
            count -= parameter::enabled;
        }

        mqtt::client.publish("output/state/parameter", String(parameter::enabled).c_str());
    }
    void all(String message)
    {
        Serial.println("[car] doing setup");
        hall(message);
        motor(message);
        line(message);
        steering(message);
        parameter(message);
    }
    void setup()
    {
        if (enabled) return;
        
        //Provide some states for setup sequence
        mqtt::on("input/control/hall", hall);
        mqtt::on("input/control/motor", motor);
        mqtt::on("input/control/line", line);
        mqtt::on("input/control/steering", steering);
        mqtt::on("input/control/parameter", parameter);
        mqtt::on("input/control/all", all);
        mqtt::on("input/control/car", [&](String message) 
                 {
                   Serial.println("[mqtt] change car mode");
                   loop_controller::current_mode = message; 
                   loop_controller::current_step = 0;
                   all("shutdown");
                   all("setup"); });

        all("setup"); //Do all setup

        enabled = true;
    }
}

#endif