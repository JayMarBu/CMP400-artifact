#pragma once
#include "engine/graphics/models/Model.h"

namespace JEngine
{

	namespace Primatives
	{
		void Cube(std::vector<Model::Vertex>* vertices, std::vector<uint32_t>* indices);

		void Line(std::vector<Model::Vertex>* vertices, std::vector<uint32_t>* indices);
	}
}