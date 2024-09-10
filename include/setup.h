#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>

namespace setup_sequence
{
    extern int count;
    extern const int required;
    extern bool enabled;

    void send_state();
    void hall(String message);
    void motor(String message);
    void line(String message);
    void steering(String message);
    void parameter(String message);
    void all(String message);
    void setup();
}

#endif