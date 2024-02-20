// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application without a GUI.

int main(int argc, char* argv[])
{
    // After defining the simulation, we can create the application.
    const auto simulation = std::make_shared<dnf_composer::Simulation>("run sim headless example", 5, 0, 0);
    // You can run the application without the user interface by setting the second parameter to false.
    constexpr bool activateUserInterface = false;
    const dnf_composer::Application app{ simulation, activateUserInterface };

	try
	{
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
		log(tools::logger::LogLevel::FATAL, errorMessage, tools::logger::LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(tools::logger::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". \n", tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(tools::logger::LogLevel::FATAL, "Unknown exception occurred. \n", tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
}