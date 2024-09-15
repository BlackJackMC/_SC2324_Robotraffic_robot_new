#pragma once // pls don't crash

#include <Arduino.h>
#include <ArduinoJson.h>
#include "mqtt.h"

template <typename T>
class CloudVar
{
public:
    const String topic = "variable/";

    CloudVar();
    CloudVar(String name);
    CloudVar(String name, T init_val);
    CloudVar(String name, T init_val, std::function<void(void)> callback);

    void set_name(String name);
    void set_initial_value(T init_val);
    void set_callback(std::function<void(void)> callback);

    T get();
    void set(T new_val);
    void operator=(T new_val);

    String name;

private:
    T value;
    std::function<void(void)> callback;

    void update();
};

// Template Definitions

template <typename T>
CloudVar<T>::CloudVar() {}

template <typename T>
CloudVar<T>::CloudVar(String name) : name(topic + name), value()
{
    update();
}

template <typename T>
CloudVar<T>::CloudVar(String name, T init_val) : name(topic + name), value(init_val)
{
    update();
}

template <typename T>
CloudVar<T>::CloudVar(String name, T init_val, std::function<void(void)> callback) : name(topic + name), value(init_val), callback(callback)
{
    update();
}

template <typename T>
void CloudVar<T>::set_name(String name)
{
    this->name = topic + name;
}

template <typename T>
void CloudVar<T>::set_initial_value(T init_val)
{
    value = init_val;
}

template <typename T>
void CloudVar<T>::set_callback(std::function<void(void)> callback)
{
    this->callback = callback;
}

template <typename T>
T CloudVar<T>::get()
{
    return value;
}

template <typename T>
void CloudVar<T>::set(T new_val)
{
    value = new_val;
    mqtt::client.publish(name.c_str(), String(new_val).c_str());
}

template <typename T>
void CloudVar<T>::operator=(T new_val)
{
    set(new_val);
}

template <typename T>
void CloudVar<T>::update()
{
    // mqtt::on(name.c_str(), [this](String message) {
    //     // Use DynamicJsonDocument for dynamic allocation
    //     DynamicJsonDocument temp(1024);
    //     temp["value"] = message;
    //     value = temp["value"].as<T>();
    //     if (callback)
    //     {
    //         callback();
    //     }
    // });
}
