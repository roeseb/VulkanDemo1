#pragma once
#include "ved_window.hpp"
#include "ved_pipeline.hpp"
#include "ved_device.hpp"
#include "ved_swapchain.hpp"

#include <memory>
#include <stdexcept>
#include <vector>
#include <array>

#define DEBUG

namespace ved
{
    namespace Demos
    {
        class SimpleDemo
        {
        public:
            SimpleDemo();
            ~SimpleDemo();
            SimpleDemo(const SimpleDemo&) = delete;
            SimpleDemo &operator=(const SimpleDemo&) = delete;

            void run();

            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

        private:
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void drawFrame();

            vedWindow window{800, 600, std::string(appname)};
            VedDevice device{this->window};
            vedSwapchain swapchain{device, window.getExtent()};
            std::unique_ptr<vedPipeline> pipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;

            static constexpr char* appname = "Easy Vulkan Demo";
        };
    }
}