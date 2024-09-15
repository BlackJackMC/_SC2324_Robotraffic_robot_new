#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include "cloud_variable.h"
#include "hall.h"
#include "motor.h"
#include "line.h"
#include "steering.h"
#include "parameter.h"

namespace setup_sequence
{
    void hall(String message);
    void motor(String message);
    void line(String message);
    void steering(String message);
    void parameter(String message);
    void all(String message);
    void setup();
}

#endif