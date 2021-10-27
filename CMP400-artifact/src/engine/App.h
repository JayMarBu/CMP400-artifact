#pragma once
#include "engine/Window.h"
#include "engine/Pipeline.h"
#include "engine/SwapChain.h"
#include "engine/Model.h"

namespace JEngine
{
	struct SimplePushConstantData
	{
		glm::vec2 offset;
		alignas(16) glm::vec3 colour;
	};

	class App
	{
		// Members ********************************************************************************
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		JEngine::Window m_window{WIDTH,HEIGHT,"JEngine window"};
		JEngine::Device m_device{ m_window };
		std::unique_ptr<JEngine::SwapChain> m_swapChain;

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
		void FreeCommandBuffers();

		void RecreateSwapChain();
		void RecordCommandBuffer(uint32_t imageIndex);

		void DrawFrame();
	};

}