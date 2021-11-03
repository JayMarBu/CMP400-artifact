#include "pch.h"
#include "engine/App.h"

#include "engine/graphics/SimpleRenderSystem.h"
#include "engine/Camera.h"

#include "engine/graphics/Buffer.h"

#include "engine/KeyboardMovementController.h"

#include "engine/graphics/FrameInfo.h"

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
			.build();

		LoadGameObjects();
	}

	App::~App()
	{
		
	}

	void App::run()
	{
		
		Camera camera{};
		
		auto camObj = GameObject::Create();
		KeyboardMovementController camController{};

		std::vector<std::unique_ptr<Buffer>> UBObuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < UBObuffers.size(); i++)
		{
			UBObuffers[i] = std::make_unique<Buffer>(
				m_device,
				sizeof(GlobalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				std::lcm(
					m_device.properties.limits.minUniformBufferOffsetAlignment,
					m_device.properties.limits.nonCoherentAtomSize)
				);

			UBObuffers[i]->Map();
		}

		auto globalSetLayout = DescriptorSetLayout::Builder(m_device)
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.Build();

		std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = UBObuffers[i]->DescriptorInfo();

			DescriptorWriter(*globalSetLayout, *m_globalPool)
				.WriteBuffer(0, &bufferInfo)
				.Build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{ m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->GetDescriptorSetLayout()};

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
				FrameInfo fInfo{
					m_renderer.GetFrameIndex(),
					m_timer.getTime(),
					commandBuffer,
					camera,
					globalDescriptorSets[m_renderer.GetFrameIndex()]
				};

				// update

				GlobalUBO ubo{};
				ubo.projectionView = camera.GetProjection() * camera.GetView();

				//UBObuffers[fInfo.frameIndex]->Map();

				UBObuffers[fInfo.frameIndex]->WriteToBuffer(&ubo);
				UBObuffers[fInfo.frameIndex]->Flush();

				//UBObuffers[fInfo.frameIndex]->Unmap();

				// render
				m_renderer.BeginSwapChainRenderPass(commandBuffer);

				simpleRenderSystem.RenderGameObjects(fInfo, m_gameObjects);

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


