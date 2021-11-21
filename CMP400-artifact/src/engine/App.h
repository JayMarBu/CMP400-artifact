#pragma once
#include "engine/system/Input.h"
#include "engine/system/Window.h"
#include "engine/graphics/Renderer.h"
#include "engine/graphics/models/Model.h"
#include "engine/GameObject.h"
#include "engine/system/FrameTimer.h"
#include "engine/graphics/Descriptors.h"

#include "engine/Camera.h"
#include "engine/KeyboardMovementController.h"

#include "engine/graphics/Buffer.h"

#include "engine/graphics/render systems/SimpleRenderSystem.h"
#include "engine/graphics/render systems/gizmo/StaticLineRenderSystem.h"

#include "engine/GuiManager.h"

#include "engine/graphics/textures/TextureManager.h"
#include "engine/graphics/textures/Sampler.h"

#include "engine/graphics/render systems/gizmo/GizmoManager.h"

#include "engine/algorithms/LSystem.h"

#include "jitter_and_fork/JitterAndFork.h"

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
		static constexpr int WIDTH = 1440;
		static constexpr int HEIGHT = 816;

	private:
		JEngine::Input m_input;
		JEngine::Window m_window{ WIDTH,HEIGHT,"JEngine window", &m_input};
		JEngine::Device m_device{ m_window };
		JEngine::Renderer m_renderer{ m_window, m_device };
		JEngine::TextureManager m_textureManager{m_device};

		JEngine::FrameTimer m_timer;

		std::unique_ptr<DescriptorPool> m_globalPool;
		std::unique_ptr<DescriptorSetLayout> m_globalSetLayout;
		std::vector<VkDescriptorSet> m_globalDescriptorSets;

		std::vector<std::unique_ptr<Buffer>> m_UBObuffers;

		std::unique_ptr<GuiManager> m_imguiManager;

		std::unique_ptr<SimpleRenderSystem> m_simpleRenderSystem;
		std::unique_ptr<SimpleRenderSystem> m_simpleNoShadingRenderSystem;
		std::unique_ptr<StaticLineRenderSystem> m_gizmoRenderSystem;

		std::vector<GameObject> m_gameObjects;
		CameraWrapper m_camera{};

		Sampler m_sampler{m_device};

		std::unique_ptr <GizmoManager> m_gizmoManager;
		std::unique_ptr<JitterAndFork> m_jitterAndFork;

		struct BoxMooreWrapper
		{
			float mean;
			float standardDeviation;

			std::vector<double> numsX;
			std::vector<double> numsY;

			void Reset()
			{
				numsX.clear();
				numsY.clear();
			}
		};

		BoxMooreWrapper m_example1;

		LSystem m_lsystem;

		struct LightningDemoWrapper
		{
			int iterations = 1;
			float angle = 40;
			float stdDev = 12;
			bool displayDebugLines = false;
		};

		LightningDemoWrapper m_lightningDemo;


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
		void InitGameObjects();

		void InitUBO();
		void InitDescriptorPool();
		void InitRenderSystems();
		void InitTextures();

		void GenerateGui();
		void BoxMooreExamples();
		void BoxMooreExmpl1();

		void LSystemGui();

		void BuildLightning();

	};

}