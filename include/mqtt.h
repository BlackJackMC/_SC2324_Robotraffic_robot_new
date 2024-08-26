#ifndef MQTT_H
#define MQTT_H

#include <map>
#include <functional>
#include <optional>

#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "env.h"
#include "net.h"

namespace mqtt
{
    using callback_t = std::function<void(String)>;

    std::map<String, callback_t> callback;
    PubSubClient client;
    bool enabled = false;

    void handler(char *topic, byte *buffer, size_t length)
    {
        String temp((char *)buffer, length);
        callback[String(topic)](temp);
    }

    void on(String event, callback_t f) { callback[event] = f; }

    void connect()
    {
        client.disconnect();
        while (!client.connected())
        {
            if (client.connect("Arduino Uno R4"))
                Serial.println("Connected");
            else
            {
                Serial.println(client.state());
                Serial.println("Retrying");
                delay(5000);
            }
        }
    }

    void setup()
    {
        client.setClient(net::wifi);
        client.setServer(MQTT_URL, MQTT_PORT);
        client.setCallback(handler);
        connect();
        enabled = true;
    }
}

#endif