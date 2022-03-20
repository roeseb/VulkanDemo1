#include "ved_window.hpp"

namespace ved {
    vedWindow::vedWindow(int w, int h, std::string myName) : width{w}, height{h}, name{myName} {
        init();    
    }

    vedWindow::~vedWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void vedWindow::init() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    }
}

