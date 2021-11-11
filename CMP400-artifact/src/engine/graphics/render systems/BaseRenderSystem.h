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

		// Methods ********************************************************************************
	public:
		BaseRenderSystem(Device& _device, VkRenderPass _renderPass, VkDescriptorSetLayout descriptorSet);
		~BaseRenderSystem();

		REMOVE_COPY_CONSTRUCTOR(BaseRenderSystem);

	protected:
		void CreatePipelineLayout(VkDescriptorSetLayout descriptorSet);
		void CreatePipeline(VkRenderPass _renderPass);
	};
}