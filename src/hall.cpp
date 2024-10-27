#include "hall.h"

namespace hall
{
    int pin = 2;

    void setup(voidFuncPtr callback)
    {
        pinMode(pin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin), callback, RISING);
        return;
    }

    void shutdown()
    {
        detachInterrupt(digitalPinToInterrupt(pin));
    }
}
