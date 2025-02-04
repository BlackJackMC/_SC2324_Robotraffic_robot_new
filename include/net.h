#ifndef NET_H
#define NET_H

#include <WiFiS3.h>
#include "env.h"

namespace net
{
    // extern WiFiSSLClient wifi;
    extern WiFiClient wifi;

    void loop();
    void setup();
}

#endif