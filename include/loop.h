#ifndef LOOP_H
#define LOOP_H

#include <map>

#include "hall.h"
#include "motor.h"
#include "setup.h"
#include "parameter.h"

namespace loop_controller
{
    const String mode[] = {"test", "real"}; // For documenting
    String current_mode = mode[0];
    int current_step = 0;

    namespace loop_sequence
    {
        namespace test
        {
            void setup()
            {
                hall::shutdown();
            }
            void run()
            {
                parameter::update_angle();
                if (parameter::canGo)
                    motor::go(parameter::speed, parameter::direction);
                else
                    motor::stop();
            }
            void loop()
            {
                run();
            }
        }
        namespace real
        {
            void wait()
            {
                parameter::update_traffic();
                if (parameter::canGo)
                    current_step++;
            }
            void run()
            {
                parameter::update_angle();
                if (parameter::canGo)
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
    }

    std::map<String, voidFuncPtr> executor = {
        {"test", loop_sequence::test::loop},
        {"real", loop_sequence::real::loop}};
}

#endif