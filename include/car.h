#ifndef CAR_H
#define CAR_H

#include <Arduino.h>
#include <map>

#include "net.h"
#include "api.h"

#include "setup.h"
#include "loop_controller.h"

namespace car
{
    extern bool enabled;

    void setup();
    void loop();
}

#endif