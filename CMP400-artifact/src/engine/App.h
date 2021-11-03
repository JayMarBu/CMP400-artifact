#pragma once
#include "engine/system/Window.h"
#include "engine/graphics/Renderer.h"
#include "engine/graphics/Model.h"
#include "engine/GameObject.h"
#include "engine/system/FrameTimer.h"
#include "engine/graphics/Descriptors.h"

#include "engine/Camera.h"
#include "engine/KeyboardMovementController.h"

#include "engine/graphics/Buffer.h"

#include "engine/graphics/SimpleRenderSystem.h"

namespace JEngine
{
	struct CameraWrapper
	{
		CameraWrapper() : gObject(GameObject::Create()) {}

		Camera camera{};
		GameObject gObject;
		KeyboardMovementController controller{};
	};

	class App
	{
		// Members ********************************************************************************
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		JEngine::Window m_window{ WIDTH,HEIGHT,"JEngine window" };
		JEngine::Device m_device{ m_window };
		JEngine::Renderer m_renderer{ m_window, m_device };

		JEngine::FrameTimer m_timer;

		std::unique_ptr<DescriptorPool> m_globalPool;
		std::unique_ptr<DescriptorSetLayout> m_globalSetLayout;
		std::vector<VkDescriptorSet> m_globalDescriptorSets;

		std::vector<std::unique_ptr<Buffer>> m_UBObuffers;

		std::unique_ptr<SimpleRenderSystem> m_simpleRenderSystem;

		std::vector<GameObject> m_gameObjects;
		CameraWrapper m_camera{};

		

		// Methods ********************************************************************************
	public:
		App();
		~App();

		REMOVE_COPY_CONSTRUCTOR(App)

		void run();

	private:
		void Init();
		void Update();
		void Render();
		void LoadGameObjects();

		void InitUBO();
		void InitDescriptorPool();
	};

}