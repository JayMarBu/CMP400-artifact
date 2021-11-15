#include "pch.h"
#include "engine/App.h"

#include "engine/graphics/FrameInfo.h"

#include "engine/graphics/models/Primatives.h"

#include "engine/maths/Random.h"

namespace JEngine
{
	struct GlobalUBO
	{
		glm::mat4 projectionView{1.0f};
		glm::vec3 lightDir = glm::normalize(glm::vec3(1,-3,-1));
	};

	App::App()
		:m_gizmoManager(m_device)
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

		m_gizmoManager.CreateCentreGrid(TWO_D);
		m_gizmoManager.WriteToVertexBuffer();

		m_example1.mean = 40;
		m_example1.standardDeviation = 2;

		InitGameObjects();
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

	void App::InitGameObjects()
	{
		m_camera.gObject.transform.translation = { 0.0f,0.0f,-5 };

		//std::shared_ptr<Model> model = Model::CreateModelFromFile(m_device, "models/obamium.obj");
		std::shared_ptr<Model> model = Model::CreateModelFromPrimative(m_device, Primatives::Line);

		auto cube = GameObject::Create();

		cube.model = model;
		cube.transform.translation = { 0.f,0.f,-.01f };
		//cube.transform.scale = glm::vec3(0.1f);

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
		m_imguiManager->newFrame();
		m_input.setMouseBlocked(m_imguiManager->mouseIsInGuiBounds);

		GenerateGui();

		m_camera.controller.MoveInPlaneXZ(&m_window, m_timer.getTime(), m_camera.gObject, &m_input);
		//m_camera.controller.MoveInPlaneXY(&m_window, m_timer.getTime(), m_camera.gObject, &m_input);
		m_camera.camera.SetViewXYZ(m_camera.gObject.transform.translation, m_camera.gObject.transform.rotation);

		float aspect = m_renderer.GetAspectRatio();
		m_camera.camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.0f);
		//m_camera.camera.SetOthographicProjection({ -8,2,-5 }, { 8,-2,5 });
	}

	void App::Render()
	{
		auto commandBuffer = m_renderer.BeginFrame();
		if (!commandBuffer)
			return;

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

		// stop rendering your stuff
		m_imguiManager->render(commandBuffer);
		m_renderer.EndSwapChainRenderPass(commandBuffer);
		m_renderer.EndFrame();
		
	}


	void App::GenerateGui()
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

		BoxMooreExamples();

		ImGui::End();
	}

	void App::BoxMooreExamples()
	{
		if (ImGui::CollapsingHeader("Box-Moore examples"))
		{
			ImGui::Indent(10);

			BoxMooreExmpl1();

			ImGui::Indent(-10);
		}
	}

	void App::BoxMooreExmpl1()
	{
		if (ImGui::CollapsingHeader("box moore example 1: 2d distribution"))
		{
			ImGui::Indent(10);
			static int NumGenerations = 1;
			bool hasData = !m_example1.numsX.empty() && !m_example1.numsY.empty();

			// ****** editable parameters ****** 
			if (ImGui::InputInt("Number of Points Generated##int", &NumGenerations)) m_example1.Reset();
			if (ImGui::InputFloat("Mean value", &m_example1.mean)) m_example1.Reset();
			if (ImGui::InputFloat("standard deviation value", &m_example1.standardDeviation)) m_example1.Reset();

			// ****** buttons ****** 
			ImGui::Text(("data point count: " + std::to_string(m_example1.numsX.size())).c_str());
			ImGui::Columns(2, "##example_1 columns buttons");
			if (ImGui::Button("Generate Box-Moore value##button"))
			{
				for (int i = 0; i < NumGenerations; i++)
				{
					auto nums = maths::BoxMuller2D(m_example1.mean, m_example1.standardDeviation);
					m_example1.numsX.push_back(nums.first);
					m_example1.numsY.push_back(nums.second);
				}
			}

			ImGui::NextColumn();
			if (ImGui::Button("Clear List##button")) m_example1.Reset();
			ImGui::Columns();

			// ****** plots ****** 
			if (ImPlot::BeginPlot("test plot"))
			{
				if (hasData)
				{
					ImPlot::PlotScatter("box-moore scatter", m_example1.numsX.data(), m_example1.numsY.data(), static_cast<uint32_t>(m_example1.numsX.size()));
				}
				ImPlot::EndPlot();
			}

			ImGui::Columns(2, "##example_1 columns");

			if (ImPlot::BeginPlot("test plot 2 x"))
			{
				if (hasData)
				{
					ImPlot::PlotHistogram("box-moore histogram", m_example1.numsX.data(), m_example1.numsX.size());
				}
				ImPlot::EndPlot();
			}

			ImGui::NextColumn();


			if (ImPlot::BeginPlot("test plot 2 y"))
			{
				if (hasData)
				{
					ImPlot::PlotHistogram("box-moore histogram", m_example1.numsY.data(), m_example1.numsY.size());
				}
				ImPlot::EndPlot();
			}
			ImGui::Columns();
			ImGui::Indent(-10);
		}
	}

}


