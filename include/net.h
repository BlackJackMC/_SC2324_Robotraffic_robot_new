#ifndef NET_H
#define NET_H

#include <Wifi.h>
#include <WiFiClient.h>

namespace net
{
    const char *ssid = SSID;
    const char *pass = PASS;

    WiFiClient wifi;

    void setup()
    {
        if (WiFi.status() == WL_NO_MODULE)
        {
            Serial.println("Wifi module not detected");
            while (true)
                ;
        }

        WiFi.begin(ssid, pass);
        for (int attempt = 0; WiFi.status() != WL_CONNECTED and attempt < 10; attempt++)
            delay(500);

        if (WiFi.status() == WL_CONNECTED)
            Serial.println("WiFi connected");
        else
            Serial.println("WiFi failed to connect");
    }
}

#endif