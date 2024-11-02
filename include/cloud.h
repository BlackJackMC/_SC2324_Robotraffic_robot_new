#pragma once

#include "cloud_var.h"

#include <map>
#include <vector>

namespace cloud
{
#define get_variable_name(v) #v
    extern std::vector<CloudVarBase *> variable;

    inline void add(auto v, auto... args);
    void _add(const String name, int &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void _add(const String name, float &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void _add(const String name, bool &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void _add(const String name, String &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void setup();
    void loop();
#undef get_variable_name
}