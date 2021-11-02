#pragma once
#include "engine/graphics/Model.h"

namespace JEngine
{

	struct CTransform2D
	{
		glm::vec2 translation{};
		float rotation{};
		glm::vec2 scale{1.f,1.f};

		glm::mat2 mat2() 
		{ 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);

			glm::mat2 scaleMat = { {scale.x, 0} , {0, scale.y} };
			glm::mat2 rotMat = { {c, -s} , {s, -c} };
			return rotMat * scaleMat;
		}

		operator glm::mat2() { return mat2(); }
	};

	class GameObject
	{
		// Members ********************************************************************************
	public:
		using id_t = unsigned int;

		std::shared_ptr<Model> model{};
		glm::vec3 colour{};
		CTransform2D transform{};

	private:

		id_t id;
		// Methods ********************************************************************************
	public:

		REMOVE_COPY_CONSTRUCTOR(GameObject);

		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		static GameObject Create()
		{
			static id_t currentId = 0;
			return GameObject(currentId++);
		}

		id_t getID() { return id; }

	private:
		GameObject(id_t objID) : id{ objID } {}
	};
}