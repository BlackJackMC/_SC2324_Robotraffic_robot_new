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

        cloud::add(P, Permission::ReadWrite, [&](){Serial.println("[pid]: " + String(P)); pid.SetTunings(P, I, D); });
        cloud::add(I, Permission::ReadWrite, [&](){Serial.println("[pid]: " + String(I)); pid.SetTunings(P, I, D); });
        cloud::add(D, Permission::ReadWrite, [&](){Serial.println("[pid]: " + String(D)); pid.SetTunings(P, I, D); });
        cloud::add(input, Permission::Read);
        cloud::add(output, Permission::Read);
        cloud::add(speed, Permission::ReadWrite);
        cloud::add(direction, Permission::ReadWrite);
        cloud::add(angle, Permission::ReadWrite);
    }
}
