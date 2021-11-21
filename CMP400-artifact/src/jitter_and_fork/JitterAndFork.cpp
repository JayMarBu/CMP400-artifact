#include "pch.h"
#include "jitter_and_fork/JitterAndFork.h"

#include "engine/algorithms/Random.h"
#include "engine/algorithms/Interpolate.h"


#include "engine/graphics/render systems/gizmo/StaticLineRenderSystem.h"

namespace JEngine
{
	// ******************************************************************************************//
	//									Line Struct												 //
	// ******************************************************************************************//

	JEngine::JitterAndFork::LineSegment& JitterAndFork::LineSegment::SetByDirection(glm::vec3 start_point, glm::vec3 direction_vector, float length)
	{
		p1 = start_point;
		p2 = p1 + (glm::normalize(direction_vector) * length);

		return *this;
	}

	JEngine::JitterAndFork::LineSegment& JitterAndFork::LineSegment::Transform(CTransform& transform)
	{
		glm::vec4 p1_result = transform.mat4() * glm::vec4(p1.x, p1.y, p1.z, 1);
		glm::vec4 p2_result = transform.mat4() * glm::vec4(p2.x, p2.y, p2.z, 1);

		p1.x = p1_result.x;
		p1.y = p1_result.y;
		p1.z = p1_result.z;

		p2.x = p2_result.x;
		p2.y = p2_result.y;
		p2.z = p2_result.z;

		return *this;
	}

	// ******************************************************************************************//
	//								 lightning Manager class									 //
	// ******************************************************************************************//
	JitterAndFork::JitterAndFork(Device& device, glm::vec3 startPoint, glm::vec3 endPoint, float angle, float stdDev, unsigned int iterations)
		: m_device(device), m_startPos(startPoint), m_endPos(endPoint), m_angle(angle), m_stdDev(stdDev), m_iterations(iterations)
	{
		GenerateLightning();
	}

	JitterAndFork::~JitterAndFork()
	{

	}

	void JitterAndFork::WriteToVertexBuffer(const std::vector<LineSegment>& lines)
	{
		std::vector<StaticLineRenderSystem::Vertex> vertices;

		for (int i = 0; i < lines.size(); i++)
		{
			vertices.push_back({ lines[i].p1, lines[i].colour });
			vertices.push_back({ lines[i].p2, lines[i].colour });
		}

		m_vertexCount = static_cast<uint32_t>(vertices.size());

		assert(m_vertexCount >= 2 && "Vertex count must be at least 2");

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

	void JitterAndFork::WriteToDebugVertexBuffer(const std::vector< std::vector<LineSegment>>& lines)
	{
		std::vector<StaticLineRenderSystem::Vertex> vertices;

		for (int i = 0; i < lines.size(); i++)
		{
			for (int j = 0; j < lines[i].size(); j++)
			{
				vertices.push_back({ lines[i][j].p1, lines[i][j].colour });
				vertices.push_back({ lines[i][j].p2, lines[i][j].colour });
			}
		}

		m_debugVertexCount = static_cast<uint32_t>(vertices.size());

		assert(m_debugVertexCount >= 2 && "Vertex count must be at least 2");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_debugVertexCount;

		uint32_t vertexSize = sizeof(vertices[0]);

		Buffer stagingBuffer{
			m_device,
			vertexSize,
			m_debugVertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)vertices.data());

		m_debugVertexBuffer = std::make_unique<Buffer>(
			m_device,
			vertexSize,
			m_debugVertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		m_device.CopyBuffer(stagingBuffer.GetBuffer(), m_debugVertexBuffer->GetBuffer(), bufferSize);
	}

	void JitterAndFork::GenerateLightning()
	{
		std::vector<LineSegment> previousLayerLines;
		std::vector<LineSegment> currentLayerLines;

		std::vector<std::vector<LineSegment>> lines;

		LineSegment line{};
		line.SetByPosition(m_startPos, m_endPos);

		currentLayerLines.push_back(line);

		for (int i = 0; i < m_iterations; i++)
		{
			previousLayerLines = currentLayerLines;
			currentLayerLines.clear();

			for (int l = 0; l < previousLayerLines.size(); l++)
			{
				int counter = l % 2;

				float r = 0.25+(maths::InvLerp<float>(0, m_iterations, i)*0.75);
				float gb = maths::InvLerp<float>(0, m_iterations + 1, i + 1);
				glm::vec3 colour = {r, gb*(r), gb*(r) };
				glm::vec3 fork_colour = {gb*(r), r, gb*(r) };
				glm::vec3 startPos = previousLayerLines[l].p1;
				glm::vec3 endPos = previousLayerLines[l].p2;
				float base_angle = CalculateBaseAngle(previousLayerLines[i]);

				// Jitter
				float length = CalculateBaseLength(startPos, endPos);

				// TODO: something funky is happening with these angles
				float angle = maths::BoxMullerMarsagliaPolar(m_angle, m_stdDev) - m_angle;
				float seg_len = CalculateSegmentLength(length, angle);

				glm::vec3 splitPos = CalculateSplitPoint(startPos, seg_len, (counter == 0) ? angle+base_angle : angle-base_angle);
				
				// Fork
				glm::vec3 fork_pos = CalculateSplitPoint(splitPos, seg_len * 0.5, (counter == 0) ? (2 * angle) - base_angle : (2 * angle) + base_angle);


				// Build Lines
				LineSegment lines[3]{};

				lines[0].SetByPosition(startPos, splitPos).SetColour(colour);
				lines[1].SetByPosition(splitPos, endPos).SetColour(colour);
				lines[2].SetByPosition(splitPos, fork_pos).SetColour(fork_colour);

				currentLayerLines.push_back(lines[0]);
				currentLayerLines.push_back(lines[1]);
				currentLayerLines.push_back(lines[2]);

			}

			if(i != m_iterations-1)
				lines.push_back(currentLayerLines);
		}
		
		WriteToVertexBuffer(currentLayerLines);
		if(m_iterations > 1)
			WriteToDebugVertexBuffer(lines);

	}

	float JitterAndFork::CalculateBaseAngle(const LineSegment& line) const
	{
		float x = line.P1().x - line.P2().x;
		float y = line.P1().y - line.P2().y;
		float z = line.P1().z - line.P2().z;

		float theta = glm::atan(x / y);

		return -1*glm::degrees(theta);
	}

	float JitterAndFork::CalculateBaseLength(const glm::vec3& startPos, const glm::vec3& endPos) const
	{
		float x = startPos.x - endPos.x;
		float y = startPos.y - endPos.y;
		float z = startPos.z - endPos.z;

		float length = glm::sqrt((x * x) + (y * y) + (z * z));

		return length;
	}

	float JitterAndFork::CalculateSegmentLength(const float& baseLength, const float& angle) const
	{
		float ang = glm::radians(angle);
		return (baseLength * 0.5) / glm::cos(ang);
	}

	glm::vec3 JitterAndFork::CalculateSplitPoint(const glm::vec3& startPos, const float& segment_length, const float& angle) const
	{
		/*
		 xlen = opp
		 ylen = adj
		 segment_length = hyp
		 angle = theta

		 opp = hyp * sin(theta)
		 adj = hyp * cos(theta)
		*/
		float xlen, ylen;

		xlen = segment_length * glm::sin(glm::radians(angle));
		ylen = segment_length * glm::cos(glm::radians(angle));

		return glm::vec3(startPos.x + xlen, startPos.y + ylen, startPos.z);
	}

	void JitterAndFork::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_vertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void JitterAndFork::Draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
	}

	void JitterAndFork::DebugBindAndDraw(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_debugVertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		vkCmdDraw(commandBuffer, m_debugVertexCount, 1, 0, 0);
	}

}