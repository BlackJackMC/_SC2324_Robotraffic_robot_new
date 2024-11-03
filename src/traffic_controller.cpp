#include "traffic_controller.h"

namespace traffic_controller
{
    String south, west, start;
    String* traffic_light_list[3];

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
        if (counter++ > 2) counter = 1;
    }

    bool allow()
    {
        return decisions[*traffic_light_list[counter]];
    }

    void setup()
    {
        properties::simple_cloud.add(south, Permission::ReadWrite, Priority::Cloud, Update_Policy::None);
        properties::simple_cloud.add(west, Permission::ReadWrite, Priority::Cloud, Update_Policy::None);
        properties::simple_cloud.add(start, Permission::ReadWrite, Priority::Cloud, Update_Policy::None);
    }
}