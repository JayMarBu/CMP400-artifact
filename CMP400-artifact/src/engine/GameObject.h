#pragma once
#include "engine/graphics/models/Model.h"

namespace JEngine
{

	struct CTransform
	{
	public:
		glm::vec3 translation{0.f,0.f,0.f};
		glm::vec3 rotation{0.f,0.f,0.f};
		glm::vec3 scale{1.f,1.f,1.f};

		glm::mat4 mat4();

		glm::mat3 invRotationMat();

		glm::mat3 normalMatrix();

		operator glm::mat4() { return mat4(); }
	};



	class GameObject
	{
		// Members ********************************************************************************
	public:
		using id_t = unsigned int;

		std::shared_ptr<Model> model{};
		glm::vec3 colour{};
		CTransform transform{};

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