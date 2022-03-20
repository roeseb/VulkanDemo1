#!/bin/bash

~/Development/VulkanSDK/x86_64/bin/glslc shaders/simple_shader.vert -o shaders/simple_shader.vert.spv
~/Development/VulkanSDK/x86_64/bin/glslc shaders/simple_shader.frag -o shaders/simple_shader.frag.spv