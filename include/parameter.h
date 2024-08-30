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

    void print_all()
    {
        // Subscribe to different topics to prevent self-destruction
        mqtt::client.publish("output/parameter/PID/P", String(Kp).c_str());
        mqtt::client.publish("output/parameter/PID/I", String(Ki).c_str());
        mqtt::client.publish("output/parameter/PID/D", String(Kd).c_str());
        mqtt::client.publish("output/parameter/setpoint", String(setpoint).c_str());
        mqtt::client.publish("output/parameter/canGo", String(canGo).c_str());
        mqtt::client.publish("output/parameter/current_checkpoint", String(current_checkpoint).c_str());
        mqtt::client.publish("output/parameter/speed", String(speed).c_str());
        mqtt::client.publish("output/parameter/direction", String(direction).c_str());
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

        parameter::canGo = decision[current["state"]];
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
        mqtt::on("input/parameter/PID/P", [&](String message) { Kp = message.toDouble(); });
        mqtt::on("input/parameter/PID/I", [&](String message) { Ki = message.toDouble(); });
        mqtt::on("input/parameter/PID/D", [&](String message) { Kd = message.toDouble(); });
        mqtt::on("input/parameter/canGo", [&](String message) { canGo = message.toInt(); });
        mqtt::on("input/parameter/setpoint", [&](String message) { setpoint = message.toDouble(); });
        mqtt::on("input/parameter/current_checkpoint", [&](String message) { current_checkpoint = message.toInt(); });
        mqtt::on("input/parameter/speed", [&](String message) { speed = message.toInt(); });
        mqtt::on("input/parameter/direction", [&](String message) { direction = message.toInt(); });
        mqtt::on("input/parameter/all", [&](String message) { print_all(); });
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