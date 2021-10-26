#include "pch.h"
#include "engine/Window.h"

JEngine::Window::Window(const int& w, const int& h, const std::string& _name)
	: width(w), height(h), name(_name)
{
	InitWindow();
}

void JEngine::Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
}

JEngine::Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void JEngine::Window::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
}
