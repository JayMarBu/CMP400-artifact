#include "pch.h"
#include "engine/App.h"

#include "engine/graphics/SimpleRenderSystem.h"

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

		while (!m_window.ShouldClose())
		{
			glfwPollEvents();

			if (auto commandBuffer = m_renderer.BeginFrame())
			{
				m_renderer.BeginSwapChainRenderPass(commandBuffer);

				simpleRenderSystem.RenderGameObjects(commandBuffer, m_gameObjects);

				m_renderer.EndSwapChainRenderPass(commandBuffer);
				m_renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_device);
	}

	void App::LoadGameObjects()
	{
		std::vector<Model::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1,0,0}},
			{{0.5f, 0.5f}, {0,1,0}},
			{{-0.5f, 0.5f}, {0,0,1}}
		};

		auto model = std::make_shared<Model>(m_device, vertices);

		auto triangle = GameObject::Create();
		triangle.model = model;
		triangle.colour = { .1f, .8f, .1f };
		triangle.transform.translation.x = .2f;
		triangle.transform.translation.y = 0;
		triangle.transform.scale.x = 2.f;
		triangle.transform.scale.y = 0.5f;
		triangle.transform.rotation = .25f * glm::two_pi<float>();

		m_gameObjects.push_back(std::move(triangle));
	}
}


