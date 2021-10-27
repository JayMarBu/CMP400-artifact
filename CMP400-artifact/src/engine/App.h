#pragma once
#include "engine/Window.h"
#include "engine/Pipeline.h"
#include "engine/SwapChain.h"
#include "engine/Model.h"

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
		JEngine::SwapChain m_swapChain{m_device, m_window.getExtent()};
		/*JEngine::Pipeline m_pipeline{ 
			m_device,
			"shaders/simple_shader/simple_shader.vert.spv",
			"shaders/simple_shader/simple_shader.frag.spv",
			Pipeline::DefaultPipelineConfigInfo(WIDTH,HEIGHT)};*/

		std::unique_ptr<JEngine::Pipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;
		std::vector<VkCommandBuffer> m_commandBuffers;

		std::unique_ptr<JEngine::Model> m_model;

		// Methods ********************************************************************************
	public:
		App();
		~App();

		REMOVE_COPY_CONSTRUCTOR(App)

		void run();
	private:
		void LoadModels();
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();

		void DrawFrame();
	};

}