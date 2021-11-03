#pragma once

#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Model.h"
#include "engine/GameObject.h"
#include "engine/graphics/FrameInfo.h"

namespace JEngine
{
	class Camera;

	class SimpleRenderSystem
	{
		// Members ********************************************************************************
	public:

	private:
		JEngine::Device& m_device;

		std::unique_ptr<JEngine::Pipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;

		// Methods ********************************************************************************
	public:
		SimpleRenderSystem(Device& _device, VkRenderPass _renderPass, VkDescriptorSetLayout descriptorSet);
		~SimpleRenderSystem();

		REMOVE_COPY_CONSTRUCTOR(SimpleRenderSystem);

		void RenderGameObjects(FrameInfo& fInfo, std::vector<GameObject>& gameObjects);

	private:
		void CreatePipelineLayout(VkDescriptorSetLayout descriptorSet);
		void CreatePipeline(VkRenderPass _renderPass);
	};

}