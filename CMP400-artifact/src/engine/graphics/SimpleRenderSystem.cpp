#include "pch.h"
#include "engine/graphics/SimpleRenderSystem.h"

#include "engine/Camera.h"

namespace JEngine
{
	struct SimplePushConstantData
	{
		glm::mat4 modelMatrix{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
	};


	SimpleRenderSystem::SimpleRenderSystem(Device& _device, VkRenderPass _renderPass, VkDescriptorSetLayout descriptorSet)
		: m_device(_device)
	{
		CreatePipelineLayout(descriptorSet);
		CreatePipeline(_renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
	}

	void SimpleRenderSystem::RenderGameObjects(FrameInfo& fInfo, std::vector<GameObject>& gameObjects)
	{
		m_pipeline->Bind(fInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			fInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_pipelineLayout,
			0, 1,
			&fInfo.globalDescriptorSet,
			0, nullptr
		);

		for (auto& obj : gameObjects)
		{
			obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + (1.f*fInfo.frameTime), glm::two_pi<float>());
			obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + (0.1f* fInfo.frameTime), glm::two_pi<float>());

			SimplePushConstantData pushData{};

			pushData.modelMatrix = obj.transform;
			pushData.normalMatrix = obj.transform.normalMatrix();

			vkCmdPushConstants(
				fInfo.commandBuffer,
				m_pipelineLayout,
				VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT,
				0,
				sizeof(SimplePushConstantData),
				&pushData
			);

			obj.model->Bind(fInfo.commandBuffer);
			obj.model->Draw(fInfo.commandBuffer);
		}
	}

	void SimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout descriptorSet)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{descriptorSet};

		VkPipelineLayoutCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineCreateInfo.pSetLayouts = descriptorSetLayouts.data();
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