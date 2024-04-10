// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "application/application.h"


namespace dnf_composer
{
	ApplicationParameters::ApplicationParameters(bool activateUserInterface)
		:activateUserInterface(activateUserInterface)
	{
		// user might want to activate user interface later
		// so we set default parameters
		using namespace imgui_kit;
		using namespace imgui_kit::win32_directx12;
		const WindowParameters winParams{ L"Dynamic Neural Field Composer", 1280, 720 };
		const FontParameters fontParams{ "../../../resources/fonts/Lexend-Light.ttf", 22 };
		const StyleParameters styleParams{ ImVec4(0.2f, 0.2f, 0.2f, 0.8f) };
		const IconParameters iconParams{ "../../../resources/icons/win_icon.ico" };
		const BackgroundImageParameters bgParams{};
		userInterfaceParameters = UserInterfaceParameters{ winParams, fontParams, styleParams, iconParams, bgParams };
		this->activateUserInterface = activateUserInterface;
	}

	ApplicationParameters::ApplicationParameters(imgui_kit::UserInterfaceParameters userInterfaceParameters)
		:userInterfaceParameters(std::move(userInterfaceParameters)), activateUserInterface(true)
	{}


	Application::Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface)
		:simulation(simulation)
	{
		if (activateUserInterface)
			userInterface = std::make_shared<imgui_kit::win32_directx12::UserInterface>(parameters.userInterfaceParameters);
		parameters.activateUserInterface = activateUserInterface;
	}

	void Application::init() const
	{
		simulation->init();
		if (parameters.activateUserInterface)
			userInterface->initialize();
	}

	void Application::step() const
	{
		simulation->step();
		if (parameters.activateUserInterface)
			userInterface->render();
	}

	void Application::close() const
	{
		simulation->close();
		if (parameters.activateUserInterface)
			userInterface->shutdown();
	}

	/*void Application::activateUserInterfaceWindow(user_interface::UserInterfaceWindowType winType, const user_interface::UserInterfaceWindowParameters& winParams) const
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
				userInterface->activateWindow(std::make_shared<user_interface::PlotWindow>(simulation, *plotParameters));
				break;
			}
		}
	}*/

	/*void Application::setActivateUserInterfaceAs(bool activateUI)
	{
		activateUserInterface = activateUI;
		if (activateUserInterface)
			userInterface = std::make_shared<imgui_kit::win32_directx12::UserInterface();
	}*/

	//void Application::activateUserInterfaceWindow(const std::shared_ptr<user_interface::UserInterfaceWindow>& window) const
	//{
	//	if (activateUserInterface)
	//		userInterface->activateWindow(window);
	//}

	bool Application::getCloseUI() const
	{
		if (parameters.activateUserInterface)
			return userInterface->isShutdownRequested();
		return false;
	}

	bool Application::getActivateUserInterface() const
	{
		return parameters.activateUserInterface;
	}
	
}
