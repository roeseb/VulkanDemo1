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

    /*vedPipeline::vedPipeline(VedDevice &device,
                             const std::string &vertexFileName,
                             const std::string &fragFileName,
                             const PipelineConfigInfo &configInfo) : vedDevice{device}
    {
        createGraphicsPipeline(device, vertexFileName, fragFileName, configInfo);
    }*/
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

        std::cout << "Vertex size: " << vertCode.size() << std::endl;
        std::cout << "Frag size: " << fragCode.size() << std::endl;
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
        return configInfo;
    }
}