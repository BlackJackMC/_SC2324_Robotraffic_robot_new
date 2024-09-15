#pragma once // pls don't crash

#include <Arduino.h>
#include <UUID.h>
#include <ArduinoJson.h>
#include "mqtt.h"

using namespace mqtt;

template <typename T>
class CloudVar
{
public:
    const String topic = "variable/";

    CloudVar();
    CloudVar(String name): name(topic + name)
    {
        update();
    }
    CloudVar(String name, T init_val): name(topic + name), value(init_val)
    {
        update();
    }
    CloudVar(String name, T init_val, std::function<void(void)> callback): name(topic + name), value(init_val)
    {
        this->callback = callback;
        update();
    }

    void set_name(String name)
    {
        this->name = topic + name;
    }
    void set_initial_value(T init_val)
    {
        value = init_val;
    }
    void set_callback(std::function<void(void)> callback)
    {
        this->callback = callback;
    }

    T get()
    {
        return value;
    }
    void set(T new_val)
    {
        value = new_val;
        mqtt::client.publish(name.c_str(), String(new_val).c_str());
    }
    void operator=(T new_val)
    {
        set(new_val);
    }

    String name;

private:
    T value;
    std::function<void(void)> callback;

    void update()
    {
        mqtt::on(name.c_str(), [this](String message) {
            JsonDocument temp; //Life hack
            temp["life hack"] = message;
            value = temp["life hack"].as<T>();
        });
    }
};