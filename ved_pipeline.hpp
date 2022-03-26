#pragma once

#include <string>
#include <vector>

#include "ved_device.hpp"

namespace ved
{

    struct PipelineConfigInfo
    {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class vedPipeline
    {
    public:
        vedPipeline(VedDevice &device,
                    const std::string &vertexFileName,
                    const std::string &fragFileName,
                    const PipelineConfigInfo &configInfo);
        ~vedPipeline();
        vedPipeline(const vedPipeline &) = delete;
        void operator=(const vedPipeline &) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        VedDevice &vedDevice;
        VkPipeline graphicsPipeLine;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);
        std::vector<char> readFile(const std::string &filePath);
        void createGraphicsPipeline(const VedDevice &device, const std::string &vertexFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo);
    };
}