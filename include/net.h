#ifndef NET_H
#define NET_H

#include <WiFiS3.h>
#include "env.h"

namespace net
{
    extern const char *ssid;
    extern const char *pass;
    extern int status;

    extern WiFiSSLClient wifi;

    void read_response();
    void printWifiStatus();
    void loop();
    void setup();
}

#endif