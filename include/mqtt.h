#ifndef MQTT_H
#define MQTT_H

#include <map>
#include <functional>
#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "env.h"

namespace mqtt
{
    std::map<String, std::function<void(String)>> callback;
    PubSubClient client;
    bool enabled = false;

    void handler(char *topic, byte *buffer, size_t length)
    {
        String temp((char *)buffer, length);
        callback[String(topic)](temp);
    }

    void on(String event, std::function<void(String)> f) { callback[event] = f; }

    void setup(WiFiClient &wifi)
    {
        client.setClient(wifi);
        client.setServer(MQTT_URL, MQTT_PORT);
        client.setCallback(handler);
        enabled = true;
    }
}

#endif