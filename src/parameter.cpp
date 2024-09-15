#include "parameter.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <PID_v1.h>
#include <map>

#include "mqtt.h"
#include "line.h"
#include "steering.h"
#define get_value(var) (#var "=" + String(var)).c_str()


namespace parameter
{
    bool can_go = false, enabled = false;
    namespace checkpoint
    {
        int current = 0;
        JsonDocument data;
        const String name[] = {"start", "west", "south"};
        std::map<String, bool> decision = {
            {"red", false},
            {"red yellow", false},
            {"green", true},
            {"green blink1", true},
            {"green blink2", true},
            {"green blink3", true},
            {"green blink4", true},
            {"yellow", true},
        };
    }
    
    int speed, direction = 1, angle;

    double setpoint, input, output;
    PID controller(&input, &output, &setpoint, 0, 0, 0, !direction);

    void get_all()
    {
        JsonDocument data;
        String buffer;

        data["speed"] = speed;
        data["direction"] = direction;
        data["setpoint"] = setpoint;
        data["canGo"] = can_go;
        data["angle"] = angle;
        data["P"] = controller.GetKp();
        data["I"] = controller.GetKi();
        data["D"] = controller.GetKd();

        serializeJson(data, buffer);
        mqtt::client.publish("input/parameter", buffer.c_str(), true);
    }

    void update_data(String message)
    {
        JsonDocument data;
        DeserializationError e = deserializeJson(data, message);

        if (e) Serial.println(e.f_str());
        
        angle = data["angle"];
        speed = data["speed"];
        direction = data["direction"];
        setpoint = data["setpoint"];
        can_go = data["canGo"];
        controller.SetTunings(data["P"].as<float>(), data["I"].as<float>(), data["D"].as<float>());
        controller.SetControllerDirection(!direction);
    }

    void update_traffic(String message)
    {
        DeserializationError e = deserializeJson(checkpoint::data, message);
        if (e) Serial.println(e.f_str());
    }

    void update_magnetic()
    {
        Serial.println("[hall] Passed a checkpoint");
        checkpoint::current++;
        if (checkpoint::current > 2) checkpoint::current = 1;
        can_go = checkpoint::decision[checkpoint::name[checkpoint::current]];
    }

    void update_angle()
    {
        input = line::sensor.readLineBlack(line::value);
        controller.Compute();
        mqtt::client.publish("output/parameter/PID/input", String(input).c_str());
        mqtt::client.publish("output/parameter/PID/output", String(output).c_str());
        angle = constrain(90 + map(output, 0, 4000, 0, 150), 0, 150); //Need reconsideration!!
        steering::turn(angle);
    }

    void setup()
    {
        if (enabled)
            return;
        controller.SetMode(AUTOMATIC);
        controller.SetOutputLimits(0, 4000);

        Serial.println("A bunch of event listeners: ");
        mqtt::on("input/parameter/all", [&](String message) { get_all(); });
        mqtt::on("input/parameter", update_data);
        mqtt::on("input/parameter/angle", [&](String message) { steering::turn(message.toInt()); });
        mqtt::on("input/traffic", update_traffic);
        enabled = true;
        Serial.println("Done");
    }

    void shutdown()
    {
        enabled = false;
        // Maybe do something useful here
    }
}
