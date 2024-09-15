#include "setup.h"

namespace setup_sequence
{
    void hall(String message)
    {
        if (message == "setup")
            hall::setup(parameter::update_magnetic);
        else if (message == "shutdown")
            hall::shutdown();
    }

    void motor(String message)
    {
        if (message == "setup")
            motor::setup();
        else if (message == "shutdown")
            motor::shutdown();
    }

    void line(String message)
    {
        if (message == "setup")
            line::setup();
        else if (message == "shutdown")
            line::shutdown();
    }

    void steering(String message)
    {
        if (message == "setup")
            steering::setup();
        else if (message == "shutdown")
            steering::shutdown();
    }

    void parameter(String message)
    {
        if (message == "setup")
            parameter::setup();
        else if (message == "shutdown")
            parameter::shutdown();
    }

    void all(String message)
    {
        Serial.println("[car] doing setup");
        hall(message);
        motor(message);
        line(message);
        steering(message);
        parameter(message);
    }

    void setup()
    {
        all("setup"); // Do all setup
    }
}