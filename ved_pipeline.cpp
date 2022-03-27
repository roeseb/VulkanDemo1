#include "ved_pipeline.hpp"

#include <fstream>
#include <iostream>

namespace ved
{
    vedPipeline::vedPipeline(VedDevice &device,
                             const std::string &vertexFileName,
                             const std::string &fragFileName,
                             const PipelineConfigInfo &configInfo) : vedDevice{device}
    {
        createGraphicsPipeline(device, vertexFileName, fragFileName, configInfo);
    }

    vedPipeline::~vedPipeline()
    {
        vkDestroyShaderModule(vedDevice.device(), vertShaderModule, nullptr);
        vkDestroyShaderModule(vedDevice.device(), fragShaderModule, nullptr);
        vkDestroyPipeline    (vedDevice.device(), graphicsPipeLine, nullptr);
    }

    std::vector<char> vedPipeline::readFile(const std::string &filePath)
    {
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            throw std::runtime_error("failed top open file " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    void vedPipeline::createGraphicsPipeline(const VedDevice &device, const std::string &vertexFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo)
    {
        auto vertCode = readFile(vertexFilePath);
        auto fragCode = readFile(fragFilePath);

        assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "can't create pipeline: no layout provided!");
        assert(configInfo.renderPass != VK_NULL_HANDLE && " can't create pipeline: no renderpass provided!");
        createShaderModule(vertCode, &vertShaderModule);
        createShaderModule(fragCode, &fragShaderModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[0].pName = "main";
        shaderStages[0].module = vertShaderModule;
        shaderStages[0].flags = 0;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;
        shaderStages[1].pName = "main";
        shaderStages[1].module = fragShaderModule;
        shaderStages[1].flags = 0;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;

        // This struct combines the viewport and scissors together
        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &configInfo.viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &configInfo.scissor;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pStages = shaderStages;

        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
        pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
        pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
        pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = configInfo.pipelineLayout;
        pipelineInfo.renderPass = configInfo.renderPass;
        pipelineInfo.subpass = configInfo.subpass;
        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if(vkCreateGraphicsPipelines(vedDevice.device(), nullptr, 1, &pipelineInfo, nullptr, &graphicsPipeLine) != VK_SUCCESS)
            throw std::runtime_error("Failed creating Graphics Pipeline");
    }

    void vedPipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        if (vkCreateShaderModule(vedDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        {
            throw new std::runtime_error("failed to create shader module");
        }
    }

    PipelineConfigInfo vedPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};
        // Specify what type of geometry we want to render
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        // Define the viewport or 'camera' as i understand it...
        configInfo.viewport.x = 0.0f;
        configInfo.viewport.y = 0.0f;
        configInfo.viewport.width = static_cast<float>(width);
        configInfo.viewport.height = static_cast<float>(height);
        configInfo.viewport.minDepth = 0.0f;
        configInfo.viewport.maxDepth = 1.0f;

        // the scissors define what of the image should be 'cut off'
        configInfo.scissor.extent = {width, height};
        configInfo.scissor.offset = {0,0};

        // The rasterization info contains info about culling, anti aliasing (depth bias), draw mode like fill in the shape
        // (polygonMode) and, linewidth. depthClamp is for clamping the Z-value. e.g. negative position is clamped to 0, values greater than 1 will be clamped to 1
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = 1.0f; 
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT; // VK_CULL_MODE_NONE suggested. but im too nerdy
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.f; // optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.f;          // optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.f;    // optional

        // Also anti aliasing somehow
        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.f;
        configInfo.multisampleInfo.pSampleMask = nullptr;
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

        // How do we combine colors, e.g if 2 triangles are overlapping?
        configInfo.colorBlendAttachment.colorWriteMask = 
            VK_COLOR_COMPONENT_R_BIT | 
            VK_COLOR_COMPONENT_G_BIT | 
            VK_COLOR_COMPONENT_B_BIT | 
            VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendInfo.blendConstants[0] = 0.f;
        configInfo.colorBlendInfo.blendConstants[1] = 0.f;
        configInfo.colorBlendInfo.blendConstants[2] = 0.f;
        configInfo.colorBlendInfo.blendConstants[3] = 0.f;

        // This basically finds out the depth of every pixel
        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.f;
        configInfo.depthStencilInfo.maxDepthBounds = 1.f;
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};
        configInfo.depthStencilInfo.back = {};

        return configInfo;
    }

    void vedPipeline::bind(VkCommandBuffer cmd)
    {
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeLine);
    }
}