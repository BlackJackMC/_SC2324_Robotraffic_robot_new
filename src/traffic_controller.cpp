#include "traffic_controller.h"

namespace traffic_controller
{
    std::array<String, 3> traffic_light_list;
    std::map<String, bool> decisions = {
        {"red", false},
        {"red yellow", false},
        {"green", true},
        {"green blink1", true},
        {"green blink2", true},
        {"green blink3", false},
        {"green blink4", false},
        {"yellow", false}};
    int counter = 0;

    void next_checkpoint()
    {
        if (counter >= 2)
            counter = 1;
        else
            counter = counter + 1;
    }

    bool allow()
    {
        return decisions[traffic_light_list[counter]];
    }

    void setup()
    {
        cloud::add(traffic_light_list[0], Permission::ReadWrite, Priority::Cloud, Update_Policy::None);
        cloud::add(traffic_light_list[1], Permission::ReadWrite, Priority::Cloud, Update_Policy::None);
        cloud::add(traffic_light_list[2], Permission::ReadWrite, Priority::Cloud, Update_Policy::None);
    }
}