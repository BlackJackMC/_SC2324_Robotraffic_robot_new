#pragma once

#include "cloud.h"

namespace cloud
{
#define get_variable_name(v) #v
    std::vector<CloudVarBase *> variable;
    bool is_setup = false;

    inline void add(auto &v, auto... args)
    {
        _add(get_variable_name(v), v, args...);
    }

    void _add(const String name, int &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarInt(name, v, permission, on_change));
    }

    void _add(const String name, float &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarFloat(name, v, permission, on_change));
    }

    void _add(const String name, bool &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarBool(name, v, permission, on_change));
    }

    void _add(const String name, String &v, Permission permission, callback_t on_change)
    {
        variable.push_back(new CloudVarString(name, v, permission, on_change));
    }

    void setup()
    {
        for (const auto i : variable)
            if (i->type > Permission::Read)
                mqtt::on(i->name, [i](String message){i->update_from_cloud(message); });

        is_setup = true;
    }

    void loop()
    {
        if (!is_setup) return;
        for (const auto i : variable)
        {
            if (i->different_from_cloud())
            {
                i->update_to_cloud();
                mqtt::publish(i->name, i->stringify());
            }
        }
    }
#undef get_variable_name
}