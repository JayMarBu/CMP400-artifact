#pragma once

#include "engine/graphics/render systems/BaseRenderSystem.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/models/Model.h"
#include "engine/GameObject.h"
#include "engine/graphics/FrameInfo.h"

namespace JEngine
{
	class SimpleRenderSystem : public BaseRenderSystem
	{
		// Members ********************************************************************************

		// Methods ********************************************************************************
	public:
		SimpleRenderSystem(
			Device& _device,
			VkRenderPass _renderPass,
			VkDescriptorSetLayout descriptorSet,
			const std::string& vert_filepath,
			const std::string& frag_filepath
		);
		~SimpleRenderSystem();

		REMOVE_COPY_CONSTRUCTOR(SimpleRenderSystem);

		void RenderGameObjects(FrameInfo& fInfo, std::vector<GameObject>& gameObjects);
	};

}