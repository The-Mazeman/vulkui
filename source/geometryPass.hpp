#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "math.hpp"
#include "graphicsVulkan.hpp"
#include "quadPass.hpp"
#include "textureQuadPass.hpp"

START_SCOPE(geometryPass)

struct State
{
    VkRenderPass renderPass;
    VkFramebuffer frameBuffer;

    void* quadPass;
    void* textureQuadPass;
};

void create(vulkan::Device* device, vulkan::Image* compositeImage, vulkan::Image* depthImage, VkDescriptorSet descriptorSet, void** geometryRenderer);
void execute(void* geometryPass, uint width, uint height, VkCommandBuffer commandBuffer);
void addTextureQuad(void* geometryPass, float x, float y, float width, float height, uint entityTag, uint textureTag);
void addQuad(void* geometryPass, float x, float y, float width, float height, uint entityTag);
void addTextureCoordinate(void* geometryPass, float x, float y, float width, float height, uint textureEntityTag);

END_SCOPE
