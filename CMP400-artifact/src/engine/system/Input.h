#pragma once

namespace JEngine
{
	class Window;

	class Input
	{
		/// Mouse stuct, store position, button click and active status
		struct Mouse
		{
			int x, y;
			bool left, right, isActive;
		};

	public:

		void SetKeyDown(int key);	///< Sets key down value for specified key
		void SetKeyUp(int key);		///< Sets key up value for specified key

		bool isKeyDown(int key);		///< Check if specified key is down
		void setMouseX(int xPosition);	///< Set mouse X-axis position
		void setMouseY(int yPosition);	///< Set mouse y-axis position
		int getMouseX();				///< Get mouse x-axis position
		int getMouseY();				///< Get mouse y-axis position
		void setLeftMouse(bool down);	///< Set left mouse click
		void setRightMouse(bool down);	///< Set right mouse click
		bool isLeftMouseDown();			///< Check left mouse click
		bool isRightMouseDown();		///< Check right mouse click
		void setMouseActive(bool active);	///< Set mouse in/active
		bool isMouseActive();			///< Check if mouse is in/active

		void SetMouseToCentre(Window* window);

		static std::unordered_map<int, int> key_index_map;

	private:
		bool keys[256];		///< Array for storing key states
		Mouse mouse;		///< Mouse state variable

	};
}