#include "MqttClient.h"

using callback_t = std::function<void(String)>;
MqttClient* MqttClient::instance = nullptr;



MqttClient& MqttClient::add_broker(String url, int port)
{
    broker_list.emplace_back(url, port);
    return *this;
}
MqttClient& MqttClient::set_username(String username)
{
    this->username = username;
    return *this;
}
MqttClient& MqttClient::set_password(String password)
{
    this->password = password;
    return *this;
}
MqttClient& MqttClient::set_id(String id)
{
    this->id = id;
    return *this;
}

MqttClient& MqttClient::set_stream(Stream& stream)
{
    this->client.setStream(stream);
    return *this;
}

MqttClient& MqttClient::set_wifi_client(Client &wifi_client)
{
    client.setClient(wifi_client);
    return *this;
}


void MqttClient::on(String topic, callback_t f)
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

void MqttClient::publish(String topic, String message)
{
    JsonDocument data;
    String buffer;
    data["id"] = id;
    data["message"] = message;
    serializeJson(data, buffer);
    client.publish(topic.c_str(), buffer.c_str());
}

void MqttClient::connect()
{
    static int current_broker_idx = 0;
    client.disconnect();
    while (!client.connected())
    {
        // Cycle through every mqtt broker save the last mqtt broker that was attempted to connect
        auto broker = broker_list[current_broker_idx];
        Serial.print(broker.first);
        Serial.print(" ");
        client.setServer(broker.first.c_str(), broker.second);

        for (int i = 0; i < 5; i++) // Try to connect for 5 attempts
        {
            if (client.connect(id.c_str(), username.c_str(), password.c_str()))
                break;
            else
                Serial.print(client.state());
                Serial.print(" ");
            delay(5000);
        }

        

        current_broker_idx = (current_broker_idx + 1) % broker_list.size();
    }

    Serial.println("Connected");
}

void MqttClient::loop()
{
    if (!client.connected()) setup();
    client.loop();
}

void MqttClient::setup()
{
    Serial.print("[mqtt]:");
    client.setStream(Serial)
          .setCallback(handler);

    instance = this;

    connect();
}

void MqttClient::handler(char *topic, byte *buffer, unsigned int length)
{
    if (MqttClient::instance == nullptr) return;
    
    MqttClient::instance->handlerImpl(topic, buffer, length);
}

void MqttClient::handlerImpl(char *topic, byte *buffer, unsigned int length)
{
    String temp((char *)buffer, length);
    auto loc = callback.find(String(topic));
    DynamicJsonDocument data(1024);
    DeserializationError e = deserializeJson(data, temp);

    if (e)
    {
        Serial.print("Error while parsing incoming message: ");
        Serial.println(e.f_str());
        return;
    }

    if (data["id"] == id)
        return;

    if (loc != callback.end())
        loc->second(data["message"]);
    else
        Serial.println("[mqtt] No callback registered for topic: " + String(topic));
}
