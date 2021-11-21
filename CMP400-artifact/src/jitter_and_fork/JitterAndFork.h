#pragma once

#include "engine/system/Device.h"
#include "engine/graphics/Buffer.h"
#include "engine/GameObject.h"

namespace JEngine
{
	class JitterAndFork
	{
		// Members ********************************************************************************
	public:

		class Builder
		{
		public:
			Builder(Device& d) : device(d) {}

			inline Builder& SetStartPos(const glm::vec3& pos) { startPos = pos; return *this; }
			inline Builder& SetEndPos(const glm::vec3& pos) { endPos = pos; return *this; }

			inline Builder& SetAngle(const float& ang) { angle = ang; return *this; }
			inline Builder& SetAngleDeviation(const float& dev) { stdDev = dev; return *this; }

			inline Builder& SetIterations(const unsigned int& i) { iterations = i; return *this; }

			inline std::unique_ptr<JitterAndFork> Build() const
			{
				return std::make_unique<JitterAndFork>(device, startPos, endPos, angle, stdDev, iterations);
			}

		private:
			Device& device;

			glm::vec3 startPos;
			glm::vec3 endPos;

			float angle;
			float stdDev;

			unsigned int iterations = 3;
		};

		struct LineSegment
		{
			glm::vec3 p1{};
			glm::vec3 p2{};
			glm::vec3 colour{ 1,1,1 };

			LineSegment() {}

			inline LineSegment& SetByPosition(glm::vec3 start_point, glm::vec3 end_point) { p1 = start_point; p2 = end_point; return *this; }
			LineSegment& SetByDirection(glm::vec3 start_point, glm::vec3 direction_vector, float length);
			LineSegment& Transform(CTransform& transform);

			inline LineSegment& SetColour(glm::vec3 in_colour) { colour = in_colour; return *this; }

			inline glm::vec3 P1() const { return p1; }
			inline glm::vec3 P2() const { return p2; }
		};

		bool displayDebugLines = false;

	private:
		JEngine::Device& m_device;

		std::unique_ptr<Buffer> m_vertexBuffer;
		uint32_t m_vertexCount;

		std::unique_ptr<Buffer> m_debugVertexBuffer;
		uint32_t m_debugVertexCount;

		glm::vec3 m_startPos;
		glm::vec3 m_endPos;

		float m_angle;
		float m_stdDev;

		unsigned int m_iterations;

		// Methods ********************************************************************************
	private:
		void WriteToVertexBuffer(const std::vector<LineSegment>& lines);
		void WriteToDebugVertexBuffer(const std::vector< std::vector<LineSegment>>& lines);

		void GenerateLightning();
		float CalculateBaseAngle(const LineSegment& line) const;
		float CalculateBaseLength(const glm::vec3& startPos, const glm::vec3& endPos) const;
		float CalculateSegmentLength(const float& baseLength, const float& angle) const;
		glm::vec3 CalculateSplitPoint(const glm::vec3& startPos, const float& segment_length, const float& angle) const;

	public:
		JitterAndFork(Device& device, glm::vec3 startPoint, glm::vec3 endPoint, float angle, float stdDev, unsigned int iterations);
		~JitterAndFork();

		REMOVE_COPY_CONSTRUCTOR(JitterAndFork);

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		void DebugBindAndDraw(VkCommandBuffer commandBuffer);

		inline bool CanDrawDebugLines() const { return m_iterations > 1; }
	};
}