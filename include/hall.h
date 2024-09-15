#pragma once

#include <Arduino.h>
#include "cloud_variable.h"

namespace hall
{
    extern int port;
    extern CloudVar<bool> enabled;

    void setup(voidFuncPtr callback);
    void shutdown();
}