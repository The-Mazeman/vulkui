#include "include.hpp"
#include "compositePass.hpp"

START_SCOPE(compositePass)

void createBuffer(State* state, vulkan::Device* device)
{
    uint16 indices[] = 
    {
        0, 1, 2, 2, 3, 0,
    };

    vulkan::Buffer indexBuffer = {};
    indexBuffer.size = sizeof(indices);
    indexBuffer.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    indexBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    indexBuffer.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    vulkan::allocateBuffer(device, &indexBuffer);
    vulkan::fillDeviceBuffer(device, indices, sizeof(indices), &indexBuffer);
    state->indexBuffer = indexBuffer;

    float vertices[] = 
    {
         -1.0f, -1.0f,
          1.0f, -1.0f,
          1.0f,  1.0f,
         -1.0f,  1.0f,

    };

    vulkan::Buffer vertexBuffer = {};
    vertexBuffer.size = sizeof(vertices);
    vertexBuffer.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vertexBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    vertexBuffer.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    vulkan::allocateBuffer(device, &vertexBuffer);
    vulkan::fillDeviceBuffer(device, vertices, sizeof(vertices), &vertexBuffer);
    state->vertexBuffer = vertexBuffer;
}
void createDescriptor(State* state, vulkan::Device* device, vulkan::Image* compositeImage)
{
    VkDevice logicalDevice = device->logicalDevice;

    VkDescriptorSetLayoutBinding compositeImageBinding = {};
	vulkan::createDescriptorLayoutBinding(0, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT, &compositeImageBinding);

    VkDescriptorSetLayoutBinding layoutBindings[] =
	{
        compositeImageBinding,
    };

	VkDescriptorSetLayout descriptorSetLayout = {};
	vulkan::createDescriptorSetLayout(logicalDevice, layoutBindings, 1, &descriptorSetLayout);
    state->descriptorSetLayout = descriptorSetLayout;

    uint descriptorTypes[] =
	{
		VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
    };

	uint descriptorCounts[] =
	{
		1,
    };

    VkDescriptorPool descriptorPool = {};
	vulkan::createDescriptorPool(logicalDevice, descriptorTypes, descriptorCounts, 1, &descriptorPool);

    VkDescriptorSet descriptorSet = {};
	vulkan::createDescriptorSet(logicalDevice, descriptorPool, descriptorSetLayout, &descriptorSet);
    state->descriptorSet = descriptorSet;

    VkImageView imageView = compositeImage->view;
    uint layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkDescriptorImageInfo descriptorImageInfo = {};
    vulkan::createDescriptorImageInfo(0, imageView, layout, &descriptorImageInfo);

    uint type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    vulkan::updateDescriptorSet(logicalDevice, 0, &descriptorImageInfo, descriptorSet, type);
}
void createRenderPass(State* state, vulkan::Device* device, uint format)
{
    VkDevice logicalDevice = device->logicalDevice;

    VkAttachmentReference inputAttachment = {};
	inputAttachment.attachment = 0;
	inputAttachment.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkAttachmentReference swapchainAttachment = {};
	swapchainAttachment.attachment = 1;
	swapchainAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription = {};
	subpassDescription.flags = 0;
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.inputAttachmentCount = 1;
	subpassDescription.pInputAttachments = &inputAttachment;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &swapchainAttachment;
	subpassDescription.pResolveAttachments = 0;
	subpassDescription.pDepthStencilAttachment = 0;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = 0;

    VkAttachmentDescription attachmentDescriptions[2] = {};

	attachmentDescriptions[0].flags = 0;
	attachmentDescriptions[0].format = (VkFormat)format;
	attachmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	attachmentDescriptions[1].flags = 0;
	attachmentDescriptions[1].format = (VkFormat)format;
	attachmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	subpassDependency.dependencyFlags = 0;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.pNext = 0;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = 2;
	renderPassCreateInfo.pAttachments = attachmentDescriptions;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

	VkResult result = {};

    VkRenderPass renderPass = {};
	result = vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, 0, &renderPass);
	assert(result == VK_SUCCESS);
    state->renderPass = renderPass;
}
void createFrameBuffer(State* state, vulkan::Device* device, vulkan::Image* compositeImage, vulkan::Image* swapchainImage)
{
    VkImageView imageViews[] = 
    {
        compositeImage->view,
        swapchainImage->view,
    };

    VkRenderPass renderPass = state->renderPass;

    uint width = swapchainImage->width;
    uint height = swapchainImage->height;

    VkFramebufferCreateInfo frameBufferCreateInfo = {};
    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferCreateInfo.pNext = 0;
    frameBufferCreateInfo.flags = 0;
    frameBufferCreateInfo.renderPass = renderPass;
    frameBufferCreateInfo.attachmentCount = 2;
    frameBufferCreateInfo.pAttachments = imageViews;
    frameBufferCreateInfo.width = width;
    frameBufferCreateInfo.height = height;
    frameBufferCreateInfo.layers = 1;

    VkDevice logicalDevice = device->logicalDevice;
	VkResult result = {};

    VkFramebuffer frameBuffer = {};
    result = vkCreateFramebuffer(logicalDevice, &frameBufferCreateInfo, 0, &frameBuffer);
    state->frameBuffer = frameBuffer;
}
void createPipelineLayout(State* state, vulkan::Device* device)
{
    VkDevice logicalDevice = device->logicalDevice;

	VkDescriptorSetLayout descriptorSetLayout = state->descriptorSetLayout;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = 0;
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = 0;

	VkResult result = {};
    VkPipelineLayout pipelineLayout = {};
    result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, 0, &pipelineLayout);
    state->pipelineLayout = pipelineLayout;
}
void createPipeline(State* state, vulkan::Device* device)
{
    VkDevice logicalDevice = device->logicalDevice;

    WCHAR vertexShaderPath[] = L"C:\\Users\\administrator-1\\Desktop\\renderer\\source\\compositePass.vert.spirv";
    VkPipelineShaderStageCreateInfo vertexShaderStage = {};
    vulkan::createShaderStage(logicalDevice, vertexShaderPath, VK_SHADER_STAGE_VERTEX_BIT, &vertexShaderStage);

    WCHAR fragmentShaderPath[] = L"C:\\Users\\administrator-1\\Desktop\\renderer\\source\\compositePass.frag.spirv";
    VkPipelineShaderStageCreateInfo fragmentShaderStage = {};
    vulkan::createShaderStage(logicalDevice, fragmentShaderPath, VK_SHADER_STAGE_FRAGMENT_BIT, &fragmentShaderStage);

    VkPipelineShaderStageCreateInfo shaderStages[] =
    {
        fragmentShaderStage,
        vertexShaderStage,
    };

    VkVertexInputAttributeDescription vertexAttributeDescription = {};
    vulkan::createVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, 0, &vertexAttributeDescription);

    VkVertexInputBindingDescription vertexBindingDescription = {};
    vulkan::createVertexBinding(0, sizeof(float) * 2, VK_VERTEX_INPUT_RATE_VERTEX, &vertexBindingDescription);

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vulkan::createVertexInputState(1, &vertexBindingDescription, 1, &vertexAttributeDescription, &vertexInputState);

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    vulkan::createInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, &inputAssemblyState);

    VkPipelineViewportStateCreateInfo viewportState = {};
    vulkan::createViewportState(&viewportState);

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    vulkan::createRasterizationState(&rasterizationState);

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    vulkan::createMultisampleState(&multisampleState);

	VkPipelineColorBlendAttachmentState attachmentColorBlendState = {};
    vulkan::createAttachmentBlendState(0, &attachmentColorBlendState);
    
    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    vulkan::createBlendState(1, &attachmentColorBlendState, &colorBlendState);

	uint dynamicStates[] = 
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    vulkan::createDynamicState(2, dynamicStates, &dynamicState);

    VkRenderPass renderPass = state->renderPass;
    VkPipelineLayout pipelineLayout = state->pipelineLayout;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;;
	graphicsPipelineCreateInfo.pNext = 0;
	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = shaderStages;
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputState;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	graphicsPipelineCreateInfo.pTessellationState = 0;
	graphicsPipelineCreateInfo.pViewportState = &viewportState;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
	graphicsPipelineCreateInfo.pDepthStencilState = 0;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
	graphicsPipelineCreateInfo.layout = pipelineLayout;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = 0;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VkResult result = {};
    VkPipeline graphicsPipeline = {};
	result = vkCreateGraphicsPipelines(logicalDevice, 0, 1, &graphicsPipelineCreateInfo, 0, &graphicsPipeline);
	assert(result == VK_SUCCESS);

    state->graphicsPipeline = graphicsPipeline;
}
void create(vulkan::Device* device, vulkan::Image* swapchainImage, vulkan::Image* compositeImage, void** compositePass)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);

    createBuffer(state, device);
    createDescriptor(state, device, compositeImage);
    createRenderPass(state, device, swapchainImage->format);
    createFrameBuffer(state, device, compositeImage, swapchainImage);
    createPipelineLayout(state, device);
    createPipeline(state, device);

    *compositePass = state;

}
void execute(void* compositePass, VkCommandBuffer commandBuffer)
{
    State* state = (State*)compositePass;

    VkRenderPass renderPass = state->renderPass;
    VkFramebuffer frameBuffer = state->frameBuffer;

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = 0;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = frameBuffer;
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = {1280, 720};
    renderPassBeginInfo.clearValueCount = 0;
    renderPassBeginInfo.pClearValues = 0;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    VkPipeline pipeline = state->graphicsPipeline;
    vkCmdBindPipeline(commandBuffer, bindPoint, pipeline);

	VkDescriptorSet descriptorSet = state->descriptorSet;
    VkPipelineLayout pipelineLayout = state->pipelineLayout;
    vkCmdBindDescriptorSets(commandBuffer, bindPoint, pipelineLayout, 0, 1, &descriptorSet, 0, 0);

    VkBuffer vertexBuffer = state->vertexBuffer.buffer;
    VkDeviceSize deviceSize = 0;
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &deviceSize);

    VkBuffer indexBuffer = state->indexBuffer.buffer;
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);
}
END_SCOPE
