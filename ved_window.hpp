#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace ved
{

    class vedWindow
    {
    public:
        vedWindow(int w, int h, std::string name);
        ~vedWindow();

        vedWindow(const vedWindow &) = delete;
        vedWindow &operator=(const vedWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); };
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        GLFWwindow *window;

        int width;
        int height;
        std::string name;

        void init();
    };

}