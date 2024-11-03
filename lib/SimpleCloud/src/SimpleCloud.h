#pragma once

#include "cloud_var.h"

#include <map>
#include <vector>
#include <WiFiS3.h>
#include <MqttClient.h>

class Cloud
{
private:
    std::vector<CloudVarBase *> variable;
    String config_id, device_id;
    MqttClient *mqtt_client;

public:
    Cloud(){}

#define add(v, ...) _add(#v, v, __VA_ARGS__)

    Cloud &_add(String name, int &v, Permission permission = Permission::ReadWrite, Priority priority = Priority::Cloud, Update_Policy update_policy = Update_Policy::Change, callback_t on_sync = []() {}, callback_t on_receive = []() {});
    Cloud &_add(String name, double &v, Permission permission = Permission::ReadWrite, Priority priority = Priority::Cloud, Update_Policy update_policy = Update_Policy::Change, callback_t on_sync = []() {}, callback_t on_receive = []() {});
    Cloud &_add(String name, float &v, Permission permission = Permission::ReadWrite, Priority priority = Priority::Cloud, Update_Policy update_policy = Update_Policy::Change, callback_t on_sync = []() {}, callback_t on_receive = []() {});
    Cloud &_add(String name, bool &v, Permission permission = Permission::ReadWrite, Priority priority = Priority::Cloud, Update_Policy update_policy = Update_Policy::Change, callback_t on_sync = []() {}, callback_t on_receive = []() {});
    Cloud &_add(String name, String &v, Permission permission = Permission::ReadWrite, Priority priority = Priority::Cloud, Update_Policy update_policy = Update_Policy::Change, callback_t on_sync = []() {}, callback_t on_receive = []() {});

    Cloud &set_config_id(String id);
    Cloud &set_device_id(String id);
    Cloud &set_mqtt_client(MqttClient &client);

    String get_variable_topic(String variable_name);
    String get_event_topic(String event_name);
    void setup();
    void loop();
};