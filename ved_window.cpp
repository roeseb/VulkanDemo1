#include "ved_window.hpp"

#include <stdexcept>

namespace ved {
    VedWindow::VedWindow(int w, int h, std::string myName) : width{w}, height{h}, name{myName} {
        init();    
    }

    VedWindow::~VedWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void VedWindow::init() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    }

    void VedWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create  window surface");
        }
    }
}

