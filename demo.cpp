#include "demo.hpp"

namespace ved
{
    void Demo::run()
    {
        while (!window.shouldClose())
        {
            glfwPollEvents();
        }
    }
}