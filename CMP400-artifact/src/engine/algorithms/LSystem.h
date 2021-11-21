#pragma once

namespace JEngine
{
	class LSystem
	{
		// Members ********************************************************************************
	private:
		std::string m_axiom;
		std::string m_currentSystem;
		std::map<char, std::string> m_rules;
		// Methods ********************************************************************************
	private:

	public:
		LSystem() {}
		LSystem(const std::string& axiom);

		inline std::string GetCurrentSystem() const { return m_currentSystem; }

		LSystem& AddRule(const char& predecessor, const std::string& successor);

		void Run(const unsigned int& iterations, bool fromStart = false);

		void Iterate();
	};
}