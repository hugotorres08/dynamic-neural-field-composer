// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This .exe creates a simulation object programatically defining its elements.
// This .exe shows you how to dynamic neural fields in three different ways.

// 1. Manual - You define element parameters, you create the elements,
// you add them to the simulation, and you create the interactions.
//
// 2. Semi-automatic - You define element parameters, you create the elements
// and they get added automatically to the simulation, and you create the interactions.
//
// 3. Automatic - You define element parameters, you create a dynamic neural field object,
// elements get created, added to the simulation, and the interactions between them are automatically created.

int main(int argc, char* argv[])
{
	try
	{
		const auto simulation = std::make_shared<dnf_composer::Simulation>("Element factory example", 5, 0, 0);
		constexpr bool activateUserInterface = true;
		const dnf_composer::Application app{ simulation, activateUserInterface };

		dnf_composer::element::ElementFactory factory;

		app.init();

		bool userRequestClose = false;
		while (!userRequestClose)
		{
			app.step();
			userRequestClose = app.getCloseUI();
		}
		app.close();
		return 0;
	}
	catch (const dnf_composer::Exception& ex)
	{
		const std::string errorMessage = "Exception: " + std::string(ex.what()) + " ErrorCode: " + std::to_string(static_cast<int>(ex.getErrorCode())) + ". \n";
		log(dnf_composer::LogLevel::FATAL, errorMessage, dnf_composer::LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(dnf_composer::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". \n", dnf_composer::LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(dnf_composer::LogLevel::FATAL, "Unknown exception occurred. \n", dnf_composer::LogOutputMode::CONSOLE);
		return 1;
	}
}