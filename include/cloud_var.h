#pragma once

#include "cloud_subscriber.h"
#include "mqtt.h"

class CloudVarBase
{
public:
    enum class Permission
    {
        Read,     // Read-only from cloud
        Write,    // Read-only from client
        ReadWrite // Read/write from both
    };

    using callback_t = std::function<void(void)>;

    CloudVarBase() = default;
    CloudVarBase(const String name, Permission type = Permission::ReadWrite, callback_t on_change = []() {})
        : name(name), type(type), on_change(on_change) {}

    virtual ~CloudVarBase() = default;

    CloudVarBase& set_name(String name)
    {
        this->name = name;
        return *this;
    }
    CloudVarBase& set_callback(callback_t on_change)
    {
        this->on_change = on_change;
        return *this;
    }
    CloudVarBase& set_type(Permission type)
    {
        this->type = type;
        return *this;
    }

protected:
    String name;
    Permission type = Permission::ReadWrite;
    callback_t on_change;

    virtual void update_from_cloud(String message) = 0;
};

class CloudVarString: public CloudVarBase
{
private:
    String local, cloud;

    CloudVarString(const String name, String init_val="", Permission type = Permission::ReadWrite, callback_t on_change = []() {}):
        CloudVarBase(name, type, on_change), local(init_val), cloud(init_val) {}

    CloudVarString& set_initial_value(String init_val)
    {
        local = init_val;
        cloud = init_val;
        return *this;
    }

    void update_from_cloud(String message) override 
    {
        local = message;

        if (on_change) on_change();
    }
};

template <typename T>
class CloudVar
{
public:
    enum class Permission
    {
        Read,     // Read-only from cloud
        Write,    // Read-only from client
        ReadWrite // Read/write from both
    };

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
    this->name = name;
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
