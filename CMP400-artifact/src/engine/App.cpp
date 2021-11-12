#include "pch.h"
#include "engine/App.h"

#include "engine/graphics/FrameInfo.h"

#include "engine/graphics/models/Primatives.h"

namespace JEngine
{
	struct GlobalUBO
	{
		glm::mat4 projectionView{1.0f};
		glm::vec3 lightDir = glm::normalize(glm::vec3(1,-3,-1));
	};

	App::App()
	{
		m_globalPool = DescriptorPool::Builder(m_device)
			.SetMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();

		
	}

	App::~App()
	{
		
	}

	// Initializtion methods **********************************************************************
	void App::Init()
	{
		m_imguiManager = std::make_unique<GuiManager>(
			m_window,
			m_device,
			m_renderer.getSwapChainRenderPass(),
			m_renderer.GetImageCount()
			);


		LoadGameObjects();
		InitTextures();
		InitUBO();
		InitDescriptorPool();
		InitRenderSystems();
	}

	void App::InitUBO()
	{
		m_UBObuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < m_UBObuffers.size(); i++)
		{
			m_UBObuffers[i] = std::make_unique<Buffer>(
				m_device,
				sizeof(GlobalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				std::lcm(
					m_device.properties.limits.minUniformBufferOffsetAlignment,
					m_device.properties.limits.nonCoherentAtomSize)
				);

			m_UBObuffers[i]->Map();
		}
	}

	void App::InitDescriptorPool()
	{
		m_globalSetLayout = DescriptorSetLayout::Builder(m_device)
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.Build();

		m_globalDescriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < m_globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = m_UBObuffers[i]->DescriptorInfo();
			auto imageInfo = m_textureManager.ImageInfo(m_sampler,"bunny");

			DescriptorWriter(*m_globalSetLayout, *m_globalPool)
				.WriteBuffer(0, &bufferInfo)
				.WriteImage(1, &imageInfo)
				.Build(m_globalDescriptorSets[i]);
		}

		//m_simpleRenderSystem = std::make_unique<SimpleRenderSystem>(m_device, m_renderer.getSwapChainRenderPass(), m_globalSetLayout->GetDescriptorSetLayout());
	}

	void App::InitRenderSystems()
	{
		m_simpleRenderSystem = std::make_unique<SimpleRenderSystem>(m_device, m_renderer.getSwapChainRenderPass(), m_globalSetLayout->GetDescriptorSetLayout());
		m_gizmoRenderSystem = std::make_unique<StaticLineRenderSystem>(m_device, m_renderer.getSwapChainRenderPass());
	}

	void App::InitTextures()
	{
		m_textureManager.LoadTexture("bunny", "media/textures/bunny.png");
	}

	void App::LoadGameObjects()
	{
		//std::shared_ptr<Model> model = Model::CreateModelFromFile(m_device, "models/obamium.obj");
		std::shared_ptr<Model> model = Model::CreateModelFromPrimative(m_device, Primatives::Cube, false);

		auto cube = GameObject::Create();

		cube.model = model;
		cube.transform.translation = { 0.f,0.f,.5f };
		cube.transform.scale = glm::vec3(0.1f);

		m_gameObjects.push_back(std::move(cube));
	}

	// Run method *********************************************************************************
	void App::run()
	{
		Init();

		while (!m_window.ShouldClose())
		{
			m_timer.frame();
			glfwPollEvents();

			Update();

			Render();
		}

		vkDeviceWaitIdle(m_device);
	}

	// Main Loop Methods **************************************************************************
	void App::Update()
	{
		m_camera.controller.MoveInPlaneXZ(&m_window, m_timer.getTime(), m_camera.gObject, &m_input);
		m_camera.camera.SetViewXYZ(m_camera.gObject.transform.translation, m_camera.gObject.transform.rotation);

		float aspect = m_renderer.GetAspectRatio();
		m_camera.camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.0f);
	}

	void App::Render()
	{
		auto commandBuffer = m_renderer.BeginFrame();
		if (!commandBuffer)
			return;
		
		m_imguiManager->newFrame();

		// ****** fill frame info struct ******
		FrameInfo fInfo{
			m_renderer.GetFrameIndex(),
			m_timer.getTime(),
			commandBuffer,
			m_camera.camera,
			m_globalDescriptorSets[m_renderer.GetFrameIndex()]
		};

		// ****** update uniform buffers ******
		GlobalUBO ubo{};
		ubo.projectionView = m_camera.camera.GetProjection() * m_camera.camera.GetView();

		m_UBObuffers[fInfo.frameIndex]->WriteToBuffer(&ubo);
		m_UBObuffers[fInfo.frameIndex]->Flush();

		// ****** render geometry *******
		m_renderer.BeginSwapChainRenderPass(commandBuffer);
		// render your stuff here

		m_simpleRenderSystem->RenderGameObjects(fInfo, m_gameObjects);

		m_gizmoRenderSystem->RenderGizmos(fInfo, m_gizmoManager);
		
		DrawGui();

		// stop rendering your stuff
		m_imguiManager->render(commandBuffer);
		m_renderer.EndSwapChainRenderPass(commandBuffer);
		m_renderer.EndFrame();
		
	}


	void App::DrawGui()
	{
		ImGui::Begin("Test");

		ImGui::Text(
			"Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / m_timer.getFPS(),
			m_timer.getFPS());

		ImGui::Text(
			"Mouse position: [%i, %i]",
			m_input.getMouseX(),
			m_input.getMouseY());

		m_camera.controller.DrawGui(m_camera.gObject);

		ImGui::End();
	}
}


