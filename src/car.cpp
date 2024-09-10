#include "car.h"

namespace car
{
    bool enabled = false;

    void setup()
    {
        setup_sequence::setup();
        enabled = setup_sequence::enabled;
    }

    void loop()
    {
        loop_controller::executor[loop_controller::current_mode]();
    }
}