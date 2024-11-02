#pragma once

#include "cloud.h"

namespace cloud
{
    std::vector<CloudVarBase *> variable;
    bool is_setup = false;

    void _add(String name, int &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarInt(name, v, permission, on_change));
    }

    void _add(String name, float &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarFloat(name, v, permission, on_change));
    }

    void _add(String name, double &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarDouble(name, v, permission, on_change));
    }

    void _add(String name, bool &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarBool(name, v, permission, on_change));
    }

    void _add(String name, String &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarString(name, v, permission, on_change));
    }

    void setup()
    {
        for (const auto i : variable)
        {
            if (i->type > Permission::Read)
                mqtt::on(i->name, [i](String message){i->update_from_cloud(message); });
            mqtt::publish(i->name, "undefined");
        }



        is_setup = true;
    }

    void loop()
    {
        if (!is_setup) return;
        for (const auto i : variable)
        {
            if (i->different_from_cloud())
            {
                Serial.println(i->name + " is different from cloud");
                i->update_to_local();
            }
        }
    }
}