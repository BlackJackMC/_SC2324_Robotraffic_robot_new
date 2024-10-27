#pragma once

#include <Arduino.h>
#include "cloud_var.h"

namespace hall
{
    extern int pin;
    
    void setup(voidFuncPtr callback);
    void shutdown();
}