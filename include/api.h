// For fetching data
#ifndef API_H
#define API_H

#include <HttpClient.h>
#include "env.h"
#include "net.h"

namespace api
{
    extern HttpClient client;

    void connect();
    void setup();
}

#endif