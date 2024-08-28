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
#define get_name(var) #var

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

    template <typename T>
    String format(T a)
    {
        return get_name(a) + String(a);
    }

    void all()
    {
        Serial.println("Printing all parameters");
        mqtt::client.publish("parameter/all", (format<bool>(canGo) + format<bool>(enabled) +
                                               format<int>(current_checkpoint) + format<int>(speed) +
                                               format<int>(direction) + format<int>(Kp) + format<int>(Ki) + format<int>(Kd))
                                                  .c_str());
        ;
    }

    void update_angle()
    {
        input = line::sensor.readLineBlack(line::value);
        controller.Compute();
        steering::servo.write(output);
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
        if (enabled) return;
        Serial.print("A bunch of event listeners: ");
        controller.SetMode(AUTOMATIC);
        controller.SetOutputLimits(0, 4000);
        mqtt::on("parameter/Kp", [&](String message)
                 { Serial.println(message); all(); Kp = message.toDouble(); });
        mqtt::on("parameter/Ki", [&](String message)
                 { Serial.println(message); all(); Ki = message.toDouble(); });
        mqtt::on("parameter/Kd", [&](String message)
                 { Serial.println(message); all(); Kd = message.toDouble(); });
        mqtt::on("parameter/canGo", [&](String message)
                 { Serial.println(message); all(); canGo = message.toInt(); });
        mqtt::on("parameter/setpoint", [&](String message)
                 { Serial.println(message); all(); setpoint = message.toDouble(); });
        mqtt::on("parameter/checkpoint", [&](String message)
                 { Serial.println(message); all(); current_checkpoint = message.toInt(); });
        mqtt::on("parameter/speed", [&](String message)
                 { Serial.println(message); all(); speed = message.toInt(); });
        mqtt::on("parameter/direction", [&](String message)
                 { Serial.println(message); all(); direction = message.toInt(); });
        enabled = true;
        Serial.println("Done");
    }

    void shutdown()
    {
        enabled = false;
        //Maybe do something useful here
    }
}

#endif