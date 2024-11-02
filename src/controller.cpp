#include "controller.h"

namespace controller
{
    int speed;
    int direction;
    float P;
    float I;
    float D;
    int angle;
    float input;
    float output;
    int setpoint;

    double local_input, local_output, local_setpoint;
    bool checkpoint_passed = false;
    
    PID pid(&local_input, &local_output, &local_setpoint, P, I, D, !direction);


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
    }
}
