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
        else
        {
            count -= hall::enabled;
            hall::shutdown();
        }

        mqtt::client.publish("control/hall", String(hall::enabled).c_str());
    }
    void motor(String message)
    {
        if (message == "setup")
        {
            motor::setup();
            count += motor::enabled;
        }
        else
        {
            count -= motor::enabled;
            motor::shutdown();
        }

        mqtt::client.publish("control/motor", String(motor::enabled).c_str());
    }
    void line(String message)
    {
        if (message == "setup")
        {
            line::setup();
            count += line::enabled;
        }
        else
        {
            count -= line::enabled;
            line::shutdown();
        }
        mqtt::client.publish("control/line", String(line::enabled).c_str());
    }
    void steering(String message)
    {
        if (message == "setup")
        {
            steering::setup();
            count += steering::enabled;
        }
        else
        {
            count -= steering::enabled;
            steering::shutdown();
        }

        mqtt::client.publish("control/steering", String(steering::enabled).c_str());
    }
    void parameter(String message)
    {
        if (message == "setup")
        {
            parameter::setup();
            count += parameter::enabled;
        }
        else
        {
            parameter::shutdown();
            count -= parameter::enabled;
        }

        mqtt::client.publish("control/parameter", String(parameter::enabled).c_str());
    }
    void all(String message)
    {
        hall(message);
        motor(message);
        line(message);
        steering(message);
        parameter(message);
    }
    void setup()
    {
        if (enabled) return;
        
        //Provide some controls for setup sequence
        mqtt::on("control/hall", hall);
        mqtt::on("control/motor", motor);
        mqtt::on("control/line", line);
        mqtt::on("control/steering", steering);
        mqtt::on("control/parameter", parameter);
        mqtt::on("control/all", all);
        mqtt::on("control/car", [&](String message) 
                 { loop_controller::current_mode = message; 
                   all("shutdown");
                   all("setup"); });

        all("setup"); //Do all setup

        enabled = true;
    }
}

#endif