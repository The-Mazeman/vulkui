#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "math.hpp"
#include "graphicsVulkan.hpp"

START_SCOPE(compositePass)

struct State
{
    vulkan::Buffer indexBuffer;
    vulkan::Buffer vertexBuffer;

    VkRenderPass renderPass;
    VkFramebuffer frameBuffer;
	VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
};

void create(vulkan::Device* device, vulkan::Image* swapchainImage, vulkan::Image* compositeImage, void** compositePass);
void execute(void* compositePass, VkCommandBuffer commandBuffer);

END_SCOPE
