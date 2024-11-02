#pragma once

#include "cloud.h"
#include <map>
#include <array>

namespace traffic_controller
{
    extern std::array<String, 3> traffic_light_list;
    extern std::map<String, bool> decision;
    extern int counter;

    void next_checkpoint();
    bool allow();
    void setup();
};