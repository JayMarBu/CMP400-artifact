#include "pch.h"
#include "App.h"

JEngine::App::App()
{
	CreatePipelineLayout();
	CreatePipeline();
	CreateCommandBuffers();
}

JEngine::App::~App()
{
	vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
}

void JEngine::App::run()
{
	while (!m_window.ShouldClose())
	{
		glfwPollEvents();
		DrawFrame();
	}

	vkDeviceWaitIdle(m_device);
}

void JEngine::App::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineCreateInfo.setLayoutCount = 0;
	pipelineCreateInfo.pSetLayouts = nullptr;
	pipelineCreateInfo.pushConstantRangeCount = 0;
	pipelineCreateInfo.pPushConstantRanges = nullptr;

	SAFE_RUN_VULKAN_FUNC(vkCreatePipelineLayout(m_device, &pipelineCreateInfo, nullptr, &m_pipelineLayout), "failed to create pipelineLayout");
}

void JEngine::App::CreatePipeline()
{
	PipelineConfigInfo pipelineConfig{};
	JEngine::Pipeline::DefaultPipelineConfigInfo(pipelineConfig, m_swapChain.width(), m_swapChain.height());

	pipelineConfig.renderPass = m_swapChain.renderPass();
	pipelineConfig.pipelineLayout = m_pipelineLayout;

	m_pipeline = std::make_unique<Pipeline>(
		m_device,
		"shaders/simple_shader/simple_shader.vert.spv",
		"shaders/simple_shader/simple_shader.frag.spv",
		pipelineConfig
		);
}

void JEngine::App::CreateCommandBuffers()
{
	m_commandBuffers.resize(m_swapChain.imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_device.commandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	SAFE_RUN_VULKAN_FUNC(vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()), "failed to allocate command buffers");

	for (int i = 0; i < m_commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		SAFE_RUN_VULKAN_FUNC(vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo), "failed to being recording command buffer");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_swapChain.renderPass();
		renderPassInfo.framebuffer = m_swapChain.frameBuffer(i);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = m_swapChain.extent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f,0.1f,0.1f,1.0f };
		clearValues[1].depthStencil = { 1, 0};

		renderPassInfo.clearValueCount = static_cast<uint32_t>( clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		m_pipeline->Bind(m_commandBuffers[i]);
		vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(m_commandBuffers[i]);
		SAFE_RUN_VULKAN_FUNC(vkEndCommandBuffer(m_commandBuffers[i]), "failed to end command buffer");
	}
}

void JEngine::App::DrawFrame()
{
	uint32_t imageIndex;
	auto result = m_swapChain.AcquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to aquire next swapchain image");
	}

	result = m_swapChain.SubmitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit command buffer");
	}

}
