#pragma once

#include "mqtt.h"

enum class Permission
{
    Read,     // Read-only from cloud
    ReadWrite // Read/write from cloud
};

using callback_t = std::function<void(void)>;


class CloudVarBase
{
public:

    CloudVarBase() = default;
    CloudVarBase(const String name, Permission type = Permission::ReadWrite, callback_t on_change = []() {}) : name(name), type(type), on_change(on_change) {}

    virtual ~CloudVarBase() = default;

    CloudVarBase &set_name(String name)
    {
        this->name = name;
        return *this;
    }
    CloudVarBase &set_callback(callback_t on_change)
    {
        this->on_change = on_change;
        return *this;
    }
    CloudVarBase &set_type(Permission type)
    {
        this->type = type;
        return *this;
    }

    virtual void update_from_cloud(String message) = 0;
    virtual void update_to_cloud() = 0;
    virtual void update_to_local() = 0;
    virtual bool different_from_cloud() = 0;

    virtual String stringify() const = 0;
    
    String name;
    Permission type = Permission::ReadWrite;
    callback_t on_change;
};

class CloudVarString : public CloudVarBase
{
public:
    String &local, cloud;

    CloudVarString(const String name, String &local, Permission type = Permission::ReadWrite, callback_t on_change = []() {}) : CloudVarBase(name, type, on_change), local(local), cloud(local) {}

    void update_from_cloud(String message) override
    {
        cloud = message;

        if (on_change)
            on_change();
    }

    void update_to_local() override { local = cloud; }
    void update_to_cloud() override { cloud = local; }
    bool different_from_cloud() override { return local != cloud; }
    String stringify() const override { return local; }
};

class CloudVarInt : public CloudVarBase
{
public:
    int &local, cloud;

    CloudVarInt(const String name, int &local, Permission type = Permission::ReadWrite, callback_t on_change = []() {}) : CloudVarBase(name, type, on_change), local(local), cloud(local) {}

    void update_from_cloud(String message) override
    {
        cloud = message.toInt();

        if (on_change)
            on_change();
    }

    void update_to_local() override { local = cloud; }
    void update_to_cloud() override { cloud = local; }
    bool different_from_cloud() override { return local != cloud; }
    String stringify() const override { return String(local); }
};

class CloudVarFloat : public CloudVarBase
{
public:
    float &local, cloud;

    CloudVarFloat(const String name, float &local, Permission type = Permission::ReadWrite, callback_t on_change = []() {}) : CloudVarBase(name, type, on_change), local(local), cloud(local) {}

    void update_from_cloud(String message) override
    {
        cloud = message.toFloat();

        if (on_change)
            on_change();
    }

    void update_to_local() override { local = cloud; }
    void update_to_cloud() override { cloud = local; }
    bool different_from_cloud() override { return local != cloud; }
    String stringify() const override { return String(local); }
};

class CloudVarBool : public CloudVarBase
{
public:
    bool &local, cloud;

    CloudVarBool(const String name, bool &local, Permission type = Permission::ReadWrite, callback_t on_change = []() {}) : CloudVarBase(name, type, on_change), local(local), cloud(local) {}

    void update_from_cloud(String message) override
    {
        cloud = message.toInt();

        if (on_change)
            on_change();
    }

    void update_to_local() override { local = cloud; }
    void update_to_cloud() override { cloud = local; }
    bool different_from_cloud() override { return local != cloud; }
    String stringify() const override { return String(local); }
};