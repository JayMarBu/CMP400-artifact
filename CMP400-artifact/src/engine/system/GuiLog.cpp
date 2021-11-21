#include "pch.h"
#include "engine/system/GuiLog.h"

namespace JEngine
{
	GuiLog::GuiLog()
	{
		m_autoScroll = true;
		Clear();
	}

	GuiLog::~GuiLog()
	{
		Clear();
	}

	void GuiLog::Clear()
	{
		m_buffer.clear();
		m_lineOffsets.clear();
		m_lineOffsets.push_back(0);
	}

	void GuiLog::Log(const char* message, ...)
	{
		int oldSize = m_buffer.size();

		va_list args;
		va_start(args, message);

		m_buffer.appendfv(message, args);
	}

	void GuiLog::Draw()
	{

	}

}