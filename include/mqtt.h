#ifndef MQTT_H
#define MQTT_H

#include <map>
#include <functional>

#include <Arduino.h>
#include <PubSubClient.h>


#include "env.h"
#include "net.h"

namespace mqtt
{
    using callback_t = std::function<void(String)>;
    
    extern std::map<String, callback_t> callback;
    extern PubSubClient client;
    extern bool enabled;

    void handler(char *topic, byte *buffer, size_t length);
    void on(String topic, callback_t f);
    void connect();
    void setup();
}

#endif