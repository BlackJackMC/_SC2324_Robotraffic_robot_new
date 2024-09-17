#include "mqtt.h"

namespace mqtt
{
    using callback_t = std::function<void(String)>;

    std::map<String, callback_t> callback;
    PubSubClient client(net::wifi);
    String id = "";
    bool enabled = false;

    void handler(char *topic, byte *buffer, size_t length)
    {
        String temp((char *)buffer, length);
        auto loc = callback.find(String(topic));
        JsonDocument data;
        DeserializationError e = deserializeJson(data, temp);

        if (e)
        {
            Serial.print("Error while parsing incoming message: ");
            Serial.println(e.f_str());
            return;
        }

        Serial.println("[mqtt][handler]: " + String(topic) + "->" + temp);

        if (data["id"] == id)
            return;

        if (loc != callback.end())
            loc->second(data["message"]);
        else
            Serial.println("[mqtt] No callback registered for topic: " + String(topic));
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

    void publish(String topic, String message)
    {
        JsonDocument data;
        String buffer;
        data["id"] = id;
        data["message"] = message;
        serializeJson(data, buffer);
        client.publish(topic.c_str(), buffer.c_str());
    }

    void connect()
    {
        client.disconnect();
        while (!client.connected())
        {
            id = ("Arduino Uno R4 - " + String(random(0xffff), HEX));
            if (client.connect(id.c_str(), MQTT_USERNAME, MQTT_PASS))
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