#include "pch.h"
#include "engine/graphics/SimpleRenderSystem.h"

namespace JEngine
{
	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 colour;
	};


	SimpleRenderSystem::SimpleRenderSystem(Device& _device, VkRenderPass _renderPass)
		: m_device(_device)
	{
		CreatePipelineLayout();
		CreatePipeline(_renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
	}

	void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects)
	{
		m_pipeline->Bind(commandBuffer);

		for (auto& obj : gameObjects)
		{
			SimplePushConstantData pushData{};

			pushData.offset = obj.transform.translation;
			pushData.colour = obj.colour;
			pushData.transform = obj.transform;

			vkCmdPushConstants(
				commandBuffer,
				m_pipelineLayout,
				VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT,
				0,
				sizeof(SimplePushConstantData),
				&pushData
			);

			obj.model->Bind(commandBuffer);
			obj.model->Draw(commandBuffer);
		}
	}

	void SimpleRenderSystem::CreatePipelineLayout()
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


	void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
	{
		assert(m_pipelineLayout != nullptr, "cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		JEngine::Pipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = m_pipelineLayout;

		m_pipeline = std::make_unique<Pipeline>(
			m_device,
			"shaders/simple_shader/simple_shader.vert.spv",
			"shaders/simple_shader/simple_shader.frag.spv",
			pipelineConfig
			);
	}

}