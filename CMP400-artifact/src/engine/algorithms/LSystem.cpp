#include "pch.h"
#include "engine/algorithms/LSystem.h"

namespace JEngine
{

	LSystem::LSystem(const std::string& axiom)
		: m_axiom(axiom)
	{
		m_currentSystem = m_axiom;
	}

	LSystem& LSystem::AddRule(const char& predecessor, const std::string& successor)
	{
		m_rules.emplace(predecessor, successor);
		return *this;
	}

	void LSystem::Run(const unsigned int& iterations, bool fromStart)
	{
		if(fromStart)m_currentSystem = m_axiom;

		for (int i = 0; i < iterations; i++) 
		{
			Iterate();
		}
	}

	void LSystem::Iterate()
	{
		std::string nextSystem;
		for (int i = 0; i < m_currentSystem.length(); i++) 
		{
			if (m_rules.count(m_currentSystem[i]) > 0) 
				nextSystem += m_rules.find(m_currentSystem[i])->second;
			else 
				nextSystem += m_currentSystem[i];
		}
		m_currentSystem = nextSystem;
	}

}