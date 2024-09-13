#include "loop_sequence.h"
#include "loop_controller.h"
#include "motor.h"
#include "setup.h"
#include "parameter.h"

using loop_controller::current_step;

namespace loop_sequence::test
{
    void setup()
    {
        setup_sequence::hall("shutdown");
        current_step++;
    }
    void run()
    {
        parameter::update_traffic();
        if (parameter::can_go)
        {
            parameter::update_angle();
            motor::go(parameter::speed, parameter::direction);
        }
        else
            motor::stop();
    }
    void loop()
    {
        switch (current_step)
        {
        case 0:
            setup();
            break;
        case 1:
            run();
            break;
        }
    }
}

namespace loop_sequence::real
{
    void wait()
    {
        parameter::update_traffic();
        if (parameter::can_go)
            current_step++;
    }
    void run()
    {
        parameter::update_angle();
        if (parameter::can_go)
            motor::go(parameter::speed, parameter::direction);
        else
            motor::stop();
    }
    void loop()
    {
        switch (loop_controller::current_step)
        {
        case 0:
            wait();
        case 1:
            run();
        }
    }
}