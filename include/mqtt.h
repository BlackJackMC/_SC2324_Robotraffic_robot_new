#pragma once

#include <MqttClient.h>
#include "net.h"
#include "env.h"

namespace mqtt
{
    extern MqttClient mqtt_client;
    void setup();
}