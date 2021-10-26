#include "pch.h"
#include "engine/App.h"

int main() 
{
	JEngine::App app{};

	try
	{
		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}