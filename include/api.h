// For fetching data
#ifndef API_H
#define API_H

#include <HttpClient.h>
#include "env.h"
#include "net.h"

namespace api
{
    HttpClient client(net::wifi, HTTP_URL, HTTP_PORT);

    void connect()
    {
        int err = client.get("/ping");
        for (int attempt = 0; err != 0 and attempt < 10; attempt++)
        {
            Serial.print(" Connection failed with error ");
            Serial.print(err);
            delay(2000);
            err = client.get("/ping");
        }
        Serial.println(" Connected");
    }

    void setup()
    {
        Serial.print("API:");
        connect();
    }
}

#endif