#include "include.hpp"
#include "renderer.hpp"

START_SCOPE(renderer)

void createBuffer(State* state)
{
    vulkan::Device* device = &state->device;

    uint width = state->swapchain.image.width;
    uint height = state->swapchain.image.height;

    vulkan::Buffer windowClipPlanesBuffer = {};
    windowClipPlanesBuffer.size = sizeof(vec4) * 128;
    windowClipPlanesBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    windowClipPlanesBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    windowClipPlanesBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &windowClipPlanesBuffer);
    state->windowClipPlanesBuffer = windowClipPlanesBuffer;

    vec4* windowClipPlanes = {};
    mapBuffer(device, &windowClipPlanesBuffer, (void**)&windowClipPlanes);
    state->windowClipPlanes = windowClipPlanes;

    vulkan::Buffer entityBuffer = {};
    entityBuffer.size = sizeof(Entity) * 128;
    entityBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    entityBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    entityBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &entityBuffer);
    state->entityBuffer = entityBuffer;

    Entity* entities = {};
    mapBuffer(device, &entityBuffer, (void**)&entities);
    state->entities = entities;

    vulkan::Buffer colorBuffer = {};
    colorBuffer.size = sizeof(vec4) * 32;
    colorBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    colorBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    colorBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &colorBuffer);
    state->colorBuffer = colorBuffer;

    vec4* colors = {};
    mapBuffer(device, &colorBuffer, (void**)&colors);
    state->colors = colors;

    colors[0] = {{0.0f, 0.0f, 0.0f, 1.0f}};
    colors[1] = {{1.0f, 1.0f, 1.0f, 1.0f}};
    colors[2] = {{1.0f, 0.0f, 0.0f, 1.0f}};
    colors[3] = {{0.0f, 1.0f, 0.0f, 1.0f}};
    colors[4] = {{0.0f, 0.0f, 0.0f, 0.0f}};
    colors[5] = {{0.2f, 0.2f, 0.2f, 1.0f}};

}
void createImage(State* state)
{
    vulkan::Device* device = &state->device;

    START_BLOCK

    uint usage =
    {
        VK_IMAGE_USAGE_SAMPLED_BIT | 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | 
        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_STORAGE_BIT
    };

    vulkan::Image* compositeImage = &state->compositeImage;
    compositeImage->usage = usage;
    compositeImage->format = state->swapchain.surfaceFormat.format;
    compositeImage->width = state->swapchain.image.width;
    compositeImage->height = state->swapchain.image.height;
    compositeImage->memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    compositeImage->tiling = VK_IMAGE_TILING_OPTIMAL;
    compositeImage->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    vulkan::createImage(device, compositeImage);
    uint newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    vulkan::transitionImage(device, compositeImage, newLayout);

    END_BLOCK

    START_BLOCK

    uint usage =
    {
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
        VK_IMAGE_USAGE_STORAGE_BIT
    };

    vulkan::Image* entityIndexImage = &state->entityIndexImage;
    entityIndexImage->usage = usage;
    entityIndexImage->format = VK_FORMAT_R32_UINT;
    entityIndexImage->width = state->swapchain.image.width;
    entityIndexImage->height = state->swapchain.image.height;
    entityIndexImage->memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    entityIndexImage->tiling = VK_IMAGE_TILING_LINEAR;
    entityIndexImage->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    vulkan::createImage(device, entityIndexImage);
    uint newLayout = VK_IMAGE_LAYOUT_GENERAL;
    vulkan::transitionImage(device, entityIndexImage, newLayout);

    uint* entityIndexSurface = {};
    vulkan::mapImage(device, entityIndexImage, (void**)&entityIndexSurface);
    state->entityIndexSurface = entityIndexSurface;

    END_BLOCK

    START_BLOCK

    uint usage =
    {
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
    };

    vulkan::Image* depthImage = &state->depthImage;
    depthImage->usage = usage;
    depthImage->format = VK_FORMAT_D32_SFLOAT;
    depthImage->width = state->swapchain.image.width;
    depthImage->height = state->swapchain.image.height;
    depthImage->memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    depthImage->tiling = VK_IMAGE_TILING_OPTIMAL;
    depthImage->aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    vulkan::createImage(device, depthImage);
    uint newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    vulkan::transitionImage(device, depthImage, newLayout);

    END_BLOCK

    START_BLOCK

    uint usage =
    {
        VK_IMAGE_USAGE_SAMPLED_BIT | 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT 
    };

    vulkan::Image* fontImage = &state->fontImage;
    fontImage->usage = usage;
    fontImage->format = VK_FORMAT_R8G8B8A8_UNORM;
    fontImage->width = 128;
    fontImage->height = 64;
    fontImage->memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    fontImage->tiling = VK_IMAGE_TILING_OPTIMAL;
    fontImage->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    vulkan::createImage(device, fontImage);
    uint newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    vulkan::transitionImage(device, fontImage, newLayout);

    WCHAR fontPath[] = L"C:\\Users\\administrator-1\\Desktop\\renderer\\font.bmp";
    void* file = {};
    uint64 fileSize = {};
    loadFile(fontPath, &file, &fileSize);

    uint8* pixel = {};
    parserBmp::parse(file, (void**)&pixel);

    WCHAR fontMetrics[] = L"C:\\Users\\administrator-1\\Desktop\\renderer\\font.fnt";
    void* metricFile = {};
    uint64 metricSize = {};
    loadFile(fontMetrics, &metricFile, &metricSize);

    GlyphInfo* glyphInfo = {};
    KerningInfo* kerningInfo = {};
    parserFontMetrics::parse(metricFile, &glyphInfo, &kerningInfo);
    state->glyphInfo = glyphInfo;
    state->kerningInfo = kerningInfo;

    vulkan::Buffer fontBuffer = {};
    fontBuffer.size = fontImage->width * fontImage->height * sizeof(uint);
    fontBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    fontBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    fontBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &fontBuffer);

    void* fontMemory = {};
    mapBuffer(device, &fontBuffer, (void**)&fontMemory);
    memcpy(fontMemory, pixel, fontBuffer.size); 

    fillDeviceImage(device, fontImage, fontBuffer.buffer);
    vulkan::transitionImage(device, fontImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    END_BLOCK
}
void createDescriptor(State* state)
{
    VkDevice logicalDevice = state->device.logicalDevice;

    uint stages = 
    {
        VK_SHADER_STAGE_VERTEX_BIT |
        VK_SHADER_STAGE_FRAGMENT_BIT
    };

    VkDescriptorSetLayoutBinding windowClipPlaneBinding = {};
	vulkan::createDescriptorLayoutBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &windowClipPlaneBinding);

    VkDescriptorSetLayoutBinding entityBinding = {};
	vulkan::createDescriptorLayoutBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &entityBinding);

    VkDescriptorSetLayoutBinding colorBinding = {};
	vulkan::createDescriptorLayoutBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &colorBinding);

    VkDescriptorSetLayoutBinding surfaceBinding = {};
	vulkan::createDescriptorLayoutBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, stages, &surfaceBinding);

    VkDescriptorSetLayoutBinding fontBinding = {};
	vulkan::createDescriptorLayoutBinding(4, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, stages, &fontBinding);


    VkDescriptorSetLayoutBinding layoutBindings[] =
	{
        entityBinding,
        colorBinding,
        surfaceBinding,
        fontBinding,
        windowClipPlaneBinding,
    };

	VkDescriptorSetLayout descriptorSetLayout = {};
	vulkan::createDescriptorSetLayout(logicalDevice, layoutBindings, 5, &descriptorSetLayout);
    state->descriptorSetLayout = descriptorSetLayout;

    uint descriptorTypes[] =
	{
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
    };

	uint descriptorCounts[] =
	{
		3,
		1,
		1,
    };

    VkDescriptorPool descriptorPool = {};
	vulkan::createDescriptorPool(logicalDevice, descriptorTypes, descriptorCounts, 3, &descriptorPool);

    VkDescriptorSet descriptorSet = {};
	vulkan::createDescriptorSet(logicalDevice, descriptorPool, descriptorSetLayout, &descriptorSet);
    state->descriptorSet = descriptorSet;

    VkBuffer windowClipPlanesBuffer = state->windowClipPlanesBuffer.buffer;
    vulkan::updateDescriptorSet(logicalDevice, 0, windowClipPlanesBuffer, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

    VkBuffer entityBuffer = state->entityBuffer.buffer;
    vulkan::updateDescriptorSet(logicalDevice, 1, entityBuffer, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

    VkBuffer colorBuffer = state->colorBuffer.buffer;
    vulkan::updateDescriptorSet(logicalDevice, 2, colorBuffer, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

    START_BLOCK

    VkImageView imageView = state->entityIndexImage.view;
    uint layout = VK_IMAGE_LAYOUT_GENERAL;

    VkDescriptorImageInfo descriptorImageInfo = {};
    vulkan::createDescriptorImageInfo(0, imageView, layout, &descriptorImageInfo);

    uint type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    vulkan::updateDescriptorSet(logicalDevice, 3, &descriptorImageInfo, descriptorSet, type);

    END_BLOCK

    START_BLOCK

    VkImageView imageView = state->fontImage.view;
    uint layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkDescriptorImageInfo descriptorImageInfo = {};
    vulkan::createDescriptorImageInfo(0, imageView, layout, &descriptorImageInfo);

    uint type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    vulkan::updateDescriptorSet(logicalDevice, 4, &descriptorImageInfo, descriptorSet, type);

    END_BLOCK

}
void create(HWND windowHandle, HINSTANCE windowInstance, void** renderer) 
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->entityCount = 0;
    state->windowCount = 0;
    state->textureEntityCount = 0;
    state->freeEntityCount = 0;
    state->reuseEntityCount = 0;

    *renderer = state;

    VkInstance instance = {};
    vulkan::createInstance(&instance);
    state->device.instance = instance;

    VkDebugUtilsMessengerEXT debugMessenger = {};
    vulkan::createDebugMessageCallback(instance, &debugMessenger);
    state->device.debugMessenger = debugMessenger;

    VkSurfaceKHR surface = {};
    vulkan::createSurface(instance, windowHandle, windowInstance, &surface);
    state->swapchain.surface = surface;

    VkPhysicalDevice physicalDevice = {};
    uint queueFamilies[4] = {};
    vulkan::createPhysicalDevice(instance, surface, &physicalDevice, queueFamilies);
    state->device.physicalDevice = physicalDevice; 
    memcpy(&state->device.queueFamilies, queueFamilies, sizeof(uint) * 4);

    VkPhysicalDeviceMemoryProperties deviceMemoryProperties = {};
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);
    state->device.deviceMemoryProperties = deviceMemoryProperties;
    
    VkDevice logicalDevice = {};
    vulkan::createLogicalDevice(physicalDevice, queueFamilies, &logicalDevice);
    state->device.logicalDevice = logicalDevice;

    for(uint i = 0; i != 4; ++i)
    {
        VkQueue queue = {};
        vkGetDeviceQueue(logicalDevice, queueFamilies[0], 0, &queue);
        state->device.queues[i] = queue;

        VkCommandPool commandPool = {};
        vulkan::createCommandPool(logicalDevice, queueFamilies[0], &commandPool);
        state->device.commandPools[i] = commandPool;

        VkCommandBuffer commandBuffer = {};
        uint level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        vulkan::createCommandBuffer(logicalDevice, commandPool, level, &commandBuffer);
        state->device.commandBuffers[i] = commandBuffer;
    }

    vulkan::Device* device = &state->device;
    vulkan::Swapchain* swapchain = &state->swapchain;
    vulkan::createSwapchain(device, surface, swapchain);

    createImage(state);
    createBuffer(state);
    createDescriptor(state);

    vulkan::createSemaphore(logicalDevice, &state->semaphore.image);
	vulkan::createSemaphore(logicalDevice, &state->semaphore.execution);
	vulkan::createFence(logicalDevice, &state->fence.pipeline);

    void* geometryPass = {};
    geometryPass::create(device, &state->compositeImage, &state->depthImage, state->descriptorSet, &geometryPass);
    state->geometryPass = geometryPass;

    void* blitPass = {};
    blitPass::create(device, &state->compositeImage, &state->entityIndexImage, &blitPass);
    state->blitPass = blitPass;

    void* compositePass = {};
    compositePass::create(device, &state->swapchain.image, &state->compositeImage, &compositePass);
    state->compositePass = compositePass;
}
void draw(void* renderer)
{
    State* state = (State*)renderer;
	VkResult result = {};

    VkDevice logicalDevice = state->device.logicalDevice;
    uint imageIndex = {};
	VkFence pipelineFence = state->fence.pipeline;
    result = vkWaitForFences(logicalDevice, 1, &pipelineFence, VK_TRUE, UINT64_MAX);
    assert(result == VK_SUCCESS);
    
	VkSemaphore imageSemaphore = state->semaphore.image;
	VkSwapchainKHR swapchain = state->swapchain.swapchain;

    result = vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX, imageSemaphore, 0, &imageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR)
	{
	}

    result = vkResetFences(logicalDevice, 1, &pipelineFence);
    assert(result == VK_SUCCESS);

    VkCommandBuffer commandBuffer = state->device.commandBuffer.graphics;
    vkResetCommandBuffer(commandBuffer, 0);

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    uint width = state->swapchain.image.width;
    uint height = state->swapchain.image.height;

    VkViewport viewport = {};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = (float)width;
    viewport.height = (float)height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.extent.width = width;
    scissor.extent.height = height;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    void* geometryPass = state->geometryPass; 
    geometryPass::execute(geometryPass, width, height, commandBuffer);
    state->textureEntityCount = 0;

    void* compositePass = state->compositePass; 
    compositePass::execute(compositePass, commandBuffer);

    result = vkEndCommandBuffer(commandBuffer);
    assert(result == VK_SUCCESS);

    VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSemaphore executionSemaphore = state->semaphore.execution;

    START_BLOCK

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageSemaphore;
    submitInfo.pWaitDstStageMask = &pipelineStageFlags;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &executionSemaphore;

    VkQueue queue = state->device.queue.graphics;
    result = vkQueueSubmit(queue, 1, &submitInfo, pipelineFence);
    assert(result == VK_SUCCESS);

    END_BLOCK

    START_BLOCK

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = 0;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &executionSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = 0;

    VkQueue queue = state->device.queue.present;
    result = vkQueuePresentKHR(queue, &presentInfo);
    assert(result == VK_SUCCESS);

    vulkan::Image entityIndexImage = state->entityIndexImage;

    VkMappedMemoryRange memoryRange = {};
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.pNext = 0;
    memoryRange.memory = entityIndexImage.memory;
    memoryRange.offset = 0;
    memoryRange.size = entityIndexImage.size;

    vkInvalidateMappedMemoryRanges(logicalDevice, 1, &memoryRange);

    state->freeEntityCount += state->reuseEntityCount;
    state->reuseEntityCount = 0;

    vkQueueWaitIdle(queue);

    END_BLOCK

}
void getEntitiesUnderArea(void* renderer, uint windowTag, Window* coveredAreaX)
{
    State* state = (State*)renderer;
    ChildList* childList = &state->childLists[windowTag];
    getChildUnderArea(renderer, coveredAreaX, childList);
}
void getChildUnderArea(void* renderer, Window* window, ChildList* childList)
{
    State* state = (State*)renderer;
    uint swapchainWidth = state->swapchain.image.width;
    uint* indexSurface = state->entityIndexSurface;
    
    uint x = (uint)window->x;
    uint y = (uint)window->y;
    uint width = (uint)window->width;
    uint height = (uint)window->height;

    uint* entityList = childList->indices;
    uint entityCount = childList->count;
    for(uint i = y; i != y + height; ++i)
    {
        for(uint j = x; j != x + width; ++j)
        {
            uint pixelIndex =  (i * swapchainWidth) + j;
            uint entity = indexSurface[pixelIndex];
            uint reject = 0;
            for(uint k = 0; k != entityCount; ++k)
            {
                if(entity == entityList[k])
                {
                    reject = 1;
                    break;
                }
            }

            if(reject == 0)
            {
                entityList[entityCount] = entity;
                ++entityCount;
            }
        }
    }
    childList->count = entityCount;
}
void addQuad(void* renderer, float x, float y, float width, float height, uint entityId)
{
    State* state = (State*)renderer;
    float swapchainWidth = (float)state->swapchain.image.width;
    float swapchainHeight = (float)state->swapchain.image.height;

    x = (x / swapchainWidth) * 2.0f - 1.0f;
    y = (y / swapchainHeight) * 2.0f - 1.0f;
    width = (width / swapchainWidth) * 2.0f;
    height = (height / swapchainHeight) * 2.0f;

    void* geometryPass = state->geometryPass;
    geometryPass::addQuad(geometryPass, x, y, width, height, entityId);
}
void addQuad(void* renderer, Window* window, uint entityId)
{
    State* state = (State*)renderer;
    float swapchainWidth = (float)state->swapchain.image.width;
    float swapchainHeight = (float)state->swapchain.image.height;

    float x = (window->x / swapchainWidth) * 2.0f - 1.0f;
    float y = (window->y / swapchainHeight) * 2.0f - 1.0f;
    float width = (window->width / swapchainWidth) * 2.0f;
    float height = (window->height / swapchainHeight) * 2.0f;

    void* geometryPass = state->geometryPass;
    geometryPass::addQuad(geometryPass, x, y, width, height, entityId);
}
void addBorder(void* renderer, Window* window, float thickness, uint entityId)
{
    State* state = (State*)renderer;

    float x = window->x;
    float y = window->y;
    float width = window->width;
    float height = window->height;

    addQuad(renderer, x, y, thickness, height, entityId);
    addQuad(renderer, x, y, width, thickness, entityId);
    addQuad(renderer, x + width - thickness, y, thickness, height, entityId);
    addQuad(renderer, x, y + height - thickness, width, thickness, entityId);
}
void addTextureQuad(void* renderer, Window* window, uint entityTag, uint textureTag)
{
    State* state = (State*)renderer;
    float swapchainWidth = (float)state->swapchain.image.width;
    float swapchainHeight = (float)state->swapchain.image.height;

    float x = (window->x / swapchainWidth) * 2.0f - 1.0f;
    float y = (window->y / swapchainHeight) * 2.0f - 1.0f;
    float width = (window->width / swapchainWidth) * 2.0f;
    float height = (window->height / swapchainHeight) * 2.0f;

    void* geometryPass = state->geometryPass;
    geometryPass::addTextureQuad(geometryPass, x, y, width, height, entityTag, textureTag);
}
void createWindowEntity(void* renderer,  uint* windowTag)
{
    State* state = (State*)renderer;
    uint windowCount = state->windowCount;
    *windowTag = windowCount;
    state->childLists[windowCount] = {};
    ++state->windowCount;
}
void getWindowHandle(void* renderer, uint windowTag, HWND* windowHandle)
{
    State* state = (State*)renderer;
    *windowHandle = state->windowHandles[windowTag];
}
void sendMessage(void* renderer, uint windowTag, uint message, void* firstParameter, void* secondParameter)
{
    State* state = (State*)renderer;
    HWND windowHandle = state->windowHandles[windowTag];
    SendMessage(windowHandle, message, (WPARAM)firstParameter, (LPARAM)secondParameter);
}
void createEntity(void* renderer, uint* entityTag)
{
    State* state = (State*)renderer;
    uint freeEntityCount = state->freeEntityCount;
    if(freeEntityCount)
    {
        uint* freeEntities = state->freeEntities;
        *entityTag = freeEntities[freeEntityCount - 1];
        --state->freeEntityCount;
        return;
    }
    uint entityCount = state->entityCount;
    *entityTag = entityCount;
    ++state->entityCount;
}
void setWindowHandle(void* renderer, uint windowTag, HWND windowHandle)
{
    State* state = (State*)renderer;
    state->windowHandles[windowTag] = windowHandle;
}
void getWindowRelativePosition(void* renderer, uint windowTag, float* x, float* y)
{
    State* state = (State*)renderer;
    *x = state->relativePositions[windowTag].x;
    *y = state->relativePositions[windowTag].y;
}
void setWindowRelativePosition(void* renderer, uint windowTag, float x, float y)
{
    State* state = (State*)renderer;
    state->relativePositions[windowTag] = {x, y};
}
void setWindowClipPlanes(void* renderer, uint windowTag, Window* window)
{
    State* state = (State*)renderer;
    float swapchainWidth = (float)state->swapchain.image.width;
    float swapchainHeight = (float)state->swapchain.image.height;

    float clipPlaneLeft = -(((window->x / swapchainWidth) * 2.0f) - 1.0f);
    float clipPlaneTop = -(((window->y / swapchainHeight) * 2.0f) - 1.0f);
    float clipPlaneRight = ((((window->x + window->width) / swapchainWidth) * 2.0f) - 1.0f);
    float clipPlaneBottom = ((((window->y + window->height) / swapchainHeight) * 2.0f) - 1.0f);
    state->windowClipPlanes[windowTag] = {{clipPlaneLeft, clipPlaneTop, clipPlaneRight, clipPlaneBottom}};
}
void setWindowDimension(void* renderer, uint windowTag, Window* window)
{
    State* state = (State*)renderer;
    state->windowDimensions[windowTag] = *window;
}
void getWindowDimension(void* renderer, uint windowTag, Window* window)
{
    State* state = (State*)renderer;
    *window = state->windowDimensions[windowTag];
}
void setEntityWindow(void* renderer, uint entityTag, uint windowTag)
{
    State* state = (State*)renderer;
    Entity* entity = &state->entities[entityTag];
    entity->windowTag = windowTag;
}
void setEntityColor(void* renderer, uint entityTag, uint colorTag)
{
    State* state = (State*)renderer;
    Entity* entity = &state->entities[entityTag];
    entity->colorIndex = colorTag;
}
void setEntityDepth(void* renderer, uint entityTag, float depth)
{
    State* state = (State*)renderer;
    Entity* entity = &state->entities[entityTag];
    entity->depth = depth;
}
void createTextureEntity(void* renderer, uint* textureEntityTag)
{
    State* state = (State*)renderer;
    uint textureEntityCount = state->textureEntityCount;
    *textureEntityTag = textureEntityCount;
    ++state->textureEntityCount;
}
void setEntityTexture(void* renderer, uint entityTag, uint textureTag)
{
    State* state = (State*)renderer;
    Entity* entity = &state->entities[entityTag];
    entity->textureIndex = textureTag;
}
void setTextureEntityCoordinates(void* renderer, uint textureEntityTag, Window* texture)
{
    State* state = (State*)renderer;
    float textureWidth = (float)state->fontImage.width;
    float textureHeight = (float)state->fontImage.height;

    float x = (texture->x / textureWidth);
    float y = (texture->y / textureHeight);
    float width = (texture->width / textureWidth);
    float height = (texture->height / textureHeight);

    void* geometryPass = state->geometryPass;
    geometryPass::addTextureCoordinate(geometryPass, x, y, width, height, textureEntityTag);
}
void getGlyph(State* state, WCHAR character, float* advanceX, Window* glyph, Window* glyphTexture)
{
    GlyphInfo* glyphInfo = state->glyphInfo;
    uint glyphIndex = (uint)character;
    glyphIndex -= 31;

    glyphTexture->x = (float)glyphInfo[glyphIndex].x;
    glyphTexture->y = (float)glyphInfo[glyphIndex].y;
    glyphTexture->width = (float)glyphInfo[glyphIndex].width;
    glyphTexture->height = (float)glyphInfo[glyphIndex].height;
    *advanceX = (float)glyphInfo[glyphIndex].advanceX;

    glyph->x += (float)glyphInfo[glyphIndex].offsetX;
    glyph->y += (float)glyphInfo[glyphIndex].offsetY;
    glyph->width = (float)glyphInfo[glyphIndex].width;
    glyph->height = (float)glyphInfo[glyphIndex].height;
    
}
void getKerning(State* state, WCHAR previous, WCHAR current, float* kerningX)
{
    KerningInfo* kerningInfo = state->kerningInfo;
    for(uint i = 0; i != 122; ++i)
    {
        if(kerningInfo[i].firstId == (uint)previous)
        {
            if(kerningInfo[i].secondId == current)
            {
                *kerningX = (float)kerningInfo[i].offsetX;
            }
        }
    }
    int a  = 0;
}
void getStringWidth(void* renderer, WCHAR* string, uint length, float* stringWidth)
{
    State* state = (State*)renderer;
    float width = {};
    for(uint i = 0; i != length; ++i)
    {
        GlyphInfo* glyphInfo = state->glyphInfo;
        uint glyphIndex = (uint)string[i];
        glyphIndex -= 31;
        
        width += (float)glyphInfo[glyphIndex].offsetX;
        float advanceX = (float)glyphInfo[glyphIndex].advanceX;
        width += advanceX;

        if(i != 0)
        {
            float kerningX = {};
            getKerning(state, string[i - 1], string[i], &kerningX);
            width += kerningX;
        }
    }
    *stringWidth = width;
}
void addString(void* renderer, WCHAR* string, uint length, float x, float y, uint entityTag)
{
    State* state = (State*)renderer;
    float advanceX = {};
    float cursor = x;
    for(uint i = 0; i != length; ++i)
    {
        Window glyph = {cursor, y, 0.0f, 0.0f};
        Window glyphTexture = {};
        getGlyph(state, string[i], &advanceX, &glyph, &glyphTexture);
        if(i != 0)
        {
            float kerningX = {};
            getKerning(state, string[i - 1], string[i], &kerningX);
            glyph.x += kerningX;
            cursor += kerningX;
        }
        cursor += advanceX;

        uint textureTag = {};
        createTextureEntity(renderer, &textureTag); 
        setTextureEntityCoordinates(renderer, textureTag, &glyphTexture);
        addTextureQuad(renderer, &glyph, entityTag, textureTag);
    }
}
void blit(void* renderer, Window* window, int deltaX, int deltaY)
{
    State* state = (State*)renderer;
	VkResult result = {};

    VkCommandBuffer commandBuffer = state->device.commandBuffer.transfer;
    vkResetCommandBuffer(commandBuffer, 0);

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    void* blitPass = state->blitPass;
    blitPass::execute(blitPass, commandBuffer, window, deltaX, deltaY);

    result = vkEndCommandBuffer(commandBuffer);
    assert(result == VK_SUCCESS);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = 0;
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = 0;

    VkQueue queue = state->device.queue.transfer;
    result = vkQueueSubmit(queue, 1, &submitInfo, 0);
    assert(result == VK_SUCCESS);

    vkQueueWaitIdle(queue);
}
void getEntityOnScreen(void* renderer, uint x, uint y, uint* entityIndex)
{
    State* state = (State*)renderer;
    VkDevice logicalDevice = state->device.logicalDevice;

    VkImageSubresource subresource = {};
    subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.mipLevel = 0;
    subresource.arrayLayer = 0;

    VkSubresourceLayout layout = {};
    vkGetImageSubresourceLayout(logicalDevice, state->entityIndexImage.image, &subresource, &layout);

    uint rowPitch = (uint)layout.rowPitch / 4;
    //uint width = state->swapchain.image.width;
    uint* ntityIndex = &state->entityIndexSurface[(y * rowPitch) + x];
    *entityIndex = state->entityIndexSurface[(y * rowPitch) + x];
}
void drawChild(void* renderer, Window* uncoveredArea, ChildList* childList)
{
    State* state = (State*)renderer;

    uint childCount = childList->count;
    for(uint i = 0; i != childCount; ++i)
    {
        uint windowTag = childList->indices[i];
        Window window = {};
        getWindowDimension(renderer, windowTag, &window);

        Window intersection = {};
        calculateIntersection(uncoveredArea, &window, &intersection);
        if(intersection.width == 0.0f || intersection.height == 0.0f)
        {
            continue;
        }
        HWND* windowHandles = state->windowHandles;
        SendMessage(windowHandles[windowTag], CWM_DRAW, (WPARAM)&intersection, 0);
    }
}
void drawOccludedEntites(void* renderer, uint windowTag)
{
    State* state = (State*)renderer;
    ChildList* childList = &state->childLists[windowTag];
    Window* window = &state->windowDimensions[windowTag];
    drawChild(renderer, window, childList);
}
void pruneChildList(void* renderer, Window* newWindow, ChildList* childList)
{
    State* state = (State*)renderer;
    uint childCount = childList->count;
    for(uint i = 0; i != childCount; ++i)
    {
        uint windowTag = childList->indices[i];
        Window window = {};
        getWindowDimension(renderer, windowTag, &window);

        Window intersection = {};
        calculateIntersection(newWindow, &window, &intersection);
        if(intersection.width == 0.0f || intersection.height == 0.0f)
        {
            --childCount;
            uint* indices = childList->indices;
            indices[i] = indices[childCount];
            --i;
        }
    }
    childList->count = childCount;
}
void markEntityForReuse(void* renderer, uint entityTag)
{
    State* state = (State*)renderer;
    uint reuseEntityCount = state->reuseEntityCount; 
    uint freeEntityCount = state->freeEntityCount; 
    uint* freeEntities = state->freeEntities;
    freeEntities[freeEntityCount + reuseEntityCount] = entityTag;
    ++state->reuseEntityCount;
}
void moveWindow(void* renderer, uint windowTag, float deltaX, float deltaY)
{
    State* state = (State*)renderer;
    Window oldWindow = state->windowDimensions[windowTag];
    Window newWindow = oldWindow;
    newWindow.x += deltaX;
    newWindow.y += deltaY;

    Window uncoveredAreaX = {};
    Window coveredAreaX = {};
    if(deltaX < 0.0f)
    {
        uncoveredAreaX = {newWindow.x + newWindow.width, oldWindow.y, -deltaX, oldWindow.height};
        coveredAreaX = {newWindow.x, oldWindow.y, -deltaX, oldWindow.height};
    }
    else
    {
        uncoveredAreaX = {oldWindow.x, oldWindow.y, deltaX, oldWindow.height};
        coveredAreaX = {oldWindow.x + oldWindow.width, oldWindow.y, deltaX, oldWindow.height};
    }
    Window uncoveredAreaY = {};
    Window coveredAreaY = {};
    if(deltaY < 0.0f)
    {
        uncoveredAreaY = {oldWindow.x, newWindow.y + oldWindow.height, oldWindow.width, -deltaY};
        coveredAreaY = {oldWindow.x , newWindow.y, oldWindow.width, -deltaY};
    }
    else
    {
        uncoveredAreaY = {oldWindow.x, oldWindow.y, oldWindow.width, deltaY};
        coveredAreaY = {oldWindow.x , oldWindow.y + oldWindow.height, oldWindow.width, deltaY};
    }
    state->windowDimensions[windowTag] = newWindow;

    Window clip = {0, 0, 1280, 720};
    Window intersection = {};
    calculateIntersection(&oldWindow, &clip, &intersection);

    uint clippedX = {};
    if(oldWindow.x < clip.x || newWindow.x < clip.x)
    {
        uncoveredAreaY.x = intersection.x;
        uncoveredAreaY.width = intersection.width;
        
        coveredAreaY.x = intersection.x;
        coveredAreaY.width = intersection.width;
        if(deltaX > 0.0f)
        {
            uncoveredAreaX.x = clip.x;
            HWND windowHandle = state->windowHandles[windowTag];
            SendMessage(windowHandle, CWM_DRAW, (WPARAM)&uncoveredAreaX, 0);
            clippedX = 1;
        }
        else
        {
            intersection.x -= deltaX;
            intersection.width += deltaX;
            coveredAreaX = {};
        }
        if(clip.x < newWindow.x)
        {
            clippedX = 0;
        }
    }
        
    if(clip.x + clip.width < oldWindow.x + oldWindow.width || clip.x + clip.width < newWindow.x + newWindow.width)
    {
        uncoveredAreaY.width = intersection.width;
        coveredAreaY.width = intersection.width;

        if(deltaX < 0.0f)
        {
            uncoveredAreaX.x = intersection.x + intersection.width - uncoveredAreaX.width;
            HWND windowHandle = state->windowHandles[windowTag];
            SendMessage(windowHandle, CWM_DRAW, (WPARAM)&uncoveredAreaX, 0);
            clippedX = 1;
        }
        else
        {
            intersection.width -= deltaX;
            coveredAreaX = {};
        }
        if(newWindow.x + newWindow.width < clip.x + clip.width)
        {
            clippedX = 0;
        }
    }
    uint clippedY = {};
    if(oldWindow.y < clip.y || newWindow.y < clip.y)
    {
        uncoveredAreaX.y = intersection.y;
        coveredAreaX.y = intersection.y;

        uncoveredAreaX.height = intersection.height;
        coveredAreaX.height = intersection.height;
        if(deltaY > 0.0f)
        {
            uncoveredAreaY.y = clip.y;
            HWND windowHandle = state->windowHandles[windowTag];
            SendMessage(windowHandle, CWM_DRAW, (WPARAM)&uncoveredAreaY, 0);
            clippedY = 1;
        }
        else
        {
            intersection.y -= deltaY;
            intersection.height += deltaY;
            coveredAreaY = {};
        }
        if(clip.y < newWindow.y)
        {
            clippedY = 0;
        }
    }
        
    if(clip.y + clip.height < oldWindow.y + oldWindow.height || clip.y + clip.height < newWindow.y + newWindow.height)
    {
        uncoveredAreaX.height = intersection.height;
        coveredAreaX.height = intersection.height;
        if(deltaY < 0.0f)
        {
            uncoveredAreaY.y = intersection.y + intersection.height - uncoveredAreaY.height;
            HWND windowHandle = state->windowHandles[windowTag];
            SendMessage(windowHandle, CWM_DRAW, (WPARAM)&uncoveredAreaY, 0);
            clippedY = 1;
        }
        else
        {
            intersection.height -= deltaY;
            coveredAreaY = {};
        }
        if(newWindow.y + newWindow.height < clip.y + clip.height)
        {
            clippedY = 0;
        }
    }
    if(intersection.width == 0 || intersection.height == 0)
    {
        return;
    }
    ChildList* childList = &state->childLists[windowTag];

    if(clippedX == 0)
    {
        drawChild(renderer, &uncoveredAreaX, childList);
    }
    if(clippedY == 0)
    {
        drawChild(renderer, &uncoveredAreaY, childList);
    }
    pruneChildList(renderer, &intersection, childList);
    getChildUnderArea(renderer, &coveredAreaX, childList);
    getChildUnderArea(renderer, &coveredAreaY, childList);

    blit(renderer, &intersection, (int)deltaX, (int)deltaY);
    intersection.x += deltaX;
    intersection.y += deltaY;
    setWindowClipPlanes(renderer, windowTag, &intersection);

    RelativePosition* relativePosition = &state->relativePositions[windowTag];
    relativePosition->x += deltaX;
    relativePosition->y += deltaY;

    uint entityTag = {};
    createEntity(renderer, &entityTag);
    setEntityWindow(renderer, entityTag, windowTag);
    setEntityDepth(renderer, entityTag, 0.10f);
    setEntityColor(renderer, entityTag, 4);
    addQuad(renderer, &intersection, entityTag);

    markEntityForReuse(renderer, entityTag);
}
void getRenderer(HWND windowHandle, void** renderer)
{
    HWND parent = GetAncestor(windowHandle, GA_PARENT);
    *renderer = GetProp(parent, L"renderer");
}
void scroll(void* renderer, uint windowTag, Window* window, float deltaX, float deltaY)
{
    Window uncoveredAreaX = {};
    if(deltaX < 0.0f)
    {
        window->x -= deltaX;
        window->width += deltaX;
        uncoveredAreaX = {window->x + window->width + deltaX, window->y, -deltaX, window->height};
    }
    else
    {
        window->width -= deltaX;
        uncoveredAreaX = {window->x, window->y, deltaX, window->height};
    }
    if(deltaX)
    {
        sendMessage(renderer, windowTag, CWM_SCROLLUPDATE, &uncoveredAreaX, 0);
    }
    Window uncoveredAreaY = {};
    if(deltaY < 0.0f)
    {
        window->y -= deltaY;
        window->height += deltaY;
        uncoveredAreaY = {window->x, window->y + window->height + deltaY, window->width, -deltaY};
    }
    else
    {
        window->height -= deltaY;
        uncoveredAreaY = {window->x, window->y, window->width, deltaY};
    }
    if(deltaY)
    {
        sendMessage(renderer, windowTag, CWM_SCROLLUPDATE, &uncoveredAreaY, 0);
    }
    blit(renderer, window, (int)deltaX, (int)deltaY);
}

END_SCOPE
