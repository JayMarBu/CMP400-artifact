#include "pch.h"
#include "engine/graphics/Model.h"
#include "tiny_obj_loader.h"

namespace std
{
	template<>
	struct hash<JEngine::Model::Vertex>
	{
		size_t operator()(JEngine::Model::Vertex const& v) const
		{
			size_t seed = 0;
			JEngine::HashCombine(seed, v.position, v.colour, v.normals, v.texcoords);
			return seed;
		}
	};
}

namespace JEngine
{

	std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
		attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, colour) });
		attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normals) });
		attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texcoords) });

		return attributeDescriptions;
	}

	Model::Model(Device& device, const Builder& builder)
		: m_device(device)
	{
		CreateVertexBuffers(builder.vertices);
		CreateIndexBuffers(builder.indices);
	}

	Model::~Model()
	{
	}

	void Model::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_vertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (m_hasIndexBuffer)
		{
			vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void Model::Draw(VkCommandBuffer commandBuffer)
	{
		if (m_hasIndexBuffer)
		{
			vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0);
		}
		else
		{
			vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
		}
	}

	std::unique_ptr<Model> Model::CreateModelFromFile(Device& device, const std::string& filepath)
	{
		Builder builder{};
		builder.LoadModel("media/" + filepath);

		std::cout << "model located at: [" << filepath << "], has been loaded successfully.\n"
			<< "\t vertex count: " << builder.vertices.size() << "\n"
			<< "\t index count: " << builder.indices.size() << "\n\n";

		return std::make_unique<Model>(device, builder);
	}

	void Model::CreateVertexBuffers(const std::vector<Vertex>& vertices)
	{
		m_vertexCount = static_cast<uint32_t>(vertices.size());

		assert(m_vertexCount >= 3 && "Vertex count must be at least 3");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_vertexCount;

		uint32_t vertexSize = sizeof(vertices[0]);

		Buffer stagingBuffer{ 
			m_device,
			vertexSize,
			m_vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)vertices.data());

		m_vertexBuffer = std::make_unique<Buffer>(
			m_device,
			vertexSize,
			m_vertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		m_device.CopyBuffer(stagingBuffer.GetBuffer(), m_vertexBuffer->GetBuffer(), bufferSize);
	}

	void Model::CreateIndexBuffers(const std::vector<uint32_t>& indices)
	{
		m_indexCount = static_cast<uint32_t>(indices.size());
		m_hasIndexBuffer = m_indexCount > 0;

		if (!m_hasIndexBuffer)
			return;

		VkDeviceSize bufferSize = sizeof(indices[0]) * m_indexCount;

		uint32_t indexSize = sizeof(indices[0]);

		Buffer stagingBuffer{
			m_device,
			indexSize,
			m_indexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)indices.data());

		m_indexBuffer = std::make_unique<Buffer>(
			m_device,
			indexSize,
			m_indexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		m_device.CopyBuffer(stagingBuffer.GetBuffer(), m_indexBuffer->GetBuffer(), bufferSize);
	}

	void Model::Builder::LoadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		vertices.clear();
		indices.clear();

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				if (index.vertex_index >= 0)
				{
					vertex.position =
					{
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

					
					vertex.colour =
					{
						attrib.colors[3 * index.vertex_index + 0],
						attrib.colors[3 * index.vertex_index + 1],
						attrib.colors[3 * index.vertex_index + 2]
					};
				}

				if (index.normal_index >= 0)
				{
					vertex.normals =
					{
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}

				if (index.texcoord_index >= 0)
				{
					vertex.texcoords =
					{
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}
				
				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

}