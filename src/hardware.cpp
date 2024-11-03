#include "hardware.h"

namespace hardware
{
    void setup()
    {
        hall::setup(controller::update_hall);
        line::setup();
        motor::setup();
        steering::setup();
    }
}