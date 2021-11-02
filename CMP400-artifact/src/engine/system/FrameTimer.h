#pragma once
#include <windows.h>

namespace JEngine
{
	class FrameTimer
	{
		// Members ********************************************************************************
	private:
		INT64 frequency;
		float ticksPerS;
		INT64 startTime;
		float frameTime;
		float fps;
		float frames;
		float elapsedTime;
	
		// Methods ********************************************************************************
	public:
		FrameTimer();
		~FrameTimer();

		void frame();	///< Update delta time, called every frame

		float getTime();	///< Get delta time
		float getFPS();		///< Get FPS (for display)

	};

}