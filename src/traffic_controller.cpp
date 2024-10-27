#include "traffic_controller.h"

namespace traffic_controller
{
    CloudVar<String> west;
    CloudVar<String> south;
    CloudVar<String> start;
    std::map<String, bool> decision = {
        {"red", false},
        {"red yellow", false},
        {"green", true},
        {"green blink1", true},
        {"green blink2", true},
        {"green blink3", false},
        {"green blink4", false},
        {"yellow", false}
    };
    int counter = 0;

    void next_checkpoint()
    {
        if (counter >= 2)
            counter = 1;
        else counter = counter + 1;
    }

    bool allow()
    {
        switch (counter)
        {
        case 0:
            return decision[start];
        case 1:
            return decision[west];
        case 2:
            return decision[south];
        default:
            break;
        }

        return false;
    }
}