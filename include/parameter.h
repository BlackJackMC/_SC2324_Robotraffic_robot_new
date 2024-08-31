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

#pragma once
#define get_value(var) (#var "=" + String(var)).c_str()

namespace parameter
{
    bool canGo = false, enabled = false;
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
    int speed, direction;

    double setpoint, input, output;
    double Kp, Ki, Kd;
    PID controller(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

    void get_all()
    {
        JsonObject data;
        JsonArray PID;
        String buffer;

        data["speed"] = speed;
        data["direction"] = direction;
        data["setpoint"] = setpoint;
        data["canGo"] = canGo;
        PID = data["PID"].to<JsonArray>();
        PID.add(Kp);
        PID.add(Ki);
        PID.add(Kd);

        serializeJson(data, buffer);
        mqtt::client.publish("input/parameter", buffer.c_str(), true);
    }

    void update_data(String message)
    {
        JsonDocument data;
        DeserializationError e = deserializeJson(data, message);

        if (e) Serial.println(e.f_str());

        speed = data["speed"];
        direction = data["direction"];
        setpoint = data["setpoint"];
        canGo = data["canGo"];
        Kp = data["PID"][0];
        Ki = data["PID"][1];
        Kd = data["PID"][2];
    }

    void update_angle()
    {
        input = line::sensor.readLineBlack(line::value);
        controller.Compute();
        steering::servo.write(output);
        mqtt::client.publish("output/parameter/PID/input", String(input).c_str());
        mqtt::client.publish("output/parameter/PID/output", String(output).c_str());
    }

    // For hall callback
    void update_traffic()
    {
        JsonDocument response;
        JsonObject current;
        api::client.get("/state?name=" + checkpoint[current_checkpoint]);
        DeserializationError e = deserializeJson(response, api::client.responseBody());

        if (e)
            Serial.println(e.f_str());

        current = response.as<JsonArray>()[0];

        parameter::canGo = decision[response.as<JsonArray>()[0]["state"]];
    }

    // For hall callback
    void update_checkpoint()
    {
        current_checkpoint++;
        if (current_checkpoint > 2)
            current_checkpoint = 1;
        update_traffic();
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
        enabled = true;
        Serial.println("Done");
    }

    void shutdown()
    {
        enabled = false;
        // Maybe do something useful here
    }
}

#endif