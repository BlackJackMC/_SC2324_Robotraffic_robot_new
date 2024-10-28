#pragma once

#include "mqtt.h"

namespace cloud_subscriber
{
    extern std::map<String, mqtt::callback_t> buffer;
    void add(String name, mqtt::callback_t callback);
    void push();
    void loop();
}