#include "loop_controller.h"

namespace loop_controller
{
    const String mode[] = {"test", "real"}; // For documenting
    String current_mode = mode[0];
    int current_step = 0;

    std::map<String, voidFuncPtr> executor = {
        {"test", loop_sequence::test::loop},
        {"real", loop_sequence::real::loop}};
    
    void change_mode(String mode)
    {
        current_mode = mode;
        reset();
    }

    void reset()
    {
        current_step = 0;
    }
}
