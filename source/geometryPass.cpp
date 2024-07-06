#include "include.hpp"
#include "geometryPass.hpp"

START_SCOPE(geometryPass)

void createRenderPass(State* state, vulkan::Device* device, uint colorFormat, uint depthFormat)
{
    VkAttachmentReference colorAttachment = {};
	colorAttachment.attachment = 1;
	colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachment = {};
	depthAttachment.attachment = 0;
	depthAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription depthSubpassDescription = {};
	depthSubpassDescription.flags = 0;
	depthSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	depthSubpassDescription.inputAttachmentCount = 0;
	depthSubpassDescription.pInputAttachments = 0;
	depthSubpassDescription.colorAttachmentCount = 0;
	depthSubpassDescription.pColorAttachments = 0;
	depthSubpassDescription.pResolveAttachments = 0;
	depthSubpassDescription.pDepthStencilAttachment = &depthAttachment;
	depthSubpassDescription.preserveAttachmentCount = 0;
	depthSubpassDescription.pPreserveAttachments = 0;

    VkSubpassDescription colorSubpassDescription = {};
	colorSubpassDescription.flags = 0;
	colorSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	colorSubpassDescription.inputAttachmentCount = 0;
	colorSubpassDescription.pInputAttachments = 0;
	colorSubpassDescription.colorAttachmentCount = 1;
	colorSubpassDescription.pColorAttachments = &colorAttachment;
	colorSubpassDescription.pResolveAttachments = 0;
	colorSubpassDescription.pDepthStencilAttachment = &depthAttachment;
	colorSubpassDescription.preserveAttachmentCount = 0;
	colorSubpassDescription.pPreserveAttachments = 0;
    
    VkSubpassDescription subpassDescriptions[] = 
    {
        depthSubpassDescription,
        colorSubpassDescription,
    };

    VkAttachmentDescription depthDescription = {};
	depthDescription.flags = 0;
	depthDescription.format = (VkFormat)depthFormat;
	depthDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	depthDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorDescription = {};
	colorDescription.flags = 0;
	colorDescription.format = (VkFormat)colorFormat;
	colorDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	colorDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	colorDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription attachmentDescriptions[] = 
    {
        depthDescription,
        colorDescription,
    };

    VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = 0;
	subpassDependency.dstSubpass = 1;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	subpassDependency.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	subpassDependency.dependencyFlags = 0;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.pNext = 0;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = 2;
	renderPassCreateInfo.pAttachments = attachmentDescriptions;
	renderPassCreateInfo.subpassCount = 2;
	renderPassCreateInfo.pSubpasses = subpassDescriptions;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

    VkDevice logicalDevice = device->logicalDevice;
	VkResult result = {};

    VkRenderPass renderPass = {};
	result = vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, 0, &renderPass);
	assert(result == VK_SUCCESS);

    state->renderPass = renderPass;
}
void createFrameBuffer(State* state, vulkan::Device* device, vulkan::Image* compositeImage, vulkan::Image* depthImage)
{
    VkImageView imageViews[] = 
    {
        depthImage->view,
        compositeImage->view,
    };

    VkRenderPass renderPass = state->renderPass;

    uint width = compositeImage->width;
    uint height = compositeImage->height;

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
void create(vulkan::Device* device, vulkan::Image* compositeImage, vulkan::Image* depthImage, VkDescriptorSet descriptorSet, void** geometryPass)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);

    createRenderPass(state, device, compositeImage->format, depthImage->format);
    createFrameBuffer(state, device, compositeImage, depthImage);

    void* quadPass = {};
    quadPass::create(device, state->renderPass, descriptorSet, &quadPass);
    state->quadPass = quadPass;

    void* textureQuadPass = {};
    textureQuadPass::create(device, state->renderPass, descriptorSet, &textureQuadPass);
    state->textureQuadPass = textureQuadPass;

    *geometryPass = state;
}
void execute(void* geometryPass, uint width, uint height, VkCommandBuffer commandBuffer)
{
    State* state = (State*)geometryPass;

    VkRenderPass renderPass = state->renderPass;
    VkFramebuffer frameBuffer = state->frameBuffer;

    VkClearValue clearValue[2] = {};
    clearValue[0].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = 0;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = frameBuffer;
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = {width, height};
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    void* quadPass = state->quadPass;
    quadPass::executeDepthPrePass(quadPass, commandBuffer);

    void* textureQuadPass = state->textureQuadPass;
    textureQuadPass::executeDepthPrePass(textureQuadPass, commandBuffer);

    vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);

    quadPass::execute(quadPass, commandBuffer);
    textureQuadPass::execute(textureQuadPass, commandBuffer);

    vkCmdEndRenderPass(commandBuffer);
}
void addQuad(void* geometryPass, float x, float y, float width, float height, uint entityTag)
{
    State* state = (State*)geometryPass;

    quadPass::Quad quad = {};
    quad.vertices[0] = {{x, y}};
    quad.vertices[1] = {{x + width, y}};
    quad.vertices[2] = {{x + width, y + height}};
    quad.vertices[3] = {{x, y + height}};

    void* quadPass = state->quadPass;
    quadPass::addQuad(quadPass, &quad, entityTag);
}
void addTextureQuad(void* geometryPass, float x, float y, float width, float height, uint entityTag, uint textureTag)
{
    State* state = (State*)geometryPass;

    textureQuadPass::Quad quad = {};
    quad.vertices[0] = {{x, y}};
    quad.vertices[1] = {{x + width, y}};
    quad.vertices[2] = {{x + width, y + height}};
    quad.vertices[3] = {{x, y + height}};

    void* textureQuadPass = state->textureQuadPass;
    textureQuadPass::addQuad(textureQuadPass, &quad, entityTag, textureTag);
}
void addTextureCoordinate(void* geometryPass, float x, float y, float width, float height, uint textureEntityTag)
{
    State* state = (State*)geometryPass;

    textureQuadPass::Quad quad = {};
    quad.vertices[0] = {{x, y}};
    quad.vertices[1] = {{x + width, y}};
    quad.vertices[2] = {{x + width, y + height}};
    quad.vertices[3] = {{x, y + height}};

    void* textureQuadPass = state->textureQuadPass;
    textureQuadPass::addTextureCoordinate(textureQuadPass, &quad, textureEntityTag);
}
END_SCOPE
