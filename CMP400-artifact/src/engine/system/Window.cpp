#include "pch.h"
#include "engine/system/Window.h"

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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, FrameBufferResizedCallback);
}

void JEngine::Window::FrameBufferResizedCallback(GLFWwindow* window, int w, int h)
{
	auto Jwindow = reinterpret_cast<JEngine::Window*>(glfwGetWindowUserPointer(window));
	Jwindow->frameBufferResized = true;
	Jwindow->width = w;
	Jwindow->height = h;

}
