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
    namespace setup_sequence
    {
        void Manual()
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

        void Auto()
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
    }

    namespace setup_mode
    {
        const String mode[] = {"manual", "auto"}; //For documenting
        std::map<String, voidFuncPtr> executor = {
            {"auto", setup_sequence::Auto},
            {"manual", setup_sequence::Manual},
        };
        String current = mode[0];
    }
    

    bool enabled = false, canRun = false;
    int current_step = 0;

    void wait()
    {
        parameter::update_traffic();
        if (parameter::canGo)
            current_step++;
    }

    void run()
    {
        parameter::update_angle();
        if (parameter::canGo)
            motor::go(parameter::speed, parameter::direction);
        else
            motor::stop();
    }

    void setup()
    {
        mqtt::on("car/run", [&](String message){ canRun = message.toInt(); });
        setup_mode::executor[setup_mode::current]();
        enabled = true;
    }

    void loop()
    {
        if (canRun)
        {
            switch (current_step)
            {
                case 0:
                    wait();
                case 1:
                    run();
            }
        }
        else
        {
            motor::stop();
        }
    }

}

#endif