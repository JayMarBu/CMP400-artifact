#pragma once
#include "engine/system/Window.h"
#include "engine/graphics/Renderer.h"
#include "engine/graphics/Model.h"
#include "engine/GameObject.h"
#include "engine/system/FrameTimer.h"

namespace JEngine
{
	class App
	{
		// Members ********************************************************************************
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		JEngine::Window m_window{WIDTH,HEIGHT,"JEngine window"};
		JEngine::Device m_device{ m_window };
		JEngine::Renderer m_renderer{ m_window, m_device };

		JEngine::FrameTimer m_timer;

		std::vector<GameObject> m_gameObjects;

		// Methods ********************************************************************************
	public:
		App();
		~App();

		REMOVE_COPY_CONSTRUCTOR(App)

		void run();
	private:
		void LoadGameObjects();

		void RenderGameObjects(VkCommandBuffer commandBuffer);

		void DrawFrame();
	};

}