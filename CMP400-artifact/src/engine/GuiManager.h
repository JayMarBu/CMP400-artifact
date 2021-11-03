#pragma once

#include "engine/system/Device.h"
#include "engine/system/Window.h"

#define WIN32_LEAN_AND_MEAN
//#define IMGUI_IMPLEMENTATION
#include <imgui.h>
//#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

namespace JEngine
{
	static void check_vk_result(VkResult err) {
		if (err == 0) return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0) abort();
	}

	class GuiManager {
	public:
		GuiManager(Window& window, Device& device, VkRenderPass renderPass, uint32_t imageCount);
		~GuiManager();

		void newFrame();

		void render(VkCommandBuffer commandBuffer);

		// Example state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		void runExample();

	private:
		Device& m_device;

		// We haven't yet covered descriptor pools in the tutorial series
		// so I'm just going to create one for just imgui and store it here for now.
		// maybe its preferred to have a separate descriptor pool for imgui anyway,
		// I haven't looked into imgui best practices at all.
		VkDescriptorPool descriptorPool;
	};
}