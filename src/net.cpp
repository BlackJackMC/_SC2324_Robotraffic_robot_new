#include "net.h"

namespace net
{
    int status = WL_IDLE_STATUS;

    // WiFiSSLClient wifi;
    WiFiClient wifi;

    void printWifiStatus()
    {
        IPAddress ip = WiFi.localIP();

        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        Serial.print("IP Address: ");
        Serial.println(ip);

        long rssi = WiFi.RSSI();
        Serial.print("signal strength (RSSI):");
        Serial.print(rssi);
        Serial.println(" dBm");
    }

    void setup()
    {

        Serial.print("[network]: ");
        if (WiFi.status() == WL_NO_MODULE)
        {
            Serial.println("Wifi module not detected");
            while (true);
        }

        if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION)
            Serial.println("Please upgrade the firmware");

        // wifi.setCACert(CERTIFICATE);

        while (status != WL_CONNECTED)
        {
            // Try to connect to one of the wifi
            for (const auto &wifi : WIFI_LIST)
            {
                Serial.println(wifi.first);
                status = WiFi.begin(wifi.first, wifi.second);
                delay(15000);
                if (!status == WL_CONNECTED) continue;
                printWifiStatus();    
                break;
            }
        }

        Serial.println("Connected");
    }

    void loop()
    {
        if (!wifi.connected()) setup();
    }
}