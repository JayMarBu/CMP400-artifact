#include "pch.h"
#include "engine/graphics/render systems/BaseRenderSystem.h"

namespace JEngine
{

	BaseRenderSystem::~BaseRenderSystem()
	{
		vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
	}

	void BaseRenderSystem::CreatePipelineLayout(
		VkDescriptorSetLayout descriptorSet,
		uint32_t pushConstantSize,
		VkShaderStageFlags pushConstantShaderStages /*= VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT*/)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = pushConstantShaderStages;
		pushConstantRange.offset = 0;
		pushConstantRange.size = pushConstantSize;

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ descriptorSet };

		VkPipelineLayoutCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineCreateInfo.pSetLayouts = descriptorSetLayouts.data();
		if (pushConstantSize > 0)
		{
			pipelineCreateInfo.pushConstantRangeCount = 1;
			pipelineCreateInfo.pPushConstantRanges = &pushConstantRange;
		}
		else
		{
			pipelineCreateInfo.pushConstantRangeCount = 0;
			pipelineCreateInfo.pPushConstantRanges = nullptr;
		}

		SAFE_RUN_VULKAN_FUNC(vkCreatePipelineLayout(m_device, &pipelineCreateInfo, nullptr, &m_pipelineLayout), "failed to create pipelineLayout");
	}

	void BaseRenderSystem::CreatePipeline(
		VkRenderPass renderPass,
		ShaderPaths shaderPaths,
		PipelineConfigInfo& pipelineConfig,
		const std::vector<VkVertexInputBindingDescription>& vertexBindingDesc, /*= Model::Vertex::getBindingDescriptions()*/
		const std::vector<VkVertexInputAttributeDescription>& vertexAttribDesc /*= Model::Vertex::getAttributeDescriptions()*/
	)
	{
		assert(m_pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = m_pipelineLayout;

		m_pipeline = std::make_unique<Pipeline>(
			m_device,
			shaderPaths,
			pipelineConfig,
			vertexBindingDesc,
			vertexAttribDesc
			);
	}

}