#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "cloud.h"
#include "mqtt.h"

enum class Permission
{
    Read, // Read-only from cloud
    Write, // Read-only from client
    ReadWrite // Read/write from both
};

template <typename T>
class CloudVar
{
public:
    using callback_t = std::function<void(void)>;

    CloudVar() = default;
    CloudVar(String name, T init_val = T(), Permission type = Permission::ReadWrite, callback_t on_change = []() {});

    void set_name(String name);
    void set_initial_value(T init_val);
    void set_callback(callback_t on_change);
    void set_type(Permission type);

    operator T() const { return value; }
    T get() const { return value; }
    void set(T new_val);
    void operator=(T new_val);

private:
    String name;
    T value = T();
    T last_sent = T();
    Permission type = Permission::ReadWrite;
    callback_t on_change;

    mqtt::callback_t update = [this](String message)
    {
        if (type == Permission::Read)
            return;

        if constexpr (std::is_same<T, bool>::value)
            value = (message == "true");
        else if constexpr (std::is_arithmetic<T>::value)
            value = message.toFloat();
        else
            value = message;


        if (on_change)
            on_change();
    };

    void register_update_callback();
};

template <typename T>
CloudVar<T>::CloudVar(String name, T init_val, Permission type, callback_t on_change)
{
    set_name(name);
    set_type(type);
    set_initial_value(init_val);
    set_callback(on_change);
    register_update_callback();
}

template <typename T>
void CloudVar<T>::set_name(String name)
{
    this->name = String(F("variable/")) + name;
}

template <typename T>
void CloudVar<T>::set_initial_value(T init_val)
{
    value = init_val;
    last_sent = init_val;
}

template <typename T>
void CloudVar<T>::set_callback(callback_t on_change)
{
    this->on_change = on_change;
}

template <typename T>
void CloudVar<T>::set_type(Permission type)
{
    this->type = type;
}

template <typename T>
void CloudVar<T>::set(T new_val)
{
    if (type == Permission::Write)
        return;

    value = new_val;

    if (last_sent != value)
    {
        mqtt::publish(name.c_str(), String(value).c_str());
        last_sent = value;
    }
}

template <typename T>
void CloudVar<T>::operator=(T new_val)
{
    set(new_val);
}

template <typename T>
void CloudVar<T>::register_update_callback()
{
    if (type == Permission::ReadWrite)
    {
        cloud::add(this->name, update);
    }
}
