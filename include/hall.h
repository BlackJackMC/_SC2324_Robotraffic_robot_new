#pragma once

#include <Arduino.h>
#include "cloud_var.h"

namespace hall
{
    extern int port;
    extern CloudVar<bool> enabled;
    extern CloudVar<String> command;

    void start(voidFuncPtr callback);
    void setup(voidFuncPtr callback);
    void shutdown();
}