#include "controller.h"

namespace controller
{
    int speed;
    int direction;
    double P, I, D, input, output, setpoint;
    int angle;

    bool checkpoint_passed = false;
    
    PID pid(&input, &output, &setpoint, P, I, D, !direction);


    void update_angle()
    {
        input = line::sensor.readLineBlack(line::value);
        pid.Compute();

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

        properties::simple_cloud.add(P, Permission::ReadWrite, Priority::Cloud, Update_Policy::Change, [&](){ pid.SetTunings(P, I, D); });
        properties::simple_cloud.add(I, Permission::ReadWrite, Priority::Cloud, Update_Policy::Change, [&](){ pid.SetTunings(P, I, D); });
        properties::simple_cloud.add(D, Permission::ReadWrite, Priority::Cloud, Update_Policy::Change, [&](){ pid.SetTunings(P, I, D); });
        properties::simple_cloud.add(input, Permission::Read, Priority::Local, Update_Policy::Demand);
        properties::simple_cloud.add(output, Permission::Read, Priority::Local, Update_Policy::Demand);
        properties::simple_cloud.add(speed, Permission::ReadWrite, Priority::Local);
        properties::simple_cloud.add(direction, Permission::ReadWrite, Priority::Cloud);
        properties::simple_cloud.add(angle, Permission::ReadWrite, Priority::Cloud);
    }
}
