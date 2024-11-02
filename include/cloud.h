#pragma once

#include "cloud_var.h"

#include <map>
#include <vector>

namespace cloud
{
    extern std::vector<CloudVarBase *> variable;

#define add(v, ...) _add("/"MAC"/variable/"#v, v, __VA_ARGS__)

    void _add(String name, int &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void _add(String name, double &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void _add(String name, float &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void _add(String name, bool &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void _add(String name, String &v, Permission permission = Permission::ReadWrite, callback_t on_change = []() {});
    void setup();
    void loop();
}