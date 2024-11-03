#ifndef NET_H
#define NET_H

#include <WiFiS3.h>
#include "env.h"

namespace net
{
    extern WiFiSSLClient wifi;

    void loop();
    void setup();
}

#endif