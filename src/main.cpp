#include <Arduino.h>
#include <map>

#include "net.h"
#include "api.h"
#include "mqtt.h"

#include "hall.h"
#include "motor.h"
#include "line.h"
#include "wheel.h"

namespace car
{
    const String mode[] = {"manual", "auto", "test"}; //For documenting
    std::map<String, voidFuncPtr> setup_func;
    String current_mode = mode[0];

    void manual_setup()
    {
        mqtt::on("control/hall", [&](String message)
        { 
            if (message == "on") 
                hall::setup([](){}); 
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

        mqtt::on("control/wheel", [&](String message)
        {
            if (message == "setup")
                wheel::setup();
            else
                wheel::shutdown();

            mqtt::client.publish("control/wheel", String(wheel::enabled).c_str());
        });

        Serial.println("Waiting for setup command");
    }

    void auto_setup()
    {
        hall::setup([](){});
        motor::setup();
        line::setup();
        wheel::setup();
        mqtt::client.publish("control/wheel", String(wheel::enabled).c_str());
        mqtt::client.publish("control/hall", String(hall::enabled).c_str());
        mqtt::client.publish("control/motor", String(motor::enabled).c_str());
        mqtt::client.publish("control/line", String(line::enabled).c_str());
        Serial.print("Auto setup completed");
    }

    void setup()
    {
        setup_func["auto"] = auto_setup;
        setup_func["manual"] = manual_setup;

        setup_func[current_mode]();
    }
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ; // Wait for serial

    net::setup();
    api::setup(net::wifi);
    mqtt::setup(net::wifi);
    mqtt::on("control/mode", [&](String message)
             { car::current_mode = message; car::setup(); });
}

void loop()
{
}