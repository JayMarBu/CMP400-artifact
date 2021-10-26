#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace JEngine
{
	class Window
	{
		// Members ********************************************************************************
	private:
		GLFWwindow* m_window;

		const int height;
		const int width;

		std::string name;

		// Methods ********************************************************************************
	public:
		Window(const int& w, const int& h, const std::string& _name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		inline bool ShouldClose() const { return glfwWindowShouldClose(m_window); }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		inline VkExtent2D getExtent() const { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

	private:
		void InitWindow();
	};
}