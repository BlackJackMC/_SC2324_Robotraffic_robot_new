#pragma once

#include "mqtt.h"

namespace cloud
{
    extern std::map<String, mqtt::callback_t> buffer;

    //Add variable topic to buffer
    void add(String name, mqtt::callback_t callback);

    //Push buffer to cloud and clear buffer
    void push();
}