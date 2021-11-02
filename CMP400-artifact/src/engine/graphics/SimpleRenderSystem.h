#pragma once

#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Model.h"
#include "engine/GameObject.h"

namespace JEngine
{
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
		SimpleRenderSystem(Device& _device, VkRenderPass _renderPass);
		~SimpleRenderSystem();

		REMOVE_COPY_CONSTRUCTOR(SimpleRenderSystem);

		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects);

	private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass _renderPass);
	};

}