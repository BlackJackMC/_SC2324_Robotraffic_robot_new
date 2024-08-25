// For fetching data
#ifndef API_H
#define API_H

#include <WiFiClient.h>
#include <ArduinoHttpClient.h>
#include "env.h"
#include "net.h"

namespace api
{
    // Manually added empty constructor, please DO NOT ABUSE
    HttpClient client;

    void setup()
    {
        client = HttpClient(net::wifi, HTTP_URL, HTTP_PORT);
    }
}

#endif