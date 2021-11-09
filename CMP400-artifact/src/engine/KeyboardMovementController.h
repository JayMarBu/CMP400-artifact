#pragma once

#include "engine/GameObject.h"
#include "engine/system/Window.h"

namespace JEngine
{
	class KeyboardMovementController
	{
		// Members ********************************************************************************
	public:

		struct KeyMappings
		{
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBack = GLFW_KEY_S;
			int moveUp = GLFW_KEY_SPACE;
			int moveDown = GLFW_KEY_Q;

			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		KeyMappings keys{};

		float moveSpeed{ 1.f };
		float turnSpeed{2.5f};

		int deltax, deltay;
		// Methods ********************************************************************************

	public:

		void MoveInPlaneXZ(Window* window, float dt, GameObject &gameObject, Input* input);
	};
}