#include "parameter.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <PID_v1.h>
#include <map>
#include "cloud_variable.h"


#include "mqtt.h"

#include "line.h"
#include "steering.h"
#define get_value(var) (#var "=" + String(var)).c_str()

namespace parameter
{
    CloudVar<bool> can_go("can_go");
    CloudVar<int> speed("speed");
    CloudVar<int> direction("direction");
    CloudVar<float> P("P");
    CloudVar<float> I("I");
    CloudVar<float> D("D");
    CloudVar<int> angle("angle");
    CloudVar<float> input("input");
    CloudVar<float> output("input");
    CloudVar<int> setpoint("setpoint");
    CloudVar<int> checkpoint("checkpoint");

    double local_input, local_output, local_setpoint;
    PID pid(&local_input, &local_output, &local_setpoint, P.get(), I.get(), D.get(), !direction.get());

    bool magnetic = false;

    void update_angle()
    {
        local_input = line::sensor.readLineBlack(line::value);
        input.set(local_input);
        pid.Compute();
        output.set(local_output);

        angle.set(constrain(angle.get() + map(output.get(), 0, 4000, 0, 150), 0, 150));
    }

    void update_magnetic()
    {
        magnetic = true;
    }

    void setup()
    {
        pid.SetMode(AUTOMATIC);
        pid.SetOutputLimits(0, 4000);

        angle.set_callback([&](){
            steering::turn(angle.get());
        });

        P.set_callback([&](){ pid.SetTunings(P.get(), I.get(), D.get()); });
        I.set_callback([&](){ pid.SetTunings(P.get(), I.get(), D.get()); });
        D.set_callback([&](){ pid.SetTunings(P.get(), I.get(), D.get()); });
    }

    void shutdown()
    {

    }
}
