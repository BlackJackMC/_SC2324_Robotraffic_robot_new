#ifndef CAR_H
#define CAR_H

#include <Arduino.h>
#include <map>

#include "net.h"
#include "api.h"

#include "setup.h"
#include "loop.h"

namespace car
{
    const String stage[] = {"setup", "run"}; // For documenting
    int current_stage = 0;
    bool enabled = false;

    void setup()
    {
        setup_sequence::setup();
    }

    void loop()
    {
        loop_controller::executor[loop_controller::current_mode]();
    }
}

#endif