// For fetching data
#ifndef API_H
#define API_H

#include <WiFiClient.h>
#include <ArduinoHttpClient.h>
#include "env.h"

namespace api
{
    HttpClient client;

    void setup(WiFiClient &wifi)
    {
        client = HttpClient(wifi, HTTP_URL, HTTP_PORT);
    }
}

#endif