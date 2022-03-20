#pragma once
#include "ved_window.hpp"
#include "ved_pipeline.hpp"
#include "ved_device.hpp"

namespace ved
{
    class Demo
    {

    public:
        void run();

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

    private:
        vedWindow window{800, 600, "Vulkan Demo"};
        VedDevice device{window};
        vedPipeline pipeline{device,
                             "shaders/simple_shader.vert.spv",
                             "shaders/simple_shader.frag.spv",
                             vedPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}