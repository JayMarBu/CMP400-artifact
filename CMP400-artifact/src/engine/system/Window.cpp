#include "pch.h"
#include "engine/system/Window.h"

JEngine::Window::Window(const int& w, const int& h, const std::string& _name, Input* input)
	: width(w), height(h), name(_name), m_input(input)
{
	InitWindow();
}

void JEngine::Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
}

void JEngine::Window::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* thisWindow = (Window*)glfwGetWindowUserPointer(window);

	switch (action)
	{
	case GLFW_PRESS:
		thisWindow->m_input->SetKeyDown(key);
		break;

	case GLFW_RELEASE:
		thisWindow->m_input->SetKeyUp(key);
		break;

	default:
		break;
	}
}

void JEngine::Window::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	Window* thisWindow = (Window*)glfwGetWindowUserPointer(window);

	thisWindow->m_input->setMouseX((int)std::round(xpos));
	thisWindow->m_input->setMouseY((int)std::round(ypos));
}

void JEngine::Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* thisWindow = (Window*)glfwGetWindowUserPointer(window);

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		switch (action)
		{
		case GLFW_PRESS:
			thisWindow->m_input->setLeftMouse(true);
			break;

		case GLFW_RELEASE:
			thisWindow->m_input->setLeftMouse(false);
			break;
		}
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		switch (action)
		{
		case GLFW_PRESS:
			thisWindow->m_input->setRightMouse(true);
			break;

		case GLFW_RELEASE:
			thisWindow->m_input->setRightMouse(false);
			break;
		}
		break;
	}
}

void JEngine::Window::MouseScrollWheelCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* thisWindow = (Window*)glfwGetWindowUserPointer(window);

}

void JEngine::Window::CursorEnterCallback(GLFWwindow* window, int entered)
{
	Window* thisWindow = (Window*)glfwGetWindowUserPointer(window);

	if (entered)
	{
		// The cursor entered the content area of the window
		thisWindow->m_input->setMouseActive(true);
	}
	else
	{
		// The cursor left the content area of the window
		thisWindow->m_input->setMouseActive(false);
	}
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
	m_input->SetWindowSize(width, height);
	m_input->setMouseX(width / 2);
	m_input->setMouseY(height / 2);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, FrameBufferResizedCallback);
	glfwSetKeyCallback(m_window, KeyCallBack);
	glfwSetCursorPosCallback(m_window, CursorPositionCallback);
	glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
	glfwSetCursorEnterCallback(m_window, CursorEnterCallback);
}

void JEngine::Window::FrameBufferResizedCallback(GLFWwindow* window, int w, int h)
{
	auto Jwindow = reinterpret_cast<JEngine::Window*>(glfwGetWindowUserPointer(window));
	Jwindow->frameBufferResized = true;
	Jwindow->width = w;
	Jwindow->height = h;

	Jwindow->m_input->SetWindowSize(w, h);
}
