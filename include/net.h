#ifndef NET_H
#define NET_H

#include <Wifi.h>
#include <WiFiSSLClient.h>
#include "env.h"

namespace net
{
    const char *ssid = SSID;
    const char *pass = PASS;

    WiFiSSLClient wifi;

    void setup()
    {
        Serial.print("Network: ");
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
            Serial.println("connected");
        else
            Serial.println("failed to connect");
    }
}

#endif