#include "SimpleCloud.h"

Cloud& Cloud::_add(String name, int &v, Permission permission, Priority priority, Update_Policy update_policy, callback_t on_sync, callback_t on_receive)
{
    variable.push_back(new CloudVarInt(name, v, permission, priority, update_policy, on_receive, on_sync));
    return *this;
}
Cloud& Cloud::_add(String name, float &v, Permission permission, Priority priority, Update_Policy update_policy, callback_t on_sync, callback_t on_receive)
{
    variable.push_back(new CloudVarFloat(name, v, permission, priority, update_policy, on_receive, on_sync));
    return *this;
}
Cloud& Cloud::_add(String name, double &v, Permission permission, Priority priority, Update_Policy update_policy, callback_t on_sync, callback_t on_receive)
{
    variable.push_back(new CloudVarDouble(name, v, permission, priority, update_policy, on_receive, on_sync));
    return *this;
}
Cloud& Cloud::_add(String name, bool &v, Permission permission, Priority priority, Update_Policy update_policy, callback_t on_sync, callback_t on_receive)
{
    variable.push_back(new CloudVarBool(name, v, permission, priority, update_policy, on_receive, on_sync));
    return *this;
}
Cloud& Cloud::_add(String name, String &v, Permission permission, Priority priority, Update_Policy update_policy, callback_t on_sync, callback_t on_receive)
{
    variable.push_back(new CloudVarString(name, v, permission, priority, update_policy, on_receive, on_sync));
    return *this;
}
Cloud& Cloud::set_config_id(String id)
{
    this->config_id = id;
    return *this;
}
Cloud& Cloud::set_device_id(String id)
{
    this->device_id = id;
    return *this;
}
Cloud &Cloud::set_mqtt_client(MqttClient &client)
{
    this->mqtt_client = &client;
    return *this;
}

String Cloud::get_variable_topic(String variable_name)
{
    return "config/" + config_id + "/" + variable_name;
}
String Cloud::get_event_topic(String event_name)
{
    return "device/" + device_id + F("/event/") + event_name;
}

void Cloud::setup()
{
    for (const auto i : variable)
    {
        if (i->permission > Permission::Read) mqtt_client->on(get_variable_topic(i->name), [i](String message){ i->update_from_cloud(message); });
        i->set_publish_callback([i, this](){ 
            mqtt_client->publish(get_variable_topic(i->name).c_str(), i->stringify()); 
        });
        i->publish_callback(); //For debugging
    }

    mqtt_client->publish(get_event_topic("variables"), "required");
}

void Cloud::loop()
{
    mqtt_client->loop();
    for (const auto i : variable)
        i->update();
}