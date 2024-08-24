#ifndef API_H
#define API_H


#include <Wifi.h>
#include <WiFiClient.h>
#include <HttpClient.h>
#include "env.h"

namespace api
{
  const char* ssid = SSID;
  const char* pass = PASS;

  WiFiClient wifi;
  HttpClient client(wifi, URL, PORT);

  void setup()
  {
    if (WiFi.status() == WL_NO_MODULE)
    {
      Serial.println("Wifi module not detected");
      while (true);
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