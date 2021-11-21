#pragma once
#include "engine/graphics/render systems/BaseRenderSystem.h"

#include "engine/graphics/FrameInfo.h"
#include "engine/graphics/Descriptors.h"
#include "engine/graphics/Buffer.h"

namespace JEngine
{
	class GizmoManager;
	class JitterAndFork;

	class StaticLineRenderSystem : public BaseRenderSystem
	{
		// Members ********************************************************************************
	public:

		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 colour;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const
			{
				return	position == other.position &&
						colour == other.colour;
			};
		};

		struct UBO
		{
			glm::mat4 projectionView;
		};

	private:
		std::unique_ptr<DescriptorPool> m_descriptorPool;
		std::unique_ptr<DescriptorSetLayout> m_descriptorSetLayout;
		std::vector<VkDescriptorSet> m_descriptorSets;

		std::vector<std::unique_ptr<Buffer>> m_UBObuffers;

		// Methods ********************************************************************************
	private:
		void CreatePipelineInfo(PipelineConfigInfo& pipelineInfo);

		void InitDescriptorSets();

	public:
		StaticLineRenderSystem(Device& device, VkRenderPass renderPass);
		~StaticLineRenderSystem();

		REMOVE_COPY_CONSTRUCTOR(StaticLineRenderSystem);

		void RenderGizmos(FrameInfo& fInfo, GizmoManager& gizmoManager, bool notBound = true);
		void RenderGizmos(FrameInfo& fInfo, JitterAndFork& gizmoManager, bool notBound = true);
	};
}