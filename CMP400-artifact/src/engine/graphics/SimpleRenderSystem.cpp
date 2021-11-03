#include "pch.h"
#include "engine/graphics/SimpleRenderSystem.h"

#include "engine/Camera.h"

namespace JEngine
{
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
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

	void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const JEngine::Camera& camera, const float& dt)
	{
		m_pipeline->Bind(commandBuffer);

		auto projectionView = camera.GetProjection() * camera.GetView();

		for (auto& obj : gameObjects)
		{
			obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + (1.f*dt), glm::two_pi<float>());
			obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + (0.1f*dt), glm::two_pi<float>());

			SimplePushConstantData pushData{};

			auto modelMat = obj.transform.mat4();
			pushData.transform = projectionView * modelMat;
			pushData.normalMatrix = obj.transform.normalMatrix();

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
		assert(m_pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

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