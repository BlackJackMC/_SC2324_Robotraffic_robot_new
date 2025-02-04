#include "mqtt.h"

namespace mqtt 
{
    MqttClient mqtt_client;

    void setup()
    {
        mqtt_client.set_wifi_client(net::wifi)
                   .set_id(DEVICE_ID)
                   .set_username(MQTT_USERNAME)
                   .set_password(MQTT_PASS)
                //    .set_stream(Serial)
                   .add_broker("192.168.1.2", 1883)
                //    .add_broker("9e35e9a554374645986f67836435675c.s1.eu.hivemq.cloud", 8883)
                //    .add_broker("broker.hivemq.com", 8883)
                   .setup();
    }
}