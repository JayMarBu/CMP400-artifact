#pragma once
#include "engine/system/Device.h"
#include "engine/graphics/Buffer.h"

namespace JEngine
{
	class Model
	{
		// Members ********************************************************************************
	public:

		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 colour;
			glm::vec3 normals;
			glm::vec2 texcoords;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const 
			{
				return	position	== other.position &&
						colour		== other.colour &&
						normals		== other.normals &&
						texcoords	== other.texcoords;
			};


		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void LoadModel(const std::string& filepath);
		};

	private:
		JEngine::Device& m_device;

		std::unique_ptr<Buffer> m_vertexBuffer;
		uint32_t m_vertexCount;

		bool m_hasIndexBuffer = false;
		std::unique_ptr<Buffer> m_indexBuffer;
		uint32_t m_indexCount;

		// Methods ********************************************************************************
	public:

		Model(JEngine::Device& device, const Builder& builder);
		~Model();

		REMOVE_COPY_CONSTRUCTOR(Model);

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		static std::unique_ptr<Model> CreateModelFromFile(Device& device, const std::string& filepath);

	private:

		void CreateVertexBuffers(const std::vector<Vertex> & vertices);
		void CreateIndexBuffers(const std::vector<uint32_t> & indices);
	};
}