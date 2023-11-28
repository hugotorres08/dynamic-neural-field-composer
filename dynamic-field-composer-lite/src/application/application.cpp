// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "application/application.h"

Application::Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface)
	:simulation(simulation), activateUserInterface(activateUserInterface)
{
	if(activateUserInterface)
		userInterface = std::make_shared<UserInterface>(this->simulation, this->visualizations);
}

void Application::addVisualization()
{
	visualizations.push_back(std::make_shared<Visualization>(simulation));
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

void Application::setActivateUserInterfaceAs(bool activateUI)
{
	activateUserInterface = activateUI;
}

void Application::activateUserInterfaceWindow(const std::shared_ptr<UserInterfaceWindow>& window) const
{
	if (activateUserInterface)
		userInterface->activateWindow(window);
}

bool Application::getCloseUI() const
{
	if (activateUserInterface)
		return userInterface->getCloseUI();
	return false;
}

