#pragma once // pls don't crash

#include <Arduino.h>
#include <ArduinoJson.h>
#include "cloud.h"
#include "mqtt.h"

enum class Permission
{
    Read,
    ReadWrite
};

template <typename T>
class CloudVar
{
public:
    const String topic PROGMEM = "variable/";
    using callback_t = std::function<void(void)>;

    CloudVar();
    CloudVar(String name, callback_t on_change);
    CloudVar(String name, Permission type, callback_t on_change = [](){});
    CloudVar(String name, T init_val, Permission type);
    CloudVar(String name, T init_val, callback_t on_change);
    CloudVar(String name, T init_val = T(), Permission type = Permission::ReadWrite, callback_t on_change = []() {});

    void set_name(String name);
    void set_initial_value(T init_val);
    void set_callback(callback_t on_change);
    void set_type(Permission type);

    operator T() const
    {
        return value;
    }

    T get();
    void set(T new_val);
    void operator=(T new_val);

    String name;

private:
    T value;
    T last_sent;
    Permission type;
    callback_t on_change;

    mqtt::callback_t update = [this](String message)
    {
        if (type == Permission::Read)
        {
            Serial.println(F("[cloud]: Read-only, can't update"));
            return;
        }

        JsonDocument temp;
        temp[F("temp")] = message;
        if constexpr (std::is_same<T, bool>::value)
        {
            value = temp[F("temp")].as<int>();
        }
        else
            value = temp[F("temp")].as<T>();
        Serial.print(F("[cloud]: "));
        Serial.print(name);
        Serial.print("->");
        Serial.println(value);
        if (on_change)
            on_change();
    };
};

// Template Definitions

template <typename T>
CloudVar<T>::CloudVar() {}

template <typename T>
CloudVar<T>::CloudVar(String name, callback_t on_change) : CloudVar(name, T(), Permission::ReadWrite, on_change) {}

template <typename T>
CloudVar<T>::CloudVar(String name, Permission type, callback_t on_change) : CloudVar(name, T(), type, on_change) {}

template <typename T>
CloudVar<T>::CloudVar(String name, T init_val, Permission type) : CloudVar(name, init_val, type) {}

template <typename T>
CloudVar<T>::CloudVar(String name, T init_val, callback_t on_change) : CloudVar(name, init_val, Permission::ReadWrite, on_change) {}

template <class T>
CloudVar<T>::CloudVar(String name, T init_val, Permission type, callback_t on_change)
{
    this->name = topic + name;
    this->value = init_val;
    this->type = type;
    this->on_change = on_change;

    if (type == Permission::ReadWrite)
    {
        cloud::add(this->name, update);
    }
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
void CloudVar<T>::set_type(Permission type)
{
    this->type = type;
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
