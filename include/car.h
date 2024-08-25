#ifndef CAR_H
#define CAR_H

#include <Arduino.h>
#include <map>

#include "net.h"
#include "api.h"
#include "mqtt.h"

#include "hall.h"
#include "motor.h"
#include "line.h"
#include "steering.h"

#include "parameter.h"

namespace car
{
    const String mode[] = {"manual", "auto", "test"}; //For documenting
    std::map<String, voidFuncPtr> setup_func; 
    String current_mode = mode[0];
    bool enabled = false;

    void manual_setup()
    {
        mqtt::on("control/hall", [&](String message)
        { 
            if (message == "on") 
                hall::setup(parameter::update_checkpoint); 
            else 
                hall::shutdown();

            mqtt::client.publish("control/hall", String(hall::enabled).c_str());
        });

        mqtt::on("control/motor", [&](String message)
        { 
            if (message == "on") 
                motor::setup(); 
            else
                motor::shutdown();
            
            mqtt::client.publish("control/motor", String(motor::enabled).c_str());
        });

        mqtt::on("control/line", [&](String message)
        { 
            if (message == "setup") line::setup(); 
            mqtt::client.publish("control/line", String(line::enabled).c_str());
        });

        mqtt::on("control/steering", [&](String message)
        {
            if (message == "setup")
                steering::setup();
            else
                steering::shutdown();

            mqtt::client.publish("control/steering", String(steering::enabled).c_str());
        });

        Serial.println("Waiting for setup command");
    }

    void auto_setup()
    {
        hall::setup(parameter::update_checkpoint);
        motor::setup();
        line::setup();
        steering::setup();
        parameter::setup();
        mqtt::client.publish("control/steering", String(steering::enabled).c_str());
        mqtt::client.publish("control/hall", String(hall::enabled).c_str());
        mqtt::client.publish("control/motor", String(motor::enabled).c_str());
        mqtt::client.publish("control/line", String(line::enabled).c_str());
        mqtt::client.publish("control/paramter", String(parameter::enabled).c_str());
        Serial.print("Auto setup completed");
    }

    void setup()
    {
        setup_func["auto"] = auto_setup;
        setup_func["manual"] = manual_setup;

        setup_func[current_mode]();
        enabled = true;
    }

    void loop()
    {
        if (parameter::go)
            motor::go(parameter::speed, parameter::direction);
        else
            motor::stop();

        parameter::update_angle();
    }

}

#endif