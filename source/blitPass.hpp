#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "math.hpp"
#include "graphicsVulkan.hpp"

START_SCOPE(blitPass)

struct State
{
    vulkan::Image colorBlitImage;
    vulkan::Image indexBlitImage;
    vulkan::Image compositeImage;
    vulkan::Image entityIndexImage;
};
void create(vulkan::Device* device, vulkan::Image* compositeImage, vulkan::Image* entityIndexSurface, void** blitPass);
void execute(void* blitPass, VkCommandBuffer commandBuffer, Window* window, int deltaX, int deltaY);

END_SCOPE
