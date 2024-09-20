#include "cloud.h"

namespace cloud
{
    std::map<String, mqtt::callback_t> buffer;

    //Add variable topic to buffer
    void add(String name, mqtt::callback_t callback)
    {
        buffer[name] = callback;
    }

    //Push buffer to cloud and clear buffer
    void push()
    {
        if (!mqtt::client.connected() or buffer.empty()) return;

        for (const auto& i: buffer)
        {
            mqtt::on(i.first, i.second);
        }

        buffer.clear();
    }

    void loop()
    {
        push();
    }
}