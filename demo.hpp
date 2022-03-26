#pragma once
#include "ved_window.hpp"
#include "ved_pipeline.hpp"
#include "ved_device.hpp"
#include "ved_swap_chain.hpp"

#include <memory>
#include <vector>

namespace ved
{
    class Demo
    {

    public:
        void run();

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        Demo();
        ~Demo();

        Demo(const Demo &) = delete;
        Demo &operator=(const Demo &) = delete;

    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        VedWindow window{800, 600, "Vulkan Demo"};
        VedDevice device{window};
        VedSwapChain swapChain{device, window.getExtent()};
        /* VedPipeline pipeline{device,
                              "shaders/simple_shader.vert.spv",
                              "shaders/simple_shader.frag.spv",
                              VedPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};*/

        std::unique_ptr<VedPipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}