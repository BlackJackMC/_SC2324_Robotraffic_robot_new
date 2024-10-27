#include "controller.h"
#include "cloud_var.h"
#include "mqtt.h"
#include "line.h"
#include "steering.h"
#include "motor.h"
#include "traffic_controller.h"

namespace controller
{
    CloudVar<int> speed("speed");
    CloudVar<int> direction("direction");
    CloudVar<float> P("P");
    CloudVar<float> I("I");
    CloudVar<float> D("D");
    CloudVar<int> angle("angle");
    CloudVar<float> input("input");
    CloudVar<float> output("output");
    CloudVar<int> setpoint("setpoint");

    double local_input, local_output, local_setpoint;
    bool checkpoint_passed = false;
    
    PID pid(&local_input, &local_output, &local_setpoint, P.get(), I.get(), D.get(), !direction.get());


    void update_angle()
    {
        local_input = line::sensor.readLineBlack(line::value);
        input = local_input;
        pid.Compute();
        output = local_output;

        angle = constrain(angle + map(output, 0, 4000, 0, 150), 0, 150);
    }
    
    void update_hall()
    {
        traffic_controller::next_checkpoint();
        checkpoint_passed = true;
    }
    
    void loop()
    {
        if (checkpoint_passed and !traffic_controller::allow()) 
        {
            motor::stop();
            return;
        }
        else
            checkpoint_passed = false;

        update_angle();
        steering::turn(angle);
        motor::go(speed, direction);
    }

    void setup()
    {
        pid.SetMode(AUTOMATIC);
        pid.SetOutputLimits(0, 4000);

        angle.set_callback([&](){ steering::turn(angle); });
        P.set_callback([&](){ pid.SetTunings(P, I, D); });
        I.set_callback([&](){ pid.SetTunings(P, I, D); });
        D.set_callback([&](){ pid.SetTunings(P, I, D); });
    }
}
