#include "net.h"

namespace net
{
    const char *ssid = WIFI_SSID;
    const char *pass = WIFI_PASS;
    int status = WL_IDLE_STATUS;

    WiFiSSLClient wifi;

    void read_response()
    {
        uint32_t received_data_num = 0;
        while (wifi.available())
        {
            char c = wifi.read();
            Serial.print(c);
            received_data_num++;
            if (received_data_num % 80 == 0)
            {
                Serial.println();
            }
        }
    }

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
        Serial.print("Network: ");
        if (WiFi.status() == WL_NO_MODULE)
        {
            Serial.println("Wifi module not detected");
            while (true)
                ;
        }
        String fv = WiFi.firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION)
        {
            Serial.println("Please upgrade the firmware");
        }

        wifi.setCACert(CERTIFICATE);

        while (status != WL_CONNECTED)
        {
            // Serial.print("Attempting to connect to SSID: ");
            Serial.println(ssid);
            status = WiFi.begin(ssid, pass);
            delay(15000);
        }
        Serial.println("Connected");
        printWifiStatus();
        delay(5000);
    }
}