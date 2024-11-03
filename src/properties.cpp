#include "properties.h"

namespace properties
{
    Cloud simple_cloud;

    void setup()
    {
        simple_cloud.set_mqtt_client(mqtt::mqtt_client)
                    .set_config_id(CONFIG_ID)
                    .set_device_id(DEVICE_ID)
                    .setup();
    }

    void loop()
    {
        simple_cloud.loop();
    }
}