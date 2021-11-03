#include "pch.h"
#include "engine/App.h"

#include "engine/graphics/SimpleRenderSystem.h"
#include "engine/Camera.h"

#include "engine/KeyboardMovementController.h"

namespace JEngine
{
	App::App()
	{
		LoadGameObjects();
	}

	App::~App()
	{
		
	}

	void App::run()
	{
		SimpleRenderSystem simpleRenderSystem{m_device, m_renderer.getSwapChainRenderPass()};
		Camera camera{};
		
		auto camObj = GameObject::Create();
		KeyboardMovementController camController{};

		while (!m_window.ShouldClose())
		{
			m_timer.frame();

			glfwPollEvents();

			camController.MoveInPlaneXZ(m_window.GetGLFWWindow(), m_timer.getTime(), camObj);
			camera.SetViewXYZ(camObj.transform.translation, camObj.transform.rotation);

			float aspect = m_renderer.GetAspectRatio();
			camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.0f);

			if (auto commandBuffer = m_renderer.BeginFrame())
			{
				m_renderer.BeginSwapChainRenderPass(commandBuffer);

				simpleRenderSystem.RenderGameObjects(commandBuffer, m_gameObjects, camera, m_timer.getTime());

				m_renderer.EndSwapChainRenderPass(commandBuffer);
				m_renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_device);
	}

	void App::LoadGameObjects()
	{
		std::shared_ptr<Model> model = Model::CreateModelFromFile(m_device, "models/flat_vase.obj");

		auto cube = GameObject::Create();

		cube.model = model;
		cube.transform.translation = { 0.f,0.5f,2.5f };
		cube.transform.scale = glm::vec3(3.f);

		m_gameObjects.push_back(std::move(cube));
	}
}


