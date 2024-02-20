// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "application/application.h"

#include "user_interface/plot_window.h"
#include "user_interface/simulation_window.h"


namespace dnf_composer
{
	Application::Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface)
		:simulation(simulation), activateUserInterface(activateUserInterface)
	{
		if (activateUserInterface)
			userInterface = std::make_shared<user_interface::UserInterface>();
	}


	void Application::init() const
	{
		simulation->init();
		if (activateUserInterface)
			userInterface->init();
	}

	void Application::step() const
	{
		simulation->step();
		if (activateUserInterface)
			userInterface->step();
	}

	void Application::close() const
	{
		simulation->close();
		if (activateUserInterface)
			userInterface->close();
	}

	void Application::activateUserInterfaceWindow(user_interface::UserInterfaceWindowType winType, const user_interface::UserInterfaceWindowParameters& winParams) const
	{

		if(activateUserInterface)
		{
			switch(winType)
			{
			case user_interface::ELEMENT_WINDOW:
				userInterface->activateWindow(std::make_shared<user_interface::ElementWindow>(simulation));
				break;
			case user_interface::MONITORING_WINDOW:
				userInterface->activateWindow(std::make_shared<user_interface::CentroidMonitoringWindow>(simulation));
				break;
			case user_interface::SIMULATION_WINDOW:
				userInterface->activateWindow(std::make_shared<user_interface::SimulationWindow>(simulation));
				break;
			case user_interface::LOG_WINDOW:
				userInterface->activateWindow(std::make_shared<user_interface::LoggerWindow>());
				break;
			case user_interface::PLOT_WINDOW:
				const auto plotParameters = dynamic_cast<const user_interface::PlotParameters*>(&winParams);
				userInterface->activateWindow(std::make_shared<user_interface::PlotWindow>(simulation, *plotParameters, true));
				break;
			}
		}
	}

	void Application::setActivateUserInterfaceAs(bool activateUI)
	{
		activateUserInterface = activateUI;
		if (activateUserInterface)
			userInterface = std::make_shared<user_interface::UserInterface>();
	}

	/*void Application::activateUserInterfaceWindow(const std::shared_ptr<user_interface::UserInterfaceWindow>& window) const
	{
		if (activateUserInterface)
			userInterface->activateWindow(window);
	}*/

	bool Application::getCloseUI() const
	{
		if (activateUserInterface)
			return userInterface->getCloseUI();
		return false;
	}

	bool Application::getActivateUserInterface() const
	{
		return activateUserInterface;
	}
	
}
