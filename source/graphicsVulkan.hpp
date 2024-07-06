#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "math.hpp"
#include "parserBmp.hpp"

START_SCOPE(vulkan)

struct Buffer
{
    VkBuffer buffer;
    VkDeviceMemory memory;
    uint64 size;
    uint usage;
    uint ownership;
    uint memoryType;
    uint ownerQueue;
};
struct Image
{
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    uint usage;
    uint format;
    uint width;
    uint height;
    uint layout;
    uint memoryType;
    uint ownerQueue;
    uint size;
    uint aspectMask;
    uint tiling;
};
struct QueueFamily
{
    uint graphics;
    uint present;
    uint compute;
    uint transfer;
};
struct CommandPool
{
    VkCommandPool graphics;
    VkCommandPool present;
    VkCommandPool compute;
    VkCommandPool transfer;
};
struct CommandBuffer
{
    VkCommandBuffer graphics;
    VkCommandBuffer present;
    VkCommandBuffer compute;
    VkCommandBuffer transfer;
};
struct Queue
{
    VkQueue graphics;
    VkQueue present;
    VkQueue compute;
    VkQueue transfer;
};
struct Device
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    VkDevice logicalDevice;

    union
    {
        uint queueFamilies[4];
        QueueFamily queueFamily;
    };
    union
    {
        VkCommandPool commandPools[4];
        CommandPool commandPool;
    };
    union
    {
        VkCommandBuffer commandBuffers[4];
        CommandBuffer commandBuffer;
    };
    union
    {
        VkQueue queues[4];
        Queue queue;
    };
};
struct Swapchain
{
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
    uint imageCount;
	VkSwapchainKHR swapchain;
    Image image;
};

void createInstance(VkInstance* instance);
void createDebugMessageCallback(VkInstance instance, VkDebugUtilsMessengerEXT* debugMessenger);
void createSurface(VkInstance instance, HWND windowHandle, HINSTANCE windowInstance, VkSurfaceKHR* surface);
void checkExtensionSupport(VkPhysicalDevice physicalDevice, uint* extensionSupport);
void checkPresentQueueSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR vulkanSurface, uint queueFamilyCount, uint* presentQueueFamily);
void getQueueFamily(VkQueueFamilyProperties* queueFamilyProperties, uint queueFamilyCount, uint flag, uint* queueFamily);
void checkSurfaceSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint* surfaceFormatCount, uint* surfacePresentModeCount);
void createPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice* physicalDevice, uint* queueFamilies);
void createLogicalDevice(VkPhysicalDevice physicalDevice, uint* queueFamilyIndices, VkDevice* logicalDevice);
void getSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, int format, int colorSpace, VkSurfaceFormatKHR* surfaceFormat);

void createCommandPool(VkDevice logicalDevice, uint queueFamily, VkCommandPool* commandPool);
void createCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool, uint level, VkCommandBuffer* commandBuffer);

void createBufferObject(VkDevice logicalDevice, uint64 size, uint usageFlags, uint ownership, VkBuffer* bufferObject);
void createBufferMemory(VkDevice logicalDevice, uint64 size, uint requiredMemoryIndex, VkDeviceMemory* bufferMemory);
void copyBuffer(Device* device, Buffer* source, uint64 size, Buffer* destination);
void fillDeviceImage(Device* device, Image* image, VkBuffer buffer);
void transitionImage(Device* device, Image* image, uint newLayout);
void createImageMemoryBarrier(VkImage image, uint oldLayout, uint newLayout, VkImageMemoryBarrier* imageMemoryBarrier);

void createDescriptorLayoutBinding(uint binding, uint type, uint count, uint stage, VkDescriptorSetLayoutBinding* descriptorSetBinding);
void createDescriptorSetLayout(VkDevice logicalDevice, VkDescriptorSetLayoutBinding* descriptorSetBinding, uint bindingCount, VkDescriptorSetLayout* descriptorSetLayout);
void createDescriptorPool(VkDevice logicalDevice, uint* descriptorTypes, uint* descriptorCounts, uint poolSizeCount, VkDescriptorPool* descriptorPool);
void createDescriptorSet(VkDevice logicalDevice, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet* descriptorSet);
void createDescriptorImageInfo(VkSampler sampler, VkImageView imageView, uint layout , VkDescriptorImageInfo* descriptorImageInfo);
void updateDescriptorSet(VkDevice logicalDevice, uint bindPoint, VkDescriptorImageInfo* descriptorImageInfo, VkDescriptorSet descriptorSet, uint type);
void updateDescriptorSet(VkDevice logicalDevice, uint bindPoint, VkBuffer buffer, VkDescriptorSet descriptorSet, uint type);
void copyDescriptorSet(VkDevice logicalDevice, uint bindPoint, VkDescriptorSet srcDescriptorSet, VkDescriptorSet dstDescriptorSet);

void createPushConstant(uint stages, uint size, VkPushConstantRange* pushConstantRange);
void createShaderModule(VkDevice logicalDevice, WCHAR* filePath, VkShaderModule* shaderModule);
void createShaderStage(VkDevice logicalDevice, WCHAR* filePath, uint stage, VkPipelineShaderStageCreateInfo* shaderStageCreateInfo);
void createSemaphore(VkDevice logicalDevice, VkSemaphore* semaphore);
void createFence(VkDevice logicalDevice, VkFence* fence);
void findRequiredMemory(uint memoryTypeBits, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, VkMemoryPropertyFlags requiredMemoryPropertyFlags, uint* requiredMemoryIndex);
void allocateBuffer(Device* device, Buffer* buffer);
void mapBuffer(Device* device, Buffer* buffer, void** memory);
void mapImage(Device* device, Image* image, void** memory);

void fillDeviceBuffer(Device* device, void* data, uint64 size, Buffer* buffer);
void createImageView(VkDevice logicalDevice, Image* image, VkImageView* imageView);
void createImage(Device* device, Image* image);
void createSwapchain(Device* device, VkSurfaceKHR surface, Swapchain* swapchain);
void createVertexAttribute(uint location, uint binding, uint format, uint offset, VkVertexInputAttributeDescription* vertexAttributeDescription);
void createVertexBinding(uint binding, uint stride, uint inputRate, VkVertexInputBindingDescription* vertexBindingDescription);
void createVertexInputState(uint bindingCount, VkVertexInputBindingDescription* vertexBindingDescription, uint attributeCount, VkVertexInputAttributeDescription* vertexAttributeDescription, VkPipelineVertexInputStateCreateInfo* vertexInputState);
void createInputAssemblyState(uint topology, VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState);
void createViewportState(VkPipelineViewportStateCreateInfo* viewportState);
void createRasterizationState(VkPipelineRasterizationStateCreateInfo* rasterizationState);
void createMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState);
void createDepthStencilState(uint compareOp, VkPipelineDepthStencilStateCreateInfo* depthStencilState);
void createAttachmentBlendState(bool enable, VkPipelineColorBlendAttachmentState* colorBlendAttachmentState);
void createBlendState(uint attachmentCount, VkPipelineColorBlendAttachmentState* attachmentColorBlendState, VkPipelineColorBlendStateCreateInfo* colorBlendState);
void createDynamicState(uint stateCount, uint* dynamicStates, VkPipelineDynamicStateCreateInfo* dynamicState);
void createSampler(Device* device, VkSampler* fontSampler);

END_SCOPE
