#include "demo.hpp"

namespace ved
{
    namespace Demos
    {
        SimpleDemo::SimpleDemo()
        {
            createPipelineLayout();
            createPipeline();
            createCommandBuffers();
        }

        SimpleDemo::~SimpleDemo()
        {
            vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
        }

        void SimpleDemo::run()
        {
            while (!window.shouldClose())
            {
                glfwPollEvents();
                drawFrame();
            }

            vkDeviceWaitIdle(device.device());
        }

        void SimpleDemo::createPipelineLayout()
        {
            VkPipelineLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            layoutInfo.setLayoutCount = 0;
            layoutInfo.pSetLayouts = nullptr;
            layoutInfo.pushConstantRangeCount = 0;
            layoutInfo.pPushConstantRanges = nullptr;

            if (vkCreatePipelineLayout(device.device(), &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
                throw std::runtime_error("Failed creating pipeline layout");
        }

        void SimpleDemo::createPipeline()
        {
            auto defaultConfig = vedPipeline::defaultPipelineConfigInfo(swapchain.width(), swapchain.height());
            defaultConfig.renderPass = swapchain.getRenderPass();
            defaultConfig.pipelineLayout = pipelineLayout;

            pipeline = std::make_unique<vedPipeline>(
                device,
                "shaders/simple_shader.vert.spv",
                "shaders/simple_shader.frag.spv",
                defaultConfig);
        }

        void SimpleDemo::createCommandBuffers()
        {
            commandBuffers.resize(swapchain.imageCount());

            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = device.getCommandPool();
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

            if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
                throw std::runtime_error("Failed to allocate command buffers!");

            for (int i = 0; i < commandBuffers.size(); i++)
            {
                VkCommandBufferBeginInfo beginInfo{};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

                if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
                    throw std::runtime_error("Failed to begin recording command buffer!");

                VkRenderPassBeginInfo renderPassInfo{};
                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                renderPassInfo.renderPass = swapchain.getRenderPass();
                renderPassInfo.framebuffer = swapchain.getFrameBuffer(i);
                renderPassInfo.renderArea.offset = {0, 0};
                renderPassInfo.renderArea.extent = swapchain.getSwapChainExtent();

                std::array<VkClearValue, 2> clearValues{};
                clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
                clearValues[1].depthStencil = {1.0f, 0};
                renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
                renderPassInfo.pClearValues = clearValues.data();

                vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
                pipeline->bind(commandBuffers[i]);
                vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
                vkCmdEndRenderPass(commandBuffers[i]);

                if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
                    throw std::runtime_error("Failed ending command buffer");
            }
        }

        void SimpleDemo::drawFrame()
        {
            uint32_t imageIndex;
            VkResult result = swapchain.acquireNextImage(&imageIndex);
            if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
                throw std::runtime_error("Failed acquiring next image");

            result = swapchain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
            if(result != VK_SUCCESS)
                throw std::runtime_error("failed to present swapchain image");
        }
    }
}