#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "math.hpp"
#include "graphicsVulkan.hpp"

START_SCOPE(textureQuadPass)

struct Quad
{
    vec2 vertices[4];
};
struct State
{
    vulkan::Buffer indexBuffer;
    vulkan::Buffer vertexBuffer;
    vulkan::Buffer textureVertexBuffer;
    vulkan::Buffer entityIndexBuffer;
    vulkan::Buffer textureIndexBuffer;

    vec2* vertices;
    vec2* textureVertices;
    uint* entityIndices;
    uint* textureIndices;

    uint instanceCount;
    uint pad;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    VkPipeline depthPipeline;
};

void create(vulkan::Device* device, VkRenderPass renderPass, VkDescriptorSet copyDescriptorSet, void** textureQuadPass);
void addQuad(void* quadPass, Quad* quad, uint entityTag, uint textureTag);
void addTextureCoordinate(void* quadPass, Quad* quad, uint textureEntityTag);
void execute(void* quadPass, VkCommandBuffer commandBuffer);
void executeDepthPrePass(void* quadPass, VkCommandBuffer commandBuffer);

END_SCOPE
