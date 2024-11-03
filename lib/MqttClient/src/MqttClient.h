#pragma once

#include <functional>
#include <vector>
#include <map>

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiS3.h>

class MqttClient
{
public:
    using callback_t = std::function<void(String)>;

    MqttClient(){}
    // MqttClient(const MqttClient& other) = default;

    MqttClient& add_broker(String url, int port);
    MqttClient& set_username(String username);
    MqttClient& set_password(String password);
    MqttClient& set_id(String id);
    MqttClient& set_wifi_client(Client &wifi_client);

    static void handler(char *topic, byte *buffer, unsigned int length);
    void on(String topic, callback_t f);
    void publish(String topic, String message);

    void setup();
    void loop();

    static MqttClient* instance;

private:
    std::map<String, callback_t> callback;
    std::vector<std::pair<String, int>> broker_list;
    PubSubClient client;
    String username, password, id;

    void connect();

    void handlerImpl(char *topic, byte *buffer, unsigned int length);
};
