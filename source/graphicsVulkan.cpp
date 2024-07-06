#include "include.hpp"
#include "graphicsVulkan.hpp"

START_SCOPE(vulkan)

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
	NOT_USING(messageSeverity);
	NOT_USING(messageType);
	NOT_USING(callbackData);
	NOT_USING(userData);
    int a = 0;
    a = 0;
    a = 0;
    a = 0;
    a = 0;
	return 0;
}
void createInstance(VkInstance* instance)
{
	VkResult result = {};

	uint requestedExtensionCount = 3;
	const char* requiredInstanceExtensions[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	};

	uint extensionCount = {};
	result = vkEnumerateInstanceExtensionProperties(0, &extensionCount, 0);

	VkExtensionProperties* extensionProperties = {};
	allocateMemory(sizeof(VkExtensionProperties) * extensionCount, (void**)&extensionProperties);
	result = vkEnumerateInstanceExtensionProperties(0, &extensionCount, extensionProperties);

	for (uint i = 0; i != requestedExtensionCount; ++i)
	{
		uint found = {};
		for (uint j = 0; j != extensionCount; ++j)
		{
			char* availableInstanceExtension = extensionProperties[j].extensionName;
			if (strcmp((char*)requiredInstanceExtensions[i], availableInstanceExtension) == 0)
			{
				found = 1;
				break;
			}
		}
		assert(found == 1);
	}
	freeMemory(extensionProperties);

	uint requestedLayerCount = 1;
	const char* requiredinstanceLayers[] =
	{
		"VK_LAYER_KHRONOS_validation",
	};

	uint availableLayerCount = {};
	VkLayerProperties* layerProperties = {};
	result = vkEnumerateInstanceLayerProperties(&availableLayerCount, 0);

	allocateMemory(sizeof(VkLayerProperties) * availableLayerCount, (void**)&layerProperties);
	result = vkEnumerateInstanceLayerProperties(&availableLayerCount, layerProperties);

	for (uint i = 0; i != requestedLayerCount; ++i)
	{
		uint found = {};
		for (uint j = 0; j != availableLayerCount; ++j)
		{
			char* availableInstanceLayer = layerProperties[j].layerName;
			if (strcmp((char*)requiredinstanceLayers[i], availableInstanceLayer) == 0)
			{
				found = 1;
				break;
			}
		}
		assert(found == 1);
	}
	freeMemory(layerProperties);

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = 0;
	applicationInfo.pApplicationName = "test";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.pEngineName = "testEngine";
	applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.apiVersion = VK_MAKE_VERSION(1, 3, 0);

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = 0;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = 1;
	instanceCreateInfo.ppEnabledLayerNames = requiredinstanceLayers;
	instanceCreateInfo.enabledExtensionCount = 3;
	instanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtensions;

	result = vkCreateInstance(&instanceCreateInfo, 0, instance);
	assert(result == VK_SUCCESS);
}
void createDebugMessageCallback(VkInstance instance, VkDebugUtilsMessengerEXT* debugMessenger)
{
	VkResult result = {};

	PFN_vkCreateDebugUtilsMessengerEXT pfnCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	PFN_vkDestroyDebugUtilsMessengerEXT pfnDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	NOT_USING(pfnDestroyDebugUtilsMessengerEXT);
	int flags =
	{
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
	};
	VkDebugUtilsMessageSeverityFlagBitsEXT severityFlags = (VkDebugUtilsMessageSeverityFlagBitsEXT)flags;

	flags =
	{
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
	};
	VkDebugUtilsMessageTypeFlagBitsEXT messageTypeFlags = (VkDebugUtilsMessageTypeFlagBitsEXT)flags;

	VkDebugUtilsMessengerCreateInfoEXT debugUtilityMessengerCreateInfo = {};
	debugUtilityMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilityMessengerCreateInfo.pNext = 0;
	debugUtilityMessengerCreateInfo.flags = 0;
	debugUtilityMessengerCreateInfo.messageSeverity = severityFlags;
	debugUtilityMessengerCreateInfo.messageType = messageTypeFlags;
	debugUtilityMessengerCreateInfo.pfnUserCallback = vulkanDebugCallback;
	debugUtilityMessengerCreateInfo.pUserData = 0;

	result = pfnCreateDebugUtilsMessengerEXT(instance, &debugUtilityMessengerCreateInfo, 0, debugMessenger);
	assert(result == VK_SUCCESS);
}
void createSurface(VkInstance instance, HWND windowHandle, HINSTANCE windowInstance, VkSurfaceKHR* surface)
{
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = windowInstance;
	surfaceCreateInfo.hwnd = windowHandle;

	VkResult result = {};
	result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, 0, surface);
	assert(result == VK_SUCCESS);
}
void checkExtensionSupport(VkPhysicalDevice physicalDevice, uint* extensionSupport)
{
	VkResult result = {};

	uint extentionCount = {};
	result = vkEnumerateDeviceExtensionProperties(physicalDevice, 0, &extentionCount, 0);
	assert(result == VK_SUCCESS);

	VkExtensionProperties* extensionProperties = {};
	allocateMemory(sizeof(VkExtensionProperties) * extentionCount, (void**)&extensionProperties);
	result = vkEnumerateDeviceExtensionProperties(physicalDevice, 0, &extentionCount, extensionProperties);

	uint requiredExtensionCount = 1;
	const char* requiredExtensions[] =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	for (uint i = 0; i != requiredExtensionCount; ++i)
	{
		for (uint j = 0; j != extentionCount; ++j)
		{
			char* availableExtension = extensionProperties[j].extensionName;
			if (strcmp((char*)requiredExtensions[i], availableExtension) == 0)
			{
				*extensionSupport = 1;
				break;
			}
		}
	}
	freeMemory(extensionProperties);
}
void checkPresentQueueSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR vulkanSurface, uint queueFamilyCount, uint* presentQueueFamily)
{
	VkResult result = {};
	for (uint i = 0; i != queueFamilyCount; ++i)
	{
		VkBool32 presentSupport = {};
		result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, vulkanSurface, &presentSupport);
		assert(result == VK_SUCCESS);

		if (presentSupport)
		{
			*presentQueueFamily = i;
		}
	}
}
void getQueueFamily(VkQueueFamilyProperties* queueFamilyProperties, uint queueFamilyCount, uint flag, uint* queueFamily)
{
	for (uint i = 0; i != queueFamilyCount; ++i)
	{
		if (queueFamilyProperties[i].queueFlags & flag)
		{
			*queueFamily = i;
		}
	}
}
void checkSurfaceSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint* surfaceFormatCount, uint* surfacePresentModeCount)
{
	VkResult result = {};
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, surfaceFormatCount, 0);

	result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, surfacePresentModeCount, 0);
	assert(result == VK_SUCCESS);
}
void createPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice* physicalDevice, uint* queueFamilies)
{
	VkResult result = {};

	uint physicalDeviceCount = {};
	result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, 0);
	assert(result == VK_SUCCESS);

	VkPhysicalDevice* physicalDevices = {};
	allocateMemory(sizeof(VkPhysicalDevice) * physicalDeviceCount, (void**)&physicalDevices);
	result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);
	assert(result == VK_SUCCESS);

	uint graphicsQueueFamily = UINT_MAX;
	uint computeQueueFamily = UINT_MAX;
	uint transferQueueFamily = UINT_MAX;
	uint presentQueueFamily = UINT_MAX;

	for (uint i = 0; i != physicalDeviceCount; ++i)
	{
		uint extensionSupport = {};
		checkExtensionSupport(physicalDevices[i], &extensionSupport);
		if (!extensionSupport)
		{
			continue;
		}

		uint queueFamilyCount = {};
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, 0);

		VkQueueFamilyProperties* queueFamilyProperties = {};
		allocateMemory(sizeof(VkQueueFamilyProperties) * queueFamilyCount, (void**)&queueFamilyProperties);

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilyProperties);
		
		getQueueFamily(queueFamilyProperties, queueFamilyCount, VK_QUEUE_GRAPHICS_BIT, &graphicsQueueFamily);
		if (graphicsQueueFamily == UINT_MAX)
		{
			continue;
		}
		
		getQueueFamily(queueFamilyProperties, queueFamilyCount, VK_QUEUE_COMPUTE_BIT, &computeQueueFamily);
		if (computeQueueFamily == UINT_MAX)
		{
			continue;
		}
		
		getQueueFamily(queueFamilyProperties, queueFamilyCount, VK_QUEUE_TRANSFER_BIT, &transferQueueFamily);
		if (transferQueueFamily == UINT_MAX)
		{
			continue;
		}

		checkPresentQueueSupport(physicalDevices[i], surface, queueFamilyCount, &presentQueueFamily);
		if (presentQueueFamily == UINT_MAX)
		{
			continue;
		}

		uint surfaceFormatCount = {};
		uint surfacePresentModeCount = {};
		checkSurfaceSupport(physicalDevices[i], surface, &surfaceFormatCount, &surfacePresentModeCount);
		if (!surfaceFormatCount || !surfacePresentModeCount)
		{
			continue;
		}
		*physicalDevice = physicalDevices[i];
		queueFamilies[0] = graphicsQueueFamily;
		queueFamilies[1] = presentQueueFamily;
		queueFamilies[2] = computeQueueFamily;
		queueFamilies[3] = transferQueueFamily;
	}
}
void createLogicalDevice(VkPhysicalDevice physicalDevice, uint* queueFamilyIndices, VkDevice* logicalDevice)
{
	uint selectedQueueFamilies[4] = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX };
	uint selectedQueueFamilyCount = {};
	for (uint i = 0; i != 4; ++i)
	{
		if (queueFamilyIndices[i] != UINT_MAX)
		{
			uint reject = 0;
			for (uint j = 0; j != i; ++j)
			{
				if (queueFamilyIndices[i] == selectedQueueFamilies[j])
				{
					reject = 1;
				}
			}
			if (!reject)
			{
				selectedQueueFamilies[selectedQueueFamilyCount] = queueFamilyIndices[i];
				++selectedQueueFamilyCount;
			}
		}
	}
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo* deviceQueueCreateInfo = {};
	allocateMemory(sizeof(VkDeviceQueueCreateInfo) * selectedQueueFamilyCount, (void**)&deviceQueueCreateInfo);
	for (uint i = 0; i != selectedQueueFamilyCount; ++i)
	{
		deviceQueueCreateInfo[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo[i].pNext = 0;
		deviceQueueCreateInfo[i].flags = 0;
		deviceQueueCreateInfo[i].queueFamilyIndex = selectedQueueFamilies[i];
		deviceQueueCreateInfo[i].queueCount = 1;
		deviceQueueCreateInfo[i].pQueuePriorities = &queuePriority;
 	}

	uint requiredExtensionCount = 2;
	const char* requiredExtensions[] =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
	};

	uint requiredLayeredCount = 1;
	const char* requiredLayers[] =
	{
		"VK_LAYER_KHRONOS_validation",
	};
    VkPhysicalDeviceFeatures features = {};
    features.fragmentStoresAndAtomics = VK_TRUE;
    features.shaderClipDistance = VK_TRUE;

	VkDeviceCreateInfo logicalDeviceCreateInfo = {};
	logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceCreateInfo.pNext = 0;//&physicalDeviceFeatures;
	logicalDeviceCreateInfo.flags = 0;
	logicalDeviceCreateInfo.queueCreateInfoCount = selectedQueueFamilyCount;
	logicalDeviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfo;
	logicalDeviceCreateInfo.enabledLayerCount = requiredLayeredCount;
	logicalDeviceCreateInfo.ppEnabledLayerNames = requiredLayers;
	logicalDeviceCreateInfo.enabledExtensionCount = requiredExtensionCount;
	logicalDeviceCreateInfo.ppEnabledExtensionNames = requiredExtensions;
	logicalDeviceCreateInfo.pEnabledFeatures = &features;

	VkResult result = {};
	result = vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, 0, logicalDevice);
	assert(result == VK_SUCCESS);
}
void getSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint format, uint colorSpace, VkSurfaceFormatKHR* surfaceFormat)
{
	VkResult result = {};
	uint surfaceFormatCount = {};
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, 0);

	VkSurfaceFormatKHR* surfaceFormats = {};
	allocateMemory(sizeof(VkSurfaceFormatKHR) * surfaceFormatCount, (void**)&surfaceFormats);
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats);

	for (uint i = 0; i != surfaceFormatCount; ++i)
	{
		if (surfaceFormats[i].format == (VkFormat)format && surfaceFormats[i].colorSpace == (VkColorSpaceKHR)colorSpace)
		{
			*surfaceFormat = surfaceFormats[i];
			break;
		}
	}
}
void createCommandPool(VkDevice logicalDevice, uint queueFamily, VkCommandPool* commandPool)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.pNext = 0;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = queueFamily;

	VkResult result = {};
	result = vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, 0, commandPool);
	assert(result == VK_SUCCESS);
}
void createCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool, uint level, VkCommandBuffer* commandBuffer)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = 0;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = (VkCommandBufferLevel)level;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkResult result = {};
	result = vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandBuffer);
}
void createBufferObject(VkDevice logicalDevice, uint64 size, uint usageFlags, uint ownership, VkBuffer* bufferObject)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = 0;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.sharingMode = (VkSharingMode)ownership;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = 0;

	VkResult result = {};
	result = vkCreateBuffer(logicalDevice, &bufferCreateInfo, 0, bufferObject);
	assert(result == VK_SUCCESS);
}
void createBufferMemory(VkDevice logicalDevice, uint64 size, uint requiredMemoryIndex, VkDeviceMemory* bufferMemory)
{
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.pNext = 0;//&flagsInfo;
	memoryAllocateInfo.allocationSize = size;
	memoryAllocateInfo.memoryTypeIndex = requiredMemoryIndex;

	VkResult result = {};
	result = vkAllocateMemory(logicalDevice, &memoryAllocateInfo, 0, bufferMemory);
	assert(result == VK_SUCCESS);
}
void copyBuffer(Device* device, Buffer* source, uint64 size, Buffer* destination)
{
	VkResult result = {};

	VkQueue queue = device->queue.transfer;
	VkCommandBuffer commandBuffer = device->commandBuffer.transfer;
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.pNext = 0;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	commandBufferBeginInfo.pInheritanceInfo = 0;

	result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
	assert(result == VK_SUCCESS);
    
    VkBuffer srcBuffer = source->buffer;
    uint64 srcSize = source->size;

	VkBufferMemoryBarrier bufferBarrier = {};
	bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	bufferBarrier.pNext = 0;
	bufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
	bufferBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	bufferBarrier.srcQueueFamilyIndex = 0;
	bufferBarrier.dstQueueFamilyIndex = 0;
	bufferBarrier.buffer = srcBuffer;
	bufferBarrier.offset = 0;
	bufferBarrier.size = srcSize;

    uint sourceStage = VK_PIPELINE_STAGE_HOST_BIT;
    uint destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, 0, 1, &bufferBarrier, 0, 0);

	VkBufferCopy bufferCopy = {};
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	bufferCopy.size = size;

    VkBuffer dstBuffer = destination->buffer;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);

	vkEndCommandBuffer(commandBuffer);

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

	result = vkQueueSubmit(queue, 1, &submitInfo, 0);
	assert(result == VK_SUCCESS);

	vkQueueWaitIdle(queue);
}
void createImageMemoryBarrier(VkImage image, uint oldLayout, uint newLayout, VkImageMemoryBarrier* imageMemoryBarrier)
{
    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    imageMemoryBarrier->sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier->pNext = 0;
    imageMemoryBarrier->srcAccessMask = 0;
    imageMemoryBarrier->dstAccessMask = 0;
    imageMemoryBarrier->oldLayout = (VkImageLayout)oldLayout;
    imageMemoryBarrier->newLayout = (VkImageLayout)newLayout;
    imageMemoryBarrier->srcQueueFamilyIndex = 0;
    imageMemoryBarrier->dstQueueFamilyIndex = 0;
    imageMemoryBarrier->image = image;
    imageMemoryBarrier->subresourceRange = subresourceRange;
}
void transitionImage(Device* device, Image* image, uint newLayout)
{
	VkResult result = {};

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    VkCommandBuffer commandBuffer = device->commandBuffer.transfer;
	vkResetCommandBuffer(commandBuffer, 0);

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = image->aspectMask;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    uint oldLayout = image->layout;
    VkImage srcImage = image->image;

    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = 0;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = 0;
    imageMemoryBarrier.oldLayout = (VkImageLayout)oldLayout;
    imageMemoryBarrier.newLayout = (VkImageLayout)newLayout;
    imageMemoryBarrier.srcQueueFamilyIndex = 0;
    imageMemoryBarrier.dstQueueFamilyIndex = 0;
    imageMemoryBarrier.image = srcImage;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    uint srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    uint dstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, 0, 0, 0, 1, &imageMemoryBarrier);

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

    VkQueue queue = device->queue.transfer;
    result = vkQueueSubmit(queue, 1, &submitInfo, 0);
    assert(result == VK_SUCCESS);
    vkQueueWaitIdle(queue);
}
void fillDeviceImage(Device* device, Image* image, VkBuffer buffer)
{
	VkResult result = {};

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    VkCommandBuffer commandBuffer = device->commandBuffer.transfer;
	vkResetCommandBuffer(commandBuffer, 0);

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    VkImageSubresourceLayers subresourceLayers = {};
    subresourceLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceLayers.mipLevel = 0;
    subresourceLayers.baseArrayLayer = 0;
    subresourceLayers.layerCount = 1;

    uint width = image->width;
    uint height = image->height;

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource = subresourceLayers;
    region.imageOffset = {};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

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

    VkQueue queue = device->queue.transfer;
    result = vkQueueSubmit(queue, 1, &submitInfo, 0);
    assert(result == VK_SUCCESS);
    vkQueueWaitIdle(queue);
}
void createDescriptorLayoutBinding(uint binding, uint type, uint count, uint stage, VkDescriptorSetLayoutBinding* descriptorSetBinding)
{
	descriptorSetBinding->binding = binding;
	descriptorSetBinding->descriptorType = (VkDescriptorType)type;
	descriptorSetBinding->descriptorCount = count;
	descriptorSetBinding->stageFlags = stage;
	descriptorSetBinding->pImmutableSamplers = 0;
}
void createDescriptorSetLayout(VkDevice logicalDevice, VkDescriptorSetLayoutBinding* descriptorSetBinding, uint bindingCount, VkDescriptorSetLayout* descriptorSetLayout)
{
	VkResult result = {};

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = 0;
	descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetBinding;

	result = vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, 0, descriptorSetLayout);
	assert(result == VK_SUCCESS);
}
void createDescriptorPool(VkDevice logicalDevice, uint* descriptorTypes, uint* descriptorCounts, uint poolSizeCount, VkDescriptorPool* descriptorPool)
{
	VkDescriptorPoolSize* poolSizes = {};
	allocateMemory(sizeof(VkDescriptorPoolSize) * poolSizeCount, (void**)&poolSizes);
	for (uint i = 0; i != poolSizeCount; ++i)
	{
		poolSizes[i].descriptorCount = descriptorCounts[i];
		poolSizes[i].type = (VkDescriptorType)descriptorTypes[i];
	}

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.pNext = 0;
	descriptorPoolCreateInfo.flags = 0;
	descriptorPoolCreateInfo.maxSets = 1;
	descriptorPoolCreateInfo.poolSizeCount = poolSizeCount;
	descriptorPoolCreateInfo.pPoolSizes = poolSizes;

	VkResult result = {};
	result = vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, 0, descriptorPool);
	assert(result == VK_SUCCESS);
}
void createDescriptorSet(VkDevice logicalDevice, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet* descriptorSet)
{
	VkResult result = {};

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = 0;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

	result = vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, descriptorSet);
	assert(result == VK_SUCCESS);
}
void createDescriptorImageInfo(VkSampler sampler, VkImageView imageView, uint layout , VkDescriptorImageInfo* descriptorImageInfo)
{
    descriptorImageInfo->sampler = sampler;
    descriptorImageInfo->imageView = imageView;
    descriptorImageInfo->imageLayout = (VkImageLayout)layout;
}
void updateDescriptorSet(VkDevice logicalDevice, uint bindPoint, VkDescriptorImageInfo* descriptorImageInfo, VkDescriptorSet descriptorSet, uint type)
{
    VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = 0;
	writeDescriptorSet.dstSet = descriptorSet;
	writeDescriptorSet.dstBinding = bindPoint;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = (VkDescriptorType)type;
	writeDescriptorSet.pImageInfo = descriptorImageInfo;
	writeDescriptorSet.pBufferInfo = 0;
	writeDescriptorSet.pTexelBufferView = 0;

    vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, 0);
}
void updateDescriptorSet(VkDevice logicalDevice, uint bindPoint, VkBuffer buffer, VkDescriptorSet descriptorSet, uint type)
{
	VkDescriptorBufferInfo descriptorBufferInfo = {};
	descriptorBufferInfo.buffer = buffer;
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = 0;
	writeDescriptorSet.dstSet = descriptorSet;
	writeDescriptorSet.dstBinding = bindPoint;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = (VkDescriptorType)type;
	writeDescriptorSet.pImageInfo = 0;
	writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
	writeDescriptorSet.pTexelBufferView = 0;

	vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, 0);
}
void copyDescriptorSet(VkDevice logicalDevice, uint bindPoint, VkDescriptorSet srcDescriptorSet, VkDescriptorSet dstDescriptorSet)
{
    VkCopyDescriptorSet copyDescriptorSet = {};
    copyDescriptorSet.sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
    copyDescriptorSet.pNext = 0;
    copyDescriptorSet.srcSet = srcDescriptorSet;
    copyDescriptorSet.srcBinding = bindPoint;
    copyDescriptorSet.srcArrayElement = 0;
    copyDescriptorSet.dstSet = dstDescriptorSet;
    copyDescriptorSet.dstBinding = bindPoint;
    copyDescriptorSet.dstArrayElement = 0;
    copyDescriptorSet.descriptorCount = 1;

	vkUpdateDescriptorSets(logicalDevice, 0, 0, 1, &copyDescriptorSet);
}
void createPushConstant(uint stages, uint size, VkPushConstantRange* pushConstantRange)
{
    pushConstantRange->stageFlags = stages;
    pushConstantRange->offset = 0;
    pushConstantRange->size = size;
}
void createShaderModule(VkDevice logicalDevice, WCHAR* filePath, VkShaderModule* shaderModule)
{
	void* shaderCode;
	uint64 shaderCodeSize;
	loadFile(filePath, &shaderCode, &shaderCodeSize);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pNext = 0;
	shaderModuleCreateInfo.flags = 0;
	shaderModuleCreateInfo.codeSize = shaderCodeSize;
	shaderModuleCreateInfo.pCode = (uint32*)shaderCode;

	VkResult result = {};
	result = vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, 0, shaderModule);
	assert(result == VK_SUCCESS);
}
void createShaderStage(VkDevice logicalDevice, WCHAR* filePath, uint stage, VkPipelineShaderStageCreateInfo* shaderStageCreateInfo)
{
    VkShaderModule shaderModule = {};
    createShaderModule(logicalDevice, filePath, &shaderModule);

	shaderStageCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo->pNext = 0;
	shaderStageCreateInfo->flags = 0;
	shaderStageCreateInfo->stage = (VkShaderStageFlagBits)stage;
	shaderStageCreateInfo->module = shaderModule;
	shaderStageCreateInfo->pName = "main";
	shaderStageCreateInfo->pSpecializationInfo = 0;
}
void createSemaphore(VkDevice logicalDevice, VkSemaphore* semaphore)

{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = 0;
	semaphoreCreateInfo.flags = 0;

	VkResult result = {};
	result = vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, 0, semaphore);
}
void createFence(VkDevice logicalDevice, VkFence* fence)
{
	VkResult result = {};

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = 0;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	result = vkCreateFence(logicalDevice, &fenceCreateInfo, 0, fence);
	assert(result == VK_SUCCESS);
}
void findRequiredMemory(uint memoryTypeBits, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, VkMemoryPropertyFlags requiredMemoryPropertyFlags, uint* requiredMemoryIndex)
{
	uint memoryTypeCount = deviceMemoryProperties.memoryTypeCount;
	for (uint i = 0; i != memoryTypeCount; ++i)
	{
		if ((memoryTypeBits & i))
		{
			if (deviceMemoryProperties.memoryTypes[i].propertyFlags & requiredMemoryPropertyFlags)
			{
				*requiredMemoryIndex = i;
			}
		}
		memoryTypeBits = memoryTypeBits >> 1;
	}
}
void allocateBuffer(Device* device, Buffer* buffer)
{
    uint usage = buffer->usage;
    uint ownership = buffer->ownership;
    uint64 size = buffer->size;

    VkDevice logicalDevice = device->logicalDevice;

    VkBuffer bufferObject = {};
    createBufferObject(logicalDevice, size, usage, ownership, &bufferObject);
    buffer->buffer = bufferObject;

    VkMemoryPropertyFlags memoryPropertyFlags = buffer->memoryType;

    VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements(logicalDevice, bufferObject, &memoryRequirements);

    VkPhysicalDeviceMemoryProperties deviceMemoryProperties = device->deviceMemoryProperties;
	uint requiredMemoryIndex = {};
	findRequiredMemory(memoryRequirements.memoryTypeBits, deviceMemoryProperties, memoryPropertyFlags, &requiredMemoryIndex);

    VkDeviceMemory memory = {};
	createBufferMemory(logicalDevice, size, requiredMemoryIndex, &memory);
    buffer->memory = memory;

	VkResult result = {};
	result = vkBindBufferMemory(logicalDevice, bufferObject, memory, 0);
	assert(result == VK_SUCCESS);
}
void mapImage(Device* device, Image* image, void** memory)
{
    VkDevice logicalDevice = device->logicalDevice;
    vkMapMemory(logicalDevice, image->memory, 0, image->size, 0, (void**)memory);
}
void mapBuffer(Device* device, Buffer* buffer, void** memory)
{
    VkDevice logicalDevice = device->logicalDevice;
    vkMapMemory(logicalDevice, buffer->memory, 0, buffer->size, 0, (void**)memory);
}
void fillDeviceBuffer(Device* device, void* data, uint64 size, Buffer* buffer)
{
    Buffer stagingBuffer = {};
    stagingBuffer.size = size;
    stagingBuffer.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    stagingBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &stagingBuffer);

    VkDeviceMemory memory = stagingBuffer.memory;
    VkDevice logicalDevice = device->logicalDevice;

    void* mappedMemory = {};
	VkResult result = {};
	result = vkMapMemory(logicalDevice, memory, 0, size, 0, &mappedMemory);
	assert(result == VK_SUCCESS);

	memcpy(mappedMemory, data, size);

    VkMappedMemoryRange memoryRange = {};
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.pNext = 0;
    memoryRange.memory = memory;
    memoryRange.offset = 0;
    memoryRange.size = size;

    vkFlushMappedMemoryRanges(logicalDevice, 1, &memoryRange);
	vkUnmapMemory(logicalDevice, memory);

    copyBuffer(device, &stagingBuffer, size, buffer);
}
void createImageView(VkDevice logicalDevice, Image* image, VkImageView* imageView)
{
    VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = 0;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = image->image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = (VkFormat)image->format;
	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.subresourceRange.aspectMask = image->aspectMask;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	VkResult result = {};
    result = vkCreateImageView(logicalDevice, &imageViewCreateInfo, 0, imageView);
    assert(result == VK_SUCCESS);
}
void createImage(Device* device, Image* image)
{
	VkResult result = {};
    VkDevice logicalDevice = device->logicalDevice;

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = 0;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = (VkFormat)image->format;
    imageCreateInfo.extent.width = image->width;
    imageCreateInfo.extent.height = image->height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = (VkImageTiling)image->tiling;
    imageCreateInfo.usage = image->usage;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.pQueueFamilyIndices = 0;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkImage vkImage = {};
    result = vkCreateImage(logicalDevice, &imageCreateInfo, 0, &vkImage);
    assert(result == VK_SUCCESS);
    image->image = vkImage;
    image->layout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkMemoryRequirements imageMemoryRequirements = {};
    vkGetImageMemoryRequirements(logicalDevice, vkImage, &imageMemoryRequirements);

    VkPhysicalDeviceMemoryProperties deviceMemoryProperties = device->deviceMemoryProperties;

    uint requiredMemoryIndex = {};
    uint memoryTypeBits = imageMemoryRequirements.memoryTypeBits;
    uint memoryFlags = image->memoryType;
    findRequiredMemory(memoryTypeBits, deviceMemoryProperties, memoryFlags, &requiredMemoryIndex);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.pNext = 0;
	memoryAllocateInfo.allocationSize = imageMemoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = requiredMemoryIndex;

    VkDeviceMemory imageMemory = {};
    vkAllocateMemory(logicalDevice, &memoryAllocateInfo, 0, &imageMemory);
    image->memory = imageMemory;
    image->size = (uint)imageMemoryRequirements.size;

    vkBindImageMemory(logicalDevice, vkImage, imageMemory, 0);

    VkImageView imageView = {};
    createImageView(logicalDevice, image, &imageView);
    image->view = imageView;
}
void createSwapchain(Device* device, VkSurfaceKHR surface, Swapchain* swapchain)
{
    VkPhysicalDevice physicalDevice = device->physicalDevice;
    VkDevice logicalDevice = device->logicalDevice;

    uint format = VK_FORMAT_R8G8B8A8_UNORM;
    uint colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    VkSurfaceFormatKHR surfaceFormat = {};
    getSurfaceFormat(physicalDevice, surface, format, colorSpace, &surfaceFormat);
    swapchain->surfaceFormat = surfaceFormat;

	VkResult result = {};
	VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
    swapchain->surfaceCapabilities = surfaceCapabilities;

    VkExtent2D swapchainExtent = surfaceCapabilities.minImageExtent;
    swapchain->image.width = swapchainExtent.width;
    swapchain->image.height = swapchainExtent.height;

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    uint swapchainImageCount = 1;
    swapchain->imageCount = swapchainImageCount;

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = 0;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = swapchainImageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = swapchainExtent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = 0;
	swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = swapchainPresentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = 0;

	VkSwapchainKHR vkSwapchain = {};
	result = vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, 0, &vkSwapchain);
	assert(result == VK_SUCCESS);
    swapchain->swapchain = vkSwapchain;

    VkImage image = {};
	result = vkGetSwapchainImagesKHR(logicalDevice, vkSwapchain, &swapchainImageCount, &image);
    swapchain->image.image = image;
    swapchain->image.format = format;
    swapchain->image.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    swapchain->image.tiling = VK_IMAGE_TILING_OPTIMAL;

    VkImageView imageView = {};
    createImageView(logicalDevice, &swapchain->image, &imageView);
    swapchain->image.view = imageView;
}
void createVertexAttribute(uint location, uint binding, uint format, uint offset, VkVertexInputAttributeDescription* vertexAttributeDescription)
{
    vertexAttributeDescription->location = location;
    vertexAttributeDescription->binding = binding;
    vertexAttributeDescription->format = (VkFormat)format;
    vertexAttributeDescription->offset = offset;
}
void createVertexBinding(uint binding, uint stride, uint inputRate, VkVertexInputBindingDescription* vertexBindingDescription)
{
	vertexBindingDescription->binding = binding;
	vertexBindingDescription->stride = stride;
	vertexBindingDescription->inputRate = (VkVertexInputRate)inputRate;
}
void createVertexInputState(uint bindingCount, VkVertexInputBindingDescription* vertexBindingDescription, uint attributeCount, VkVertexInputAttributeDescription* vertexAttributeDescription, VkPipelineVertexInputStateCreateInfo* vertexInputState)
{
	vertexInputState->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputState->pNext = 0;
	vertexInputState->flags = 0;
	vertexInputState->vertexBindingDescriptionCount = bindingCount;
	vertexInputState->pVertexBindingDescriptions = vertexBindingDescription;
	vertexInputState->vertexAttributeDescriptionCount = attributeCount;
	vertexInputState->pVertexAttributeDescriptions = vertexAttributeDescription;
}
void createInputAssemblyState(uint topology, VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState)
{
	inputAssemblyState->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState->pNext = 0;
	inputAssemblyState->flags = 0;
	inputAssemblyState->topology = (VkPrimitiveTopology)topology;
	inputAssemblyState->primitiveRestartEnable = 0;
}
void createViewportState(VkPipelineViewportStateCreateInfo* viewportState)
{
	viewportState->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState->pNext = 0;
	viewportState->flags = 0;
	viewportState->viewportCount = 1;
	viewportState->pViewports = 0;
	viewportState->scissorCount = 1;
	viewportState->pScissors = 0;
}
void createRasterizationState(VkPipelineRasterizationStateCreateInfo* rasterizationState)
{
	rasterizationState->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState->pNext = 0;
	rasterizationState->flags = 0;
	rasterizationState->depthClampEnable = 0;
	rasterizationState->rasterizerDiscardEnable = 0;
	rasterizationState->polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationState->cullMode = VK_CULL_MODE_NONE;
	rasterizationState->frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizationState->depthBiasEnable = 0;
	rasterizationState->depthBiasConstantFactor = 0;
	rasterizationState->depthBiasClamp = 0;
	rasterizationState->depthBiasSlopeFactor = 0;
	rasterizationState->lineWidth = 1.0f;
}
void createMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState)
{
	multisampleState->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState->pNext = 0;
	multisampleState->flags = 0;
	multisampleState->rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleState->sampleShadingEnable = 0;
	multisampleState->minSampleShading = 1.0f;
	multisampleState->pSampleMask = 0;
	multisampleState->alphaToCoverageEnable = 0;
	multisampleState->alphaToOneEnable = 0;
}
void createDepthStencilState(uint compareOp, VkPipelineDepthStencilStateCreateInfo* depthStencilState)
{
    VkStencilOpState stencilState = {};
    stencilState.failOp = VK_STENCIL_OP_KEEP;
    stencilState.passOp = VK_STENCIL_OP_KEEP;
    stencilState.depthFailOp = VK_STENCIL_OP_KEEP;
    stencilState.compareOp = VK_COMPARE_OP_NEVER;
    stencilState.compareMask = 0;
    stencilState.writeMask = 0;
    stencilState.reference = 0;

    depthStencilState->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState->pNext = 0;
    depthStencilState->flags = 0;
    depthStencilState->depthTestEnable = VK_TRUE;
    depthStencilState->depthWriteEnable = VK_TRUE;
    depthStencilState->depthCompareOp = (VkCompareOp)compareOp;
    depthStencilState->depthBoundsTestEnable = VK_FALSE;
    depthStencilState->stencilTestEnable = VK_FALSE;
    depthStencilState->front = stencilState;
    depthStencilState->back = stencilState;
    depthStencilState->minDepthBounds = 0.0f;
    depthStencilState->maxDepthBounds = 0.0f;
}
void createAttachmentBlendState(bool enable, VkPipelineColorBlendAttachmentState* colorBlendAttachmentState)
{
	uint colorWriteMask =
	{
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT
	};

	colorBlendAttachmentState->blendEnable = enable;
	colorBlendAttachmentState->srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachmentState->dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachmentState->colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState->dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachmentState->alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState->colorWriteMask = colorWriteMask;
}
void createBlendState(uint attachmentCount, VkPipelineColorBlendAttachmentState* attachmentColorBlendState, VkPipelineColorBlendStateCreateInfo* colorBlendState)
{
	colorBlendState->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendState->pNext = 0;
	colorBlendState->flags = 0;
	colorBlendState->logicOpEnable = VK_FALSE;
	colorBlendState->logicOp = VK_LOGIC_OP_CLEAR;
	colorBlendState->attachmentCount = attachmentCount;
	colorBlendState->pAttachments = attachmentColorBlendState;
	colorBlendState->blendConstants[0] = 0.0f;
	colorBlendState->blendConstants[1] = 0.0f;
	colorBlendState->blendConstants[2] = 0.0f;
	colorBlendState->blendConstants[3] = 0.0f;
}
void createDynamicState(uint stateCount, uint* dynamicStates, VkPipelineDynamicStateCreateInfo* dynamicState)
{
	dynamicState->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState->pNext = 0;
	dynamicState->flags = 0;
	dynamicState->dynamicStateCount = stateCount;
	dynamicState->pDynamicStates = (VkDynamicState*)dynamicStates;
}
void createSampler(Device* device, VkSampler* fontSampler)
{
	VkResult result = {};
    VkDevice logicalDevice = device->logicalDevice;

    VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.pNext = 0;
    samplerCreateInfo.flags = 0;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.addressModeV= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 1.0f;
    samplerCreateInfo.compareEnable = VK_FALSE;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerCreateInfo.minLod = 0.0f;
    samplerCreateInfo.maxLod = 0.0f;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

    result = vkCreateSampler(logicalDevice, &samplerCreateInfo, 0, fontSampler);
}

END_SCOPE
