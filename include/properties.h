#pragma once

#include <SimpleCloud.h>
#include "env.h"
#include "mqtt.h"

namespace properties
{
    extern Cloud simple_cloud;
    void setup();
    void loop();
}
