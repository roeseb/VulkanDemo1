#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
namespace ved
{

    class VedWindow
    {
    public:
        VedWindow(int w, int h, std::string name);
        ~VedWindow();

        VedWindow(const VedWindow &) = delete;
        VedWindow &operator=(const VedWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); };
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        VkExtent2D getExtent()
        {
            std::cout << "Extent is " << width << "x" << height << std::endl;
            return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        }

    private:
        GLFWwindow *window;

        const int width;
        const int height;
        std::string name;

        void init();
    };

}