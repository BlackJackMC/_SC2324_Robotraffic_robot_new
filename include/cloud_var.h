#pragma once // pls don't crash

#include <Arduino.h>
#include <ArduinoJson.h>
#include "cloud.h"
#include "mqtt.h"

template <typename T>
class CloudVar
{
public:
    const String topic PROGMEM = "variable/";
    using callback_t = std::function<void(void)>;

    CloudVar();
    CloudVar(String name);
    CloudVar(String name, T init_val);
    CloudVar(String name, T init_val, callback_t on_change);

    void set_name(String name);
    void set_initial_value(T init_val);
    void set_callback(callback_t on_change);

    explicit operator T() const
    {
        return value;
    }

    T get();
    void set(T new_val);
    void operator=(T new_val);

    String name;

private:
    T value;
    callback_t on_change;

    mqtt::callback_t update = [this](String message)
    {
        JsonDocument temp;
        temp["temp"] = message;
        Serial.println("[cloud_var]: " + String(temp["temp"].as<double>(), 5) 
                                 + " " + String(temp["temp"].as<int>()) 
                                 + " " + String(temp["temp"].as<bool>())
                                 + " " + String(temp["temp"].as<String>()));
        value = temp["temp"].as<T>();
        on_change();
    };
};

// Template Definitions

template <typename T>
CloudVar<T>::CloudVar() {}

template <typename T>
CloudVar<T>::CloudVar(String name) : name(topic + name), value()
{
    cloud::add(this->name, update);
}

template <typename T>
CloudVar<T>::CloudVar(String name, T init_val) : name(topic + name), value(init_val)
{
    cloud::add(this->name, update);
}

template <typename T>
CloudVar<T>::CloudVar(String name, T init_val, callback_t on_change) : name(topic + name), value(init_val), on_change(on_change)
{
    cloud::add(this->name, update);
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
void CloudVar<T>::set_callback(callback_t on_change)
{
    this->on_change = on_change;
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
    mqtt::publish(name.c_str(), String(value).c_str());
}

template <typename T>
void CloudVar<T>::operator=(T new_val)
{
    set(new_val);
}
