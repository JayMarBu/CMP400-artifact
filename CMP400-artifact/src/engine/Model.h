#pragma once
#include "engine/Device.h"

namespace JEngine
{
	class Model
	{
		// Members ********************************************************************************
	public:

		struct Vertex
		{
			glm::vec2 position;
			glm::vec3 colour;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

	private:
		JEngine::Device& m_device;
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		uint32_t m_vertexCount;

		// Methods ********************************************************************************
	public:

		Model(JEngine::Device& device, const std::vector<Vertex>& vertices);
		~Model();

		REMOVE_COPY_CONSTRUCTOR(Model);

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:

		void CreateVertexBuffers(const std::vector<Vertex> & vertices);
	};
}