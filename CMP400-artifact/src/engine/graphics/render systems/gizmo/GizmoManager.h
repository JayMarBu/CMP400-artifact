#pragma once
#include "engine/system/Device.h"
#include "engine/graphics/Buffer.h"
#include "engine/GameObject.h"

namespace JEngine
{
	enum Dimentions
	{
		TWO_D,
		THREE_D
	};

	class GizmoManager
	{
		// Members ********************************************************************************
	public:

		struct LineStatic
		{
			glm::vec3 p1{};
			glm::vec3 p2{};
			glm::vec3 colour{1,1,1};

			LineStatic() {}

			inline LineStatic& SetByPosition(glm::vec3 start_point, glm::vec3 end_point) { p1 = start_point; p2 = end_point; return *this; }
			LineStatic& SetByDirection(glm::vec3 start_point, glm::vec3 direction_vector, float length);
			LineStatic& Transform(CTransform& transform);

			inline LineStatic& SetColour(glm::vec3 in_colour) { colour = in_colour; return *this; }
		};

		static const int MAX_LINE_COUNT = 128;

		std::vector<GameObject> linesObjs;

	private:
		JEngine::Device& m_device;

		// static line info
		std::unique_ptr<Buffer> m_vertexBuffer;
		uint32_t m_vertexCount;

		//std::unique_ptr <Buffer> m_stagingBuffer;

		std::unordered_map<std::string,LineStatic> m_lines;

		VkDeviceSize m_bufferSize;

		// dynamic line info
		std::unique_ptr<Buffer> m_dynamicInstanceBuffer;

		// Methods ********************************************************************************
	public:
		GizmoManager(Device& device);
		~GizmoManager();

		REMOVE_COPY_CONSTRUCTOR(GizmoManager);

		LineStatic& CreateLine(const std::string& id);
		LineStatic& GetLine(const std::string& id);

		void WriteToVertexBuffer();

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		void CreateCentreGrid(Dimentions dimentions);

		void CreateLineObj(GameObject& gObj, glm::vec3 pos, glm::vec3 direction, float length, float lineWidth = 1, glm::vec3 colour = {1,1,1});
		void SetLineObjColour(GameObject& gObj, glm::vec3 colour);

	private:
		void CreateVertexBuffer();
	};
}