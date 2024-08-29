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
    
    std::map<String, callback_t> callback;
    PubSubClient client(net::wifi);
    bool enabled = false;

    void handler(char *topic, byte *buffer, size_t length)
    {
        String temp((char *)buffer, length);
        auto loc = callback.find(String(topic));
        Serial.println("[mqtt][handler]: " + String(topic) + "->" + temp);

        if (loc != callback.end())
        {
            loc->second(temp);
        }
        else
        {
            Serial.print("[mqtt] No callback registered for topic: ");
            Serial.println(String(topic));
        }
    }

    void on(String topic, callback_t f) 
    { 
        auto loc = callback.find(topic);
        if (loc == callback.end())
        {
            client.subscribe(topic.c_str());
            callback[topic] = f; 
            Serial.println("[mqtt] Subscribed to " + topic);
        }
        else
        {
            loc->second = f;
        }
        
    }

    void connect()
    {
        client.disconnect();
        while (!client.connected())
        {
            if (client.connect(("Arduino Uno R4 - " + String(random(0xffff), HEX)).c_str(), MQTT_USERNAME, MQTT_PASS))
            // if (client.connect(("Arduino Uno R4 - " + String(random(0xffff), HEX)).c_str()))
                Serial.println(" Connected");
            else
            {
                Serial.print(client.state());
                Serial.print(" Retrying ");
                delay(5000);
            }
        }
    }

    void setup()
    {
        Serial.print("[mqtt]:");
        client.setServer(MQTT_URL, MQTT_PORT);
        client.setCallback(handler);
        connect();
        enabled = true;
    }
}

#endif