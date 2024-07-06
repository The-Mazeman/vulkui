#include "include.hpp"
#include "textureQuadPass.hpp"

START_SCOPE(textureQuadPass)

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

    allocateBuffer(device, &indexBuffer);
    fillDeviceBuffer(device, indices, sizeof(indices), &indexBuffer);
    state->indexBuffer = indexBuffer;

    vulkan::Buffer vertexBuffer = {};
    vertexBuffer.size = sizeof(Quad) * 128;
    vertexBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vertexBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    vertexBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &vertexBuffer);
    state->vertexBuffer = vertexBuffer;

    vec2* vertices = {};
    mapBuffer(device, &vertexBuffer, (void**)&vertices);
    state->vertices = vertices;

    vulkan::Buffer textureVertexBuffer = {};
    textureVertexBuffer.size = sizeof(Quad) * 128;
    textureVertexBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    textureVertexBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    textureVertexBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &textureVertexBuffer);
    state->textureVertexBuffer = textureVertexBuffer;

    vec2* textureVertices = {};
    mapBuffer(device, &textureVertexBuffer, (void**)&textureVertices);
    state->textureVertices = textureVertices;

    vulkan::Buffer textureIndexBuffer = {};
    textureIndexBuffer.size = sizeof(uint) * 128;
    textureIndexBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    textureIndexBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    textureIndexBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &textureIndexBuffer);
    state->textureIndexBuffer = textureIndexBuffer;

    uint* textureIndices = {};
    mapBuffer(device, &textureIndexBuffer, (void**)&textureIndices);
    state->textureIndices = textureIndices;

    vulkan::Buffer entityIndexBuffer = {};
    entityIndexBuffer.size = sizeof(uint) * 128;
    entityIndexBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    entityIndexBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    entityIndexBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &entityIndexBuffer);
    state->entityIndexBuffer = entityIndexBuffer;

    uint* entityIndices = {};
    mapBuffer(device, &entityIndexBuffer, (void**)&entityIndices);
    state->entityIndices = entityIndices;
}
void createDescriptor(State* state, vulkan::Device* device, VkDescriptorSet copyDescriptorSet)
{
    VkDevice logicalDevice = device->logicalDevice;

    uint stages = 
    {
        VK_SHADER_STAGE_VERTEX_BIT |
        VK_SHADER_STAGE_FRAGMENT_BIT
    };

    VkDescriptorSetLayoutBinding windowRegionBinding = {};
	vulkan::createDescriptorLayoutBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &windowRegionBinding);

    VkDescriptorSetLayoutBinding entityBinding = {};
	vulkan::createDescriptorLayoutBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &entityBinding);

    VkDescriptorSetLayoutBinding colorBinding = {};
	vulkan::createDescriptorLayoutBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &colorBinding);

    VkDescriptorSetLayoutBinding surfaceBinding = {};
	vulkan::createDescriptorLayoutBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, stages, &surfaceBinding);

    VkDescriptorSetLayoutBinding fontBinding = {};
	vulkan::createDescriptorLayoutBinding(4, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, stages, &fontBinding);

    VkDescriptorSetLayoutBinding samplerBinding = {};
	vulkan::createDescriptorLayoutBinding(5, VK_DESCRIPTOR_TYPE_SAMPLER, 1, stages, &samplerBinding);

    VkDescriptorSetLayoutBinding textureVertexBinding = {};
	vulkan::createDescriptorLayoutBinding(6, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &textureVertexBinding);

    VkDescriptorSetLayoutBinding textureIndexBinding = {};
	vulkan::createDescriptorLayoutBinding(7, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &textureIndexBinding);

    VkDescriptorSetLayoutBinding vertexBinding = {};
	vulkan::createDescriptorLayoutBinding(8, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &vertexBinding);

    VkDescriptorSetLayoutBinding entityIndexBinding = {};
	vulkan::createDescriptorLayoutBinding(9, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &entityIndexBinding);


    VkDescriptorSetLayoutBinding layoutBindings[] =
	{
        windowRegionBinding,
        entityBinding,
        colorBinding,
        surfaceBinding,
        fontBinding,
        samplerBinding,
        textureVertexBinding,
        textureIndexBinding,
        vertexBinding,
        entityIndexBinding,
    };

	VkDescriptorSetLayout descriptorSetLayout = {};
	vulkan::createDescriptorSetLayout(logicalDevice, layoutBindings, 10, &descriptorSetLayout);
    state->descriptorSetLayout = descriptorSetLayout;

    uint descriptorTypes[] =
	{
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
		VK_DESCRIPTOR_TYPE_SAMPLER,
    };

	uint descriptorCounts[] =
	{
		7,
		1,
		1,
		1,
    };

    VkDescriptorPool descriptorPool = {};
	vulkan::createDescriptorPool(logicalDevice, descriptorTypes, descriptorCounts, 4, &descriptorPool);

    VkDescriptorSet descriptorSet = {};
	vulkan::createDescriptorSet(logicalDevice, descriptorPool, descriptorSetLayout, &descriptorSet);
    state->descriptorSet = descriptorSet;

    vulkan::copyDescriptorSet(logicalDevice, 0, copyDescriptorSet, descriptorSet);
    vulkan::copyDescriptorSet(logicalDevice, 1, copyDescriptorSet, descriptorSet);
    vulkan::copyDescriptorSet(logicalDevice, 2, copyDescriptorSet, descriptorSet);
    vulkan::copyDescriptorSet(logicalDevice, 3, copyDescriptorSet, descriptorSet);
    vulkan::copyDescriptorSet(logicalDevice, 4, copyDescriptorSet, descriptorSet);

    VkSampler sampler = {};
    createSampler(device, &sampler);

    VkDescriptorImageInfo descriptorImageInfo = {};
    vulkan::createDescriptorImageInfo(sampler, 0, 0, &descriptorImageInfo);

    uint type = VK_DESCRIPTOR_TYPE_SAMPLER;
    vulkan::updateDescriptorSet(logicalDevice, 5, &descriptorImageInfo, descriptorSet, type);

    VkBuffer textureVertexBuffer = state->textureVertexBuffer.buffer;
    vulkan::updateDescriptorSet(logicalDevice, 6, textureVertexBuffer, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

    VkBuffer textureIndexBuffer = state->textureIndexBuffer.buffer;
    vulkan::updateDescriptorSet(logicalDevice, 7, textureIndexBuffer, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

    VkBuffer vertexBuffer = state->vertexBuffer.buffer;
    vulkan::updateDescriptorSet(logicalDevice, 8, vertexBuffer, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

    VkBuffer entityIndexBuffer = state->entityIndexBuffer.buffer;
    vulkan::updateDescriptorSet(logicalDevice, 9, entityIndexBuffer, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
}
void createPipelineLayout(State* state, vulkan::Device* device)
{
    VkDevice logicalDevice = device->logicalDevice;

    uint stages = 
    {
        VK_SHADER_STAGE_VERTEX_BIT |
        VK_SHADER_STAGE_FRAGMENT_BIT
    };

    VkPushConstantRange pushConstantRange = {};
    vulkan::createPushConstant(stages, 8, &pushConstantRange);

	VkDescriptorSetLayout descriptorSetLayout = state->descriptorSetLayout;
    
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = 0;
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

	VkResult result = {};
    VkPipelineLayout pipelineLayout = {};
    result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, 0, &pipelineLayout);
    state->pipelineLayout = pipelineLayout;
}
void createDepthPipeline(State* state, vulkan::Device* device, VkRenderPass renderPass)
{
    VkDevice logicalDevice = device->logicalDevice;

    WCHAR vertexShaderPath[] = L"C:\\Users\\administrator-1\\Desktop\\renderer\\source\\textureQuadDepthPass.vert.spirv";
    VkPipelineShaderStageCreateInfo vertexShaderStage = {};
    vulkan::createShaderStage(logicalDevice, vertexShaderPath, VK_SHADER_STAGE_VERTEX_BIT, &vertexShaderStage);

    VkPipelineShaderStageCreateInfo shaderStages[] =
    {
        vertexShaderStage,
    };

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vulkan::createVertexInputState(0, 0, 0, 0, &vertexInputState);

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    vulkan::createInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, &inputAssemblyState);

    VkPipelineViewportStateCreateInfo viewportState = {};
    vulkan::createViewportState(&viewportState);

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    vulkan::createRasterizationState(&rasterizationState);

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    vulkan::createMultisampleState(&multisampleState);

    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    vulkan::createDepthStencilState(VK_COMPARE_OP_LESS, &depthStencilState);

	VkPipelineColorBlendAttachmentState attachmentColorBlendState = {};
    vulkan::createAttachmentBlendState(1, &attachmentColorBlendState);
    
    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    vulkan::createBlendState(1, &attachmentColorBlendState, &colorBlendState);

	uint dynamicStates[] = 
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    vulkan::createDynamicState(2, dynamicStates, &dynamicState);

    VkPipelineLayout pipelineLayout = state->pipelineLayout;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.pNext = 0;
	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.stageCount = 1;
	graphicsPipelineCreateInfo.pStages = shaderStages;
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputState;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	graphicsPipelineCreateInfo.pTessellationState = 0;
	graphicsPipelineCreateInfo.pViewportState = &viewportState;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
	graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilState;
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

    state->depthPipeline = graphicsPipeline;
}
void createGraphicsPipeline(State* state, vulkan::Device* device, VkRenderPass renderPass)
{
    VkDevice logicalDevice = device->logicalDevice;

    WCHAR vertexShaderPath[] = L"C:\\Users\\administrator-1\\Desktop\\renderer\\source\\textureQuadPass.vert.spirv";
    VkPipelineShaderStageCreateInfo vertexShaderStage = {};
    vulkan::createShaderStage(logicalDevice, vertexShaderPath, VK_SHADER_STAGE_VERTEX_BIT, &vertexShaderStage);

    WCHAR fragmentShaderPath[] = L"C:\\Users\\administrator-1\\Desktop\\renderer\\source\\textureQuadPass.frag.spirv";
    VkPipelineShaderStageCreateInfo fragmentShaderStage = {};
    vulkan::createShaderStage(logicalDevice, fragmentShaderPath, VK_SHADER_STAGE_FRAGMENT_BIT, &fragmentShaderStage);

    VkPipelineShaderStageCreateInfo shaderStages[] =
    {
        fragmentShaderStage,
        vertexShaderStage,
    };

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vulkan::createVertexInputState(0, 0, 0, 0, &vertexInputState);

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    vulkan::createInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, &inputAssemblyState);

    VkPipelineViewportStateCreateInfo viewportState = {};
    vulkan::createViewportState(&viewportState);

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    vulkan::createRasterizationState(&rasterizationState);

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    vulkan::createMultisampleState(&multisampleState);

    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    vulkan::createDepthStencilState(VK_COMPARE_OP_EQUAL, &depthStencilState);

	VkPipelineColorBlendAttachmentState attachmentColorBlendState = {};
    vulkan::createAttachmentBlendState(1, &attachmentColorBlendState);
    
    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    vulkan::createBlendState(1, &attachmentColorBlendState, &colorBlendState);

	uint dynamicStates[] = 
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    vulkan::createDynamicState(2, dynamicStates, &dynamicState);

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
	graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilState;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
	graphicsPipelineCreateInfo.layout = pipelineLayout;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.subpass = 1;
	graphicsPipelineCreateInfo.basePipelineHandle = 0;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VkResult result = {};
    VkPipeline graphicsPipeline = {};
	result = vkCreateGraphicsPipelines(logicalDevice, 0, 1, &graphicsPipelineCreateInfo, 0, &graphicsPipeline);
	assert(result == VK_SUCCESS);

    state->pipeline = graphicsPipeline;
}

void create(vulkan::Device* device, VkRenderPass renderPass, VkDescriptorSet copyDescriptorSet, void** textureQuadPass)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->instanceCount = 0;

    createBuffer(state, device);
    createDescriptor(state, device, copyDescriptorSet);
    createPipelineLayout(state, device);
    createDepthPipeline(state, device, renderPass);
    createGraphicsPipeline(state, device, renderPass);

    *textureQuadPass = state;
}
void bindResources(State* state, VkCommandBuffer commandBuffer, VkPipeline pipeline)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkPipelineLayout pipelineLayout = state->pipelineLayout;
    VkDescriptorSet descriptorSet = state->descriptorSet;
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, 0);

    VkBuffer indexBuffer = state->indexBuffer.buffer;
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}
void executeDepthPrePass(void* quadPass, VkCommandBuffer commandBuffer)
{
    State* state = (State*)quadPass;

    VkPipeline pipeline = state->depthPipeline;
    bindResources(state, commandBuffer, pipeline);

    uint instanceCount = state->instanceCount;
    vkCmdDrawIndexed(commandBuffer, 6, instanceCount, 0, 0, 0);
}
void execute(void* quadPass, VkCommandBuffer commandBuffer)
{
    State* state = (State*)quadPass;

    VkPipeline pipeline = state->pipeline;
    bindResources(state, commandBuffer, pipeline);

    uint instanceCount = state->instanceCount;
    vkCmdDrawIndexed(commandBuffer, 6, instanceCount, 0, 0, 0);
    state->instanceCount = 0;
}
void addTextureCoordinate(void* quadPass, Quad* quad, uint textureEntityTag)
{
    State* state = (State*)quadPass;
    vec2* vertices = state->textureVertices;
    memcpy(&vertices[textureEntityTag * 4], quad, sizeof(Quad));
}
void addQuad(void* quadPass, Quad* quad, uint entityTag, uint textureTag)
{
    State* state = (State*)quadPass;
    uint instanceCount = state->instanceCount;
    vec2* vertices = state->vertices;
    memcpy(&vertices[instanceCount * 4], quad, sizeof(Quad));

    uint* entityIndices = state->entityIndices;
    entityIndices[instanceCount] = entityTag;

    uint* textureIndices = state->textureIndices;
    textureIndices[instanceCount] = textureTag;

    ++state->instanceCount;
}

END_SCOPE
