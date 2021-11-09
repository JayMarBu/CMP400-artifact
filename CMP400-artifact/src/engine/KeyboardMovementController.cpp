#include "pch.h"
#include "engine/KeyboardMovementController.h"

namespace JEngine
{

	void KeyboardMovementController::MoveInPlaneXZ(Window* window, float dt, GameObject& gameObject, Input* input)
	{
		glm::vec3 rotate{ 0.0f };

		if (glfwGetKey(window->GetGLFWWindow(), keys.lookRight) == GLFW_PRESS)
			rotate.y += 1;
		if (glfwGetKey(window->GetGLFWWindow(), keys.lookLeft) == GLFW_PRESS)
			rotate.y -= 1;
		if (glfwGetKey(window->GetGLFWWindow(), keys.lookUp) == GLFW_PRESS)
			rotate.x += 1;
		if (glfwGetKey(window->GetGLFWWindow(), keys.lookDown) == GLFW_PRESS)
			rotate.x -= 1;

		if (input->isRightMouseDown())
		{
			glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			deltax = input->getMouseX() - (window->getExtent().width / 2);
			deltay = input->getMouseY() - (window->getExtent().height / 2);

			rotate.y += turnSpeed * dt * (float)deltax;
			rotate.x -= turnSpeed * dt * (float)deltay;

			input->SetMouseToCentre(window);
		}
		else
		{
			glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if(VEC_IS_ZERO(rotate))
			gameObject.transform.rotation += turnSpeed * dt * glm::normalize(rotate);

		gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

		float yaw = gameObject.transform.rotation.y;
		const glm::vec3 forwardDir{ sin(yaw), 0.0f, cos(yaw) };
		const glm::vec3 rightDir{ forwardDir.z, 0.0f, -forwardDir.x };
		const glm::vec3 upDir{ 0.0f, -1.0f, 0.0f };

		glm::vec3 moveDir{ 0 };

		//if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
		if (input->isKeyDown(keys.moveRight))
			moveDir += rightDir;
		if (input->isKeyDown(keys.moveLeft))
			moveDir -= rightDir;
		if (input->isKeyDown(keys.moveForward))
			moveDir += forwardDir;
		if (input->isKeyDown(keys.moveBack))
			moveDir -= forwardDir; 
		if (input->isKeyDown(keys.moveUp))
			moveDir += upDir;
		if (input->isKeyDown(keys.moveDown))
			moveDir -= upDir;

		if (VEC_IS_ZERO(moveDir))
			gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
	}

}