#include "application/application.h"
#include <iostream> // For std::cin
#include <conio.h>  // For _kbhit() and _getch()

int main()
{
	try
	{
		using namespace dnf_composer;
		
		const auto simulation = std::make_shared<Simulation>("default", 1.0, 0.0, 0.0);
		Application app{ simulation };

		app.init();

		//while (!app.hasGUIBeenClosed())
		for (int i = 0; i < 100; i++)
		{
			app.step();
		}

		app.toggleGUI();

		// Continue calling app.step() while ENTER is not pressed
		std::cout << "Press ENTER to stop..." << std::endl;
		while (true)
		{
			if (_kbhit())
			{
				if (_getch() == '\r') // Check if the key pressed is ENTER
				{
					break;
				}
			}  
			app.step();
		}

		app.toggleGUI();

		// Continue calling app.step() while ENTER is not pressed
		std::cout << "Press ENTER to stop..." << std::endl;
		while (true)
		{
			if (_kbhit())
			{
				if (_getch() == '\r') // Check if the key pressed is ENTER
				{
					break;
				}
			}
			app.step();
		}

		app.close();

	}
	catch (const dnf_composer::Exception& ex)
	{
		const std::string errorMessage = "Exception: " + std::string(ex.what()) + " ErrorCode: " + std::to_string(static_cast<int>(ex.getErrorCode())) + ". ";
		log(dnf_composer::tools::logger::LogLevel::FATAL, errorMessage, dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(dnf_composer::tools::logger::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". ", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(dnf_composer::tools::logger::LogLevel::FATAL, "Unknown exception occurred. ", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
}