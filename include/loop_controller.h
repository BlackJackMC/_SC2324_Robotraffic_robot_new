#ifndef LOOP_CONTROLLER_H
#define LOOP_CONTROLLER_H

#include <Arduino.h>
#include <map>

#include "loop_sequence.h"

namespace loop_controller
{
    extern const String mode[];
    extern String current_mode;
    extern int current_step;
    extern std::map<String, voidFuncPtr> executor;
    void change_mode(String mode);
    void reset();
}

#endif