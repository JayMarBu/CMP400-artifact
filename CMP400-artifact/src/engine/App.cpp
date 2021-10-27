#include "pch.h"
#include "App.h"

JEngine::App::App()
{
	LoadModels();
	CreatePipelineLayout();
	RecreateSwapChain();
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

void JEngine::App::LoadModels()
{
	std::vector<Model::Vertex> vertices
	{
		{{0.0f, -0.5f}, {1,0,0}},
		{{0.5f, 0.5f}, {0,1,0}},
		{{-0.5f, 0.5f}, {0,0,1}}
	};

	m_model = std::make_unique<Model>(m_device, vertices);
}

void JEngine::App::CreatePipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineCreateInfo.setLayoutCount = 0;
	pipelineCreateInfo.pSetLayouts = nullptr;
	pipelineCreateInfo.pushConstantRangeCount = 1;
	pipelineCreateInfo.pPushConstantRanges = &pushConstantRange;

	SAFE_RUN_VULKAN_FUNC(vkCreatePipelineLayout(m_device, &pipelineCreateInfo, nullptr, &m_pipelineLayout), "failed to create pipelineLayout");
}

void JEngine::App::CreatePipeline()
{
	assert(m_swapChain != nullptr, "cannot create pipeline before swapchain!");
	assert(m_pipelineLayout != nullptr, "cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipelineConfig{};
	JEngine::Pipeline::DefaultPipelineConfigInfo(pipelineConfig);

	pipelineConfig.renderPass = m_swapChain->renderPass();
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
	m_commandBuffers.resize(m_swapChain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_device.commandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	SAFE_RUN_VULKAN_FUNC(vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()), "failed to allocate command buffers");
}

void JEngine::App::FreeCommandBuffers()
{
	vkFreeCommandBuffers(m_device, m_device.commandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
	m_commandBuffers.clear();
}

void JEngine::App::RecreateSwapChain()
{
	auto extent = m_window.getExtent();

	while (extent.width == 0 || extent.height == 0)
	{
		extent = m_window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_device);

	if (m_swapChain == nullptr)
		m_swapChain = std::make_unique<JEngine::SwapChain>(m_device, extent);
	else
	{
		m_swapChain = std::make_unique<JEngine::SwapChain>(m_device, extent, std::move(m_swapChain));
		if (m_swapChain->imageCount() != m_commandBuffers.size())
		{
			FreeCommandBuffers();
			CreateCommandBuffers();
		}
	}

	// TODO: if render pass is compatible, don't do this
	CreatePipeline();
}

void JEngine::App::RecordCommandBuffer(uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	SAFE_RUN_VULKAN_FUNC(vkBeginCommandBuffer(m_commandBuffers[imageIndex], &beginInfo), "failed to being recording command buffer");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_swapChain->renderPass();
	renderPassInfo.framebuffer = m_swapChain->frameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = m_swapChain->extent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.01f,0.01f,0.01f,1.0f };
	clearValues[1].depthStencil = { 1, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(m_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(m_swapChain->extent().width);
	viewport.height = static_cast<float>(m_swapChain->extent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, m_swapChain->extent() };
	vkCmdSetViewport(m_commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(m_commandBuffers[imageIndex], 0, 1, &scissor);

	m_pipeline->Bind(m_commandBuffers[imageIndex]);
	m_model->Bind(m_commandBuffers[imageIndex]);

	for (int j = 0; j < 4; j++)
	{
		SimplePushConstantData pushData{};

		pushData.offset = {0, -0.4f + j * 0.25f };
		pushData.colour = {0, 0, 0.2f + 0.2f*j};

		vkCmdPushConstants(
			m_commandBuffers[imageIndex],
			m_pipelineLayout,
			VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT,
			0,
			sizeof(SimplePushConstantData),
			&pushData
		);

		m_model->Draw(m_commandBuffers[imageIndex]);
	}

	vkCmdEndRenderPass(m_commandBuffers[imageIndex]);
	SAFE_RUN_VULKAN_FUNC(vkEndCommandBuffer(m_commandBuffers[imageIndex]), "failed to end command buffer");
}

void JEngine::App::DrawFrame()
{
	uint32_t imageIndex;
	auto result = m_swapChain->AcquireNextImage(&imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire next swapchain image");
	}

	RecordCommandBuffer(imageIndex);
	result = m_swapChain->SubmitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.WasWindowResized())
	{
		m_window.ResetWindowResizedFlag();
		RecreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit command buffer");
	}

}
