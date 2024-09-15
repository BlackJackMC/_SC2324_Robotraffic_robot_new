#include "setup.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "mqtt.h"
#include "hall.h"
#include "motor.h"
#include "line.h"
#include "steering.h"
#include "parameter.h"

namespace setup_sequence
{
    int count = 0; // Track the setup progress, not the proper way but it works
    const int required = 5;
    bool enabled = false;

    void send_state()
    {
        JsonDocument data;
        String buffer;

        data["hall"] = hall::enabled;
        data["motor"] = motor::enabled;
        data["line"] = line::enabled;
        data["steering"] = steering::enabled;
        data["parameter"] = parameter::enabled;

        serializeJson(data, buffer);
        mqtt::client.publish("output/state", buffer.c_str());
    }

    void hall(String message)
    {
        if (message == "setup")
        {
            hall::setup(parameter::update_magnetic);
            count += hall::enabled;
        }
        else if (message == "shutdown")
        {
            count -= hall::enabled;
            hall::shutdown();
        }

        send_state();
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

        send_state();
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
        send_state();
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

        send_state();
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

        send_state();
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
        if (enabled)
            return;

        // Provide some states for setup sequence
        mqtt::on("input/control/hall", hall);
        mqtt::on("input/control/motor", motor);
        mqtt::on("input/control/line", line);
        mqtt::on("input/control/steering", steering);
        mqtt::on("input/control/parameter", parameter);
        mqtt::on("input/control/all", all);
        mqtt::on("input/control/state", [&](String message)
                 { send_state(); });
        send_state(); // Initial state
        all("setup"); // Do all setup

        enabled = true;
    }
}