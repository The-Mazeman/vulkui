#include "include.hpp"
#include "blitPass.hpp"

START_SCOPE(blitPass)

void createImage(State* state, vulkan::Device* device)
{
    uint usage =
    {
        VK_IMAGE_USAGE_STORAGE_BIT |
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT 
    };

    uint newLayout = VK_IMAGE_LAYOUT_GENERAL;

    vulkan::Image* compositeImage = &state->compositeImage;

    vulkan::Image* colorBlitImage = &state->colorBlitImage;
    colorBlitImage->usage = usage;
    colorBlitImage->format = compositeImage->format;
    colorBlitImage->width = compositeImage->width;
    colorBlitImage->height = compositeImage->height;
    colorBlitImage->memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    colorBlitImage->tiling = VK_IMAGE_TILING_OPTIMAL;
    colorBlitImage->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    vulkan::createImage(device, colorBlitImage);
    vulkan::transitionImage(device, colorBlitImage, newLayout);

    vulkan::Image* entityIndexImage = &state->entityIndexImage;

    vulkan::Image* indexBlitImage = &state->indexBlitImage;
    indexBlitImage->usage = usage;
    indexBlitImage->format = entityIndexImage->format;
    indexBlitImage->width = entityIndexImage->width;
    indexBlitImage->height = entityIndexImage->height;
    indexBlitImage->memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    indexBlitImage->tiling = VK_IMAGE_TILING_OPTIMAL;
    indexBlitImage->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    vulkan::createImage(device, indexBlitImage);
    vulkan::transitionImage(device, indexBlitImage, newLayout);
}
void create(vulkan::Device* device, vulkan::Image* compositeImage, vulkan::Image* entityIndexImage, void** blitPass)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->compositeImage = *compositeImage;
    state->entityIndexImage = *entityIndexImage;

    createImage(state, device);
    *blitPass = state;

}
void execute(void* blitPass, VkCommandBuffer commandBuffer, Window* window, int deltaX, int deltaY)
{
    State* state = (State*)blitPass;
	VkResult result = {};

    VkImage compositeImage = state->compositeImage.image;
    VkImage colorBlitImage = state->colorBlitImage.image;

    VkImage entityIndexImage = state->entityIndexImage.image;
    VkImage indexBlitImage = state->indexBlitImage.image;
    VkImageLayout layout = VK_IMAGE_LAYOUT_GENERAL;

    int x = (int)window->x;
    int y = (int)window->y;
    uint width = (uint)window->width;
    uint height = (uint)window->height;

    START_BLOCK

    VkImageMemoryBarrier imageMemoryBarrier = {};
    vulkan::createImageMemoryBarrier(compositeImage, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL, &imageMemoryBarrier);
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, 0, 0, 0, 1, &imageMemoryBarrier);

    END_BLOCK

    START_BLOCK

    VkImageSubresourceLayers subresourceLayers = {};
    subresourceLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceLayers.mipLevel = 0;
    subresourceLayers.baseArrayLayer = 0;
    subresourceLayers.layerCount = 1;

    VkImageCopy imageCopy = {};
    imageCopy.srcSubresource = subresourceLayers;
    imageCopy.srcOffset = {x, y, 0};
    imageCopy.dstSubresource = subresourceLayers;
    imageCopy.dstOffset = {x, y, 0};
    imageCopy.extent = {width, height, 1};

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, 0, 0, 0, 0, 0);
    vkCmdCopyImage(commandBuffer, compositeImage, layout, colorBlitImage, layout, 1, &imageCopy);
    vkCmdCopyImage(commandBuffer, entityIndexImage, layout, indexBlitImage, layout, 1, &imageCopy);

    END_BLOCK

    START_BLOCK

    VkImageSubresourceLayers subresourceLayers = {};
    subresourceLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceLayers.mipLevel = 0;
    subresourceLayers.baseArrayLayer = 0;
    subresourceLayers.layerCount = 1;

    int newX = (int)window->x + deltaX;
    int newY = (int)window->y + deltaY;

    VkImageCopy imageCopy = {};
    imageCopy.srcSubresource = subresourceLayers;
    imageCopy.srcOffset = {x, y, 0};
    imageCopy.dstSubresource = subresourceLayers;
    imageCopy.dstOffset = {newX, newY, 0};
    imageCopy.extent = {width, height, 1};

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, 0, 0, 0, 0, 0);
    vkCmdCopyImage(commandBuffer, colorBlitImage, layout, compositeImage, layout, 1, &imageCopy);
    vkCmdCopyImage(commandBuffer, indexBlitImage, layout, entityIndexImage, layout, 1, &imageCopy);

    END_BLOCK

    START_BLOCK

    VkImageMemoryBarrier imageMemoryBarrier = {};
    vulkan::createImageMemoryBarrier(compositeImage, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, &imageMemoryBarrier);
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, 0, 0, 0, 1, &imageMemoryBarrier);

    END_BLOCK

}
END_SCOPE
