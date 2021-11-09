#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "engine/system/Input.h"

namespace JEngine
{
	class Window
	{
		// Members ********************************************************************************
	private:
		GLFWwindow* m_window;

		Input* m_input;

		int height;
		int width;
		bool frameBufferResized = false;

		std::string name;

		// Methods ********************************************************************************
	public:
		Window(const int& w, const int& h, const std::string& _name, Input* input);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		inline bool ShouldClose() const { return glfwWindowShouldClose(m_window); }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		inline VkExtent2D getExtent() const { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool WasWindowResized() { return frameBufferResized; }
		void ResetWindowResizedFlag() { frameBufferResized = false; }
		inline GLFWwindow* GetGLFWWindow() const { return m_window; }

	private:
		void InitWindow();

		static void FrameBufferResizedCallback(GLFWwindow* window, int w, int h);
		static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	};
}