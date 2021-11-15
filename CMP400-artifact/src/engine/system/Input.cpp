#include "pch.h"
#include "engine/system/Input.h"
#include "engine/system/Window.h"

namespace JEngine
{
	std::unordered_map<int, int> Input::key_index_map = {
		{GLFW_KEY_SPACE              ,0},
		{GLFW_KEY_APOSTROPHE         ,1},/* ' */
		{GLFW_KEY_COMMA              ,2},/* , */
		{GLFW_KEY_MINUS              ,3},/* - */
		{GLFW_KEY_PERIOD             ,4},/* . */
		{GLFW_KEY_SLASH              ,5},/* / */
		{GLFW_KEY_0                  ,6},
		{GLFW_KEY_1                  ,7},
		{GLFW_KEY_2                  ,8},
		{GLFW_KEY_3                  ,9},
		{GLFW_KEY_4                  ,10},
		{GLFW_KEY_5                  ,11},
		{GLFW_KEY_6                  ,12},
		{GLFW_KEY_7                  ,13},
		{GLFW_KEY_8                  ,14},
		{GLFW_KEY_9                  ,15},
		{GLFW_KEY_SEMICOLON          ,16},/* ; */
		{GLFW_KEY_EQUAL              ,17},/* = */
		{GLFW_KEY_A                  ,18},
		{GLFW_KEY_B                  ,19},
		{GLFW_KEY_C                  ,20},
		{GLFW_KEY_D                  ,21},
		{GLFW_KEY_E                  ,22},
		{GLFW_KEY_F                  ,23},
		{GLFW_KEY_G                  ,24},
		{GLFW_KEY_H                  ,25},
		{GLFW_KEY_I                  ,26},
		{GLFW_KEY_J                  ,27},
		{GLFW_KEY_K                  ,28},
		{GLFW_KEY_L                  ,29},
		{GLFW_KEY_M                  ,30},
		{GLFW_KEY_N                  ,31},
		{GLFW_KEY_O                  ,32},
		{GLFW_KEY_P                  ,33},
		{GLFW_KEY_Q                  ,34},
		{GLFW_KEY_R                  ,35},
		{GLFW_KEY_S                  ,36},
		{GLFW_KEY_T                  ,37},
		{GLFW_KEY_U                  ,38},
		{GLFW_KEY_V                  ,39},
		{GLFW_KEY_W                  ,40},
		{GLFW_KEY_X                  ,41},
		{GLFW_KEY_Y                  ,42},
		{GLFW_KEY_Z                  ,43},
		{GLFW_KEY_LEFT_BRACKET       ,44},/* [ */
		{GLFW_KEY_BACKSLASH          ,45},/* \ */
		{GLFW_KEY_RIGHT_BRACKET      ,46},/* ] */
		{GLFW_KEY_GRAVE_ACCENT       ,47},/* ` */
		{GLFW_KEY_WORLD_1            ,48},/* non-US #1 */
		{GLFW_KEY_WORLD_2            ,46},/* non-US #2 */
									   
		{GLFW_KEY_ESCAPE             ,47},
		{GLFW_KEY_ENTER              ,48},
		{GLFW_KEY_TAB                ,49},
		{GLFW_KEY_BACKSPACE          ,50},
		{GLFW_KEY_INSERT             ,51},
		{GLFW_KEY_DELETE             ,52},
		{GLFW_KEY_RIGHT              ,53},
		{GLFW_KEY_LEFT               ,54},
		{GLFW_KEY_DOWN               ,55},
		{GLFW_KEY_UP                 ,56},
		{GLFW_KEY_PAGE_UP            ,57},
		{GLFW_KEY_PAGE_DOWN          ,58},
		{GLFW_KEY_HOME               ,59},
		{GLFW_KEY_END                ,60},
		{GLFW_KEY_CAPS_LOCK          ,61},
		{GLFW_KEY_SCROLL_LOCK        ,62},
		{GLFW_KEY_NUM_LOCK           ,63},
		{GLFW_KEY_PRINT_SCREEN       ,64},
		{GLFW_KEY_PAUSE              ,65},
		{GLFW_KEY_F1                 ,66},
		{GLFW_KEY_F2                 ,67},
		{GLFW_KEY_F3                 ,68},
		{GLFW_KEY_F4                 ,69},
		{GLFW_KEY_F5                 ,70},
		{GLFW_KEY_F6                 ,71},
		{GLFW_KEY_F7                 ,72},
		{GLFW_KEY_F8                 ,73},
		{GLFW_KEY_F9                 ,74},
		{GLFW_KEY_F10                ,75},
		{GLFW_KEY_F11                ,76},
		{GLFW_KEY_F12                ,77},
		{GLFW_KEY_F13                ,78},
		{GLFW_KEY_F14                ,79},
		{GLFW_KEY_F15                ,80},
		{GLFW_KEY_F16                ,81},
		{GLFW_KEY_F17                ,82},
		{GLFW_KEY_F18                ,83},
		{GLFW_KEY_F19                ,84},
		{GLFW_KEY_F20                ,85},
		{GLFW_KEY_F21                ,86},
		{GLFW_KEY_F22                ,87},
		{GLFW_KEY_F23                ,88},
		{GLFW_KEY_F24                ,89},
		{GLFW_KEY_F25                ,90},
		{GLFW_KEY_KP_0               ,91},
		{GLFW_KEY_KP_1               ,92},
		{GLFW_KEY_KP_2               ,93},
		{GLFW_KEY_KP_3               ,94},
		{GLFW_KEY_KP_4               ,95},
		{GLFW_KEY_KP_5               ,96},
		{GLFW_KEY_KP_6               ,97},
		{GLFW_KEY_KP_7               ,98},
		{GLFW_KEY_KP_8               ,99},
		{GLFW_KEY_KP_9               ,100},
		{GLFW_KEY_KP_DECIMAL         ,101},
		{GLFW_KEY_KP_DIVIDE          ,102},
		{GLFW_KEY_KP_MULTIPLY        ,103},
		{GLFW_KEY_KP_SUBTRACT        ,104},
		{GLFW_KEY_KP_ADD             ,105},
		{GLFW_KEY_KP_ENTER           ,106},
		{GLFW_KEY_KP_EQUAL           ,107},
		{GLFW_KEY_LEFT_SHIFT         ,108},
		{GLFW_KEY_LEFT_CONTROL       ,109},
		{GLFW_KEY_LEFT_ALT           ,110},
		{GLFW_KEY_LEFT_SUPER         ,111},
		{GLFW_KEY_RIGHT_SHIFT        ,112},
		{GLFW_KEY_RIGHT_CONTROL      ,113},
		{GLFW_KEY_RIGHT_ALT          ,114},
		{GLFW_KEY_RIGHT_SUPER        ,115},
		{GLFW_KEY_MENU               ,116}

	};

	void Input::SetKeyDown(int key)
	{
		keys[key_index_map[key]] = true;
	}

	void Input::SetKeyUp(int key)
	{
		keys[key_index_map[key]] = false;
	}

	bool Input::isKeyDown(int key)
	{
		return keys[key_index_map[key]];
	}

	void Input::setMouseX(int xPosition)
	{
		mouse.x = xPosition;
	}

	void Input::setMouseY(int yPosition)
	{
		mouse.y = yPosition;
	}

	int Input::getMouseX()
	{
		return mouse.x;
	}

	int Input::getMouseY()
	{
		return mouse.y;
	}

	void Input::setLeftMouse(bool down)
	{
		mouse.left = down;
	}

	void Input::setRightMouse(bool down)
	{
		mouse.right = down;
	}

	bool Input::isLeftMouseDown()
	{
		return mouse.left;
	}

	bool Input::isRightMouseDown()
	{
		return mouse.right;
	}

	void Input::setMouseActive(bool active)
	{
		mouse.isActive = active;
	}
	bool Input::isMouseActive()
	{
		return mouse.isActive;
	}

	void Input::setMouseBlocked(bool blocked)
	{
		mouse.isBlocked = blocked;
	}

	bool Input::isMouseBlocked()
	{
		return mouse.isBlocked;
	}

	void Input::SetMouseToCentre(Window* window)
	{
		glfwSetCursorPos(window->GetGLFWWindow(), window->getExtent().width / 2, window->getExtent().height / 2);
		setMouseX(window->getExtent().width / 2);
		setMouseY(window->getExtent().height / 2);
	}

}