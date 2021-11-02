#pragma once
#include "engine/graphics/Model.h"

namespace JEngine
{

	struct CTransform
	{
	public:
		glm::vec3 translation{0.f,0.f,0.f};
		glm::vec3 rotation{0.f,0.f,0.f};
		glm::vec3 scale{1.f,1.f,1.f};

		glm::mat4 mat4() {
			const float c3 = glm::cos(rotation.z);
			const float s3 = glm::sin(rotation.z);
			const float c2 = glm::cos(rotation.x);
			const float s2 = glm::sin(rotation.x);
			const float c1 = glm::cos(rotation.y);
			const float s1 = glm::sin(rotation.y);
			return glm::mat4{
				{
					scale.x * (c1 * c3 + s1 * s2 * s3),
					scale.x * (c2 * s3),
					scale.x * (c1 * s2 * s3 - c3 * s1),
					0.0f,
				},
				{
					scale.y * (c3 * s1 * s2 - c1 * s3),
					scale.y * (c2 * c3),
					scale.y * (c1 * c3 * s2 + s1 * s3),
					0.0f,
				},
				{
					scale.z * (c2 * s1),
					scale.z * (-s2),
					scale.z * (c1 * c2),
					0.0f,
				},
				{translation.x, translation.y, translation.z, 1.0f} };
		}

		operator glm::mat4() { return mat4(); }

	//private:
	//	glm::quat rotation_quat;
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