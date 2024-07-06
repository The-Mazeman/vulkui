#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "math.hpp"
#include "parserBmp.hpp"
#include "graphicsVulkan.hpp"
#include "geometryPass.hpp"
#include "compositePass.hpp"
#include "blitPass.hpp"
#include "parserFontMetrics.hpp"

START_SCOPE(renderer)

struct Entity
{
    uint windowTag;
    uint colorIndex;
    float depth;
    uint textureIndex;
};
struct Semaphore
{
	VkSemaphore execution;
	VkSemaphore image;
};
struct Fence
{
	VkFence pipeline;
};
struct RelativePosition
{
    float x;
    float y;
};
struct State
{
    HWND windowHandle;
    vulkan::Device device;
    vulkan::Swapchain swapchain;

    VkDescriptorSet descriptorSet;
	VkDescriptorSetLayout descriptorSetLayout;

    Fence fence;
    Semaphore semaphore;

    void* geometryPass;
    void* blitPass;
    void* compositePass;
    vulkan::Image compositeImage;
    vulkan::Image depthImage;
    vulkan::Image fontImage;

    vulkan::Image entityIndexImage;
    uint* entityIndexSurface;

    vulkan::Buffer windowClipPlanesBuffer;
    vec4* windowClipPlanes;

    vulkan::Buffer entityBuffer;
    Entity* entities;
    uint entityCount;
    uint textureEntityCount;

    vulkan::Buffer colorBuffer;
    vec4* colors;

    Window windowDimensions[32];
    RelativePosition relativePositions[32];
    ChildList childLists[32];
    HWND windowHandles[32];
    uint windowCount;
    uint superEntity;

    uint freeEntities[32];
    uint freeEntityCount;
    uint reuseEntityCount;

    GlyphInfo* glyphInfo;
    KerningInfo* kerningInfo;
};
void create(HWND windowHandle, HINSTANCE windowInstance, void** vulkan);
void draw(void* renderer);
void drawChild(void* renderer, Window* uncoveredArea, ChildList* childList);

void getEntitiesUnderArea(void* renderer, uint windowTag, Window* coveredAreaX);

void createWindowEntity(void* renderer,  uint* windowTag);
void setWindowClipPlanes(void* renderer, uint windowTag, Window* window);
void setWindowDimension(void* renderer, uint windowTag, Window* window);
void getWindowDimension(void* renderer, uint windowTag, Window* window);
void setWindowRelativePosition(void* renderer, uint windowTag, float x, float y);
void getWindowRelativePosition(void* renderer, uint windowTag, float* x, float* y);
void setWindowHandle(void* renderer, uint windowTag, HWND windowHandle);
void getWindowHandle(void* renderer, uint windowTag, HWND* windowHandle);

void setEntityWindow(void* renderer, uint entityTag, uint windowTag);

void createEntity(void* renderer, uint* entityTag);
void setEntityColor(void* renderer, uint entityTag, uint colorTag);
void setEntityDepth(void* renderer, uint entityTag, float depth);
void addQuad(void* renderer, Window* window, uint entityId);
void addBorder(void* renderer, Window* window, float thickness, uint entityId);
void addTextureQuad(void* renderer, Window* window, uint entityTag, uint textureTag);

void createTextureEntity(void* renderer, uint* textureEntityTag);
void setTextureEntityCoordinates(void* renderer, uint textureEntityTag, Window* texture);
void setEntityTexture(void* renderer, uint entityTag, uint textureTag);

void addString(void* renderer, WCHAR* string, uint length, float x, float y, uint entityTag);
void getStringWidth(void* renderer, WCHAR* string, uint length, float* stringWidth);

void scroll(void* renderer, uint windowTag, Window* window, float deltaX, float deltaY);
void blit(void* renderer, Window* window, Window* clip, int deltaX, int deltaY);
void getEntityOnScreen(void* renderer, uint x, uint y, uint* entityIndex);
void getChildUnderArea(void* renderer, Window* window, ChildList* childList);

void moveWindow(void* renderer, uint windowTag, float deltaX, float deltaY);

void getRenderer(HWND windowHandle, void** renderer);
void sendMessage(void* renderer, uint windowTag, uint message, void* firstParameter, void* secondParameter);

void drawOccludedEntites(void* renderer, uint windowTag);

END_SCOPE

