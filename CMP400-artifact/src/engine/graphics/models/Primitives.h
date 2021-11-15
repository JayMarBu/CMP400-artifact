#pragma once
#include "engine/graphics/models/Model.h"

namespace JEngine
{

	namespace Primitives
	{
		void Cube(std::vector<Model::Vertex>* vertices, std::vector<uint32_t>* indices, glm::vec3 colour = {1,1,1});

		void Line(std::vector<Model::Vertex>* vertices, std::vector<uint32_t>* indices, glm::vec3 colour = { 1,1,1 });
	}
}