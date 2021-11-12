#pragma once
#include "engine/system/Device.h"
#include "engine/graphics/Pipeline.h"

namespace JEngine
{
	class BaseRenderSystem
	{
		// Members ********************************************************************************
	protected:
		JEngine::Device& m_device;

		std::unique_ptr<JEngine::Pipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;

		VkShaderStageFlags m_pushConstantShaderStages;

		// Methods ********************************************************************************
	public:
		BaseRenderSystem() = delete;
		~BaseRenderSystem();

		REMOVE_COPY_CONSTRUCTOR(BaseRenderSystem);

	protected:
		BaseRenderSystem(Device& device) : m_device(device) {}

		virtual void CreatePipelineLayout(
			VkDescriptorSetLayout descriptorSet,
			uint32_t pushConstantSize,
			VkShaderStageFlags pushConstantShaderStages = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT
		);

		virtual void CreatePipeline(
			VkRenderPass renderPass,
			ShaderPaths shaderPaths,
			PipelineConfigInfo& pipelineConfig,
			const std::vector<VkVertexInputBindingDescription>& vertexBindingDesc = Model::Vertex::getBindingDescriptions(),
			const std::vector<VkVertexInputAttributeDescription>& vertexAttribDesc = Model::Vertex::getAttributeDescriptions()
		);
	};
}