#include "pch.h"
#include "engine/graphics/render systems/gizmo/GizmoManager.h"
#include "engine/graphics/render systems/gizmo/StaticLineRenderSystem.h"
#include "engine/graphics/models/Model.h"
#include "utils/Utils.h"
#include "engine/graphics/models/Primitives.h"

namespace JEngine
{
	// ******************************************************************************************//
	//									Line Struct												 //
	// ******************************************************************************************//
	GizmoManager::LineStatic& GizmoManager::LineStatic::SetByDirection(glm::vec3 start_point, glm::vec3 direction_vector, float length)
	{
		p1 = start_point;
		p2 = p1 + (glm::normalize(direction_vector) * length);

		return *this;
	}

	GizmoManager::LineStatic& GizmoManager::LineStatic::Transform(CTransform& transform)
	{
		glm::vec4 p1_result = transform.mat4() * glm::vec4(p1.x,p1.y,p1.z,1);
		glm::vec4 p2_result = transform.mat4() * glm::vec4(p2.x,p2.y,p2.z,1);

		p1.x = p1_result.x;
		p1.y = p1_result.y;
		p1.z = p1_result.z;

		p2.x = p2_result.x;
		p2.y = p2_result.y;
		p2.z = p2_result.z;

		return *this;
	}


	// ******************************************************************************************//
	//								   Gizmo Manager class										 //
	// ******************************************************************************************//
	GizmoManager::GizmoManager(Device& device)
		:m_device(device)
	{
		m_bufferSize = sizeof(StaticLineRenderSystem::Vertex) * MAX_LINE_COUNT;
		CreateVertexBuffer();
	}

	GizmoManager::~GizmoManager()
	{

	}

	JEngine::GizmoManager::LineStatic& GizmoManager::CreateLine(const std::string& id)
	{
		if (MAP_CONTAINS(m_lines, id))
			throw std::runtime_error("line: [" + id + "] already exists");

		m_lines.insert(std::make_pair(id, LineStatic{}));

		return m_lines[id];
	}


	GizmoManager::LineStatic& GizmoManager::GetLine(const std::string& id)
	{
		if(!MAP_CONTAINS(m_lines, id))
			throw std::runtime_error("line with id: [" + id + "] does not exist. use CreateLine("+id+") to create a new line with that id");

		return m_lines[id];
	}

	void GizmoManager::CreateVertexBuffer()
	{
		uint32_t vertexSize = sizeof(StaticLineRenderSystem::Vertex);

		m_vertexBuffer = std::make_unique<Buffer>(
			m_device,
			vertexSize,
			m_bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);
	}

	void GizmoManager::WriteToVertexBuffer()
	{
		std::vector<StaticLineRenderSystem::Vertex> vertices;

		for (LOOP_MAP_ITEMS(m_lines, line))
		{
			vertices.push_back({ line->second.p1, line->second.colour });
			vertices.push_back({ line->second.p2, line->second.colour });
		}

		m_vertexCount = static_cast<uint32_t>(vertices.size());

		uint32_t vertexSize = sizeof(StaticLineRenderSystem::Vertex);

		Buffer stagingBuffer(
			m_device,
			vertexSize,
			m_bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

		stagingBuffer.Map();

		stagingBuffer.WriteToBuffer((void*)vertices.data());
		//stagingBuffer.Flush();
		m_device.CopyBuffer(stagingBuffer.GetBuffer(), m_vertexBuffer->GetBuffer(), m_bufferSize);
	}

	void GizmoManager::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_vertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void GizmoManager::Draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
	}

	void GizmoManager::CreateCentreGrid(Dimentions dimentions)
	{
		glm::vec3 axisColour{ 0.05,0.05,0.05 };
		glm::vec3 minorColour{ 0.025,0.025,0.025 };

		CreateLine("defaultGridXaxis").SetByPosition({ 1000,0,0 }, { -1000,0,0 }).SetColour({ 0.05,0.025,0.025 });
		CreateLine("defaultGridYaxis").SetByPosition({ 0,1000,0 }, { 0,-1000,0 }).SetColour({ 0.025,0.05,0.025 });
		

		switch (dimentions)
		{
		case JEngine::TWO_D:
			for (int i = -10; i < 11; i++)
			{
				if (i == 0) continue;
				CreateLine("defaultGridX" + std::to_string(i)).SetByPosition({ 10,i,0 }, { -10,i,0 }).SetColour(minorColour);
				CreateLine("defaultGridY" + std::to_string(i)).SetByPosition({ i,10,0 }, { i,-10,0 }).SetColour(minorColour);
			}
			break;
		case JEngine::THREE_D:
			CreateLine("defaultGridZaxis").SetByPosition({ 0,0,1000 }, { 0,0,-1000 }).SetColour({ 0.025,0.025,0.05 });

			for (int i = -10; i < 11; i++)
			{
				if (i == 0) continue;
				CreateLine("defaultGridX" + std::to_string(i)).SetByPosition({ 10,0,i }, { -10,0,i }).SetColour(minorColour);
				CreateLine("defaultGridZ" + std::to_string(i)).SetByPosition({ i,0,10 }, { i,0,-10 }).SetColour(minorColour);
			}
			break;
		}
	}

	void GizmoManager::CreateLineObj(GameObject& gObj, glm::vec3 pos, glm::vec3 direction, float length, float lineWidth, glm::vec3 colour)
	{
		std::shared_ptr<Model> model = Model::CreateModelFromPrimative(m_device, Primitives::Line, true, colour);

		gObj.model = model;

		gObj.transform.translation = pos;
		gObj.transform.scale.y = length;

		gObj.transform.scale.x = lineWidth;
		gObj.transform.scale.z = lineWidth;

		gObj.transform.rotation = glm::radians(direction);
	}

	void GizmoManager::SetLineObjColour(GameObject& gObj, glm::vec3 colour)
	{
	}

}