//For hall sensor
#ifndef HALL_H
#define HALL_H

#include <Arduino.h>
#include <functional>

namespace hall
{
    extern int port;
    extern bool enabled;

    void setup(voidFuncPtr callback);
    void shutdown();
}

#endif