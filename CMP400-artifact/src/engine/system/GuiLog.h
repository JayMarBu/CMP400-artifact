#pragma once

namespace JEngine
{
	class GuiLog
	{
		// Members ********************************************************************************
	private:
		ImGuiTextBuffer m_buffer;
		ImGuiTextFilter m_filter;
		ImVector<int> m_lineOffsets;
		bool m_autoScroll;
		// Methods ********************************************************************************
	public:
		GuiLog();
		~GuiLog();

		REMOVE_COPY_CONSTRUCTOR(GuiLog);

		void Clear();

		void Log(const char* message, ...);

		void Draw();
	};
}