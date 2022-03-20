#pragma once
#include "ved_window.hpp"

namespace ved
{
    class Demo
    {
        public: void run();
    private:
        vedWindow window{800, 600, "Vulkan Demo"};
    }; 
}