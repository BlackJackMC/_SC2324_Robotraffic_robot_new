#include "parameter.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <PID_v1.h>
#include <map>

#include "api.h"
#include "mqtt.h"
#include "line.h"
#include "steering.h"
#define get_value(var) (#var "=" + String(var)).c_str()


namespace parameter
{
    bool can_go = false, enabled = false;
    int current_checkpoint = 0;
    const String checkpoint[] = {"start", "west", "south"};
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
    int speed, direction = 1, angle, magnetic = 0;

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
        data["magnetic"] = magnetic;

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
        magnetic = data["magnetic"];
        controller.SetTunings(data["P"].as<float>(), data["I"].as<float>(), data["D"].as<float>());
        controller.SetControllerDirection(!direction);
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

    void update_traffic()
    {
        if (!magnetic) return;
        magnetic = false;
        digitalWrite(LED_BUILTIN, HIGH);
        JsonDocument response;
        JsonObject current;
        DeserializationError e = deserializeJson(response, "[{\"_id\":\"66cb1eeba948bc5e0a6a22e1\",\"name\":\"west\",\"state\":\"green\"}]");

        if (e)
            Serial.println("[api]: " + String(e.f_str()));
        current = response.as<JsonArray>()[0];
        parameter::can_go = decision[response.as<JsonArray>()[0]["state"]];
        digitalWrite(LED_BUILTIN, LOW);
    }

    // For hall callback
    void update_checkpoint()
    {
        current_checkpoint++;
        if (current_checkpoint > 2)
            current_checkpoint = 1;
        magnetic = true;
        // digitalWrite(LED_BUILTIN, HIGH);
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
        enabled = true;
        Serial.println("Done");
    }

    void shutdown()
    {
        enabled = false;
        // Maybe do something useful here
    }
}