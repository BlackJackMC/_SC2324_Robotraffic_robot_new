#pragma once

#include "properties.h"
#include <map>

namespace traffic_controller
{
    void next_checkpoint();
    bool allow();
    void setup();
};