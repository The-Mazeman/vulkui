#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "math.hpp"
#include "graphicsVulkan.hpp"

START_SCOPE(quadPass)

struct Quad
{
    vec2 vertices[4];
};
struct State
{	
    vulkan::Buffer indexBuffer;
    vulkan::Buffer vertexBuffer;
    vulkan::Buffer entityIndexBuffer;

    vec2* vertices;
    uint* entityIndices;

    uint instanceCount;
    uint pad;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    VkPipeline depthPipeline;
};

void create(vulkan::Device* device, VkRenderPass renderPass, VkDescriptorSet copyDescriptorSet, void** quadPass);
void execute(void* quadPass, VkCommandBuffer commandBuffer);
void executeDepthPrePass(void* quadPass, VkCommandBuffer commandBuffer);
void addQuad(void* quadPass, Quad* quad, uint entityId);

END_SCOPE
