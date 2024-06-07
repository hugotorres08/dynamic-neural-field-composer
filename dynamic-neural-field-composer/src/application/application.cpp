// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "application/application.h"


namespace dnf_composer
{
	ApplicationParameters::ApplicationParameters(bool activateUserInterface)
		:uiActive(activateUserInterface)
	{
		using namespace imgui_kit;
		const WindowParameters winParams{ "Dynamic Neural Field Composer" };
		const FontParameters fontParams{ std::string(PROJECT_DIR) + "/resources/fonts/Lexend-Light.ttf", 15 };
		const StyleParameters styleParams{ ImVec4(0.3f, 0.3f, 0.3f, 0.6f), colours::White };
		const IconParameters iconParams{ std::string(PROJECT_DIR) + "/resources/icons/icon.ico" };
		const BackgroundImageParameters bgParams{ std::string(PROJECT_DIR) + "/resources/images/background.png", ImageFitType::ZOOM_TO_FIT };
		uiParameters = UserInterfaceParameters{ winParams, fontParams, styleParams, iconParams, bgParams};
	}

	ApplicationParameters::ApplicationParameters(imgui_kit::UserInterfaceParameters userInterfaceParameters)
		:uiParameters(std::move(userInterfaceParameters)), uiActive(true)
	{
	}

	Application::Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface)
		:simulation(simulation)
	{
		if (activateUserInterface)
			ui = std::make_shared<imgui_kit::UserInterface>(parameters.uiParameters);
		parameters.uiActive = activateUserInterface;
	}

	Application::Application(const std::shared_ptr<Simulation>& simulation, ApplicationParameters uiParams)
		:simulation(simulation), parameters(std::move(uiParams))
	{
		ui = std::make_shared<imgui_kit::UserInterface>(parameters.uiParameters);
	}

	void Application::init() const
	{
		simulation->init();
		if (parameters.uiActive)
			ui->initialize();
	}

	void Application::step() const
	{
		simulation->step();
		if (parameters.uiActive)
			ui->render();
	}

	void Application::close() const
	{
		simulation->close();
		if (parameters.uiActive)
			ui->shutdown();
	}

	void Application::setActivateUserInterfaceAs(bool activateUI)
	{
		parameters.uiActive = activateUI;
		if (parameters.uiActive)
			ui = std::make_shared<imgui_kit::UserInterface>(parameters.uiParameters);
	}

	bool Application::hasUIBeenClosed() const
	{
		if (parameters.uiActive)
			return ui->isShutdownRequested();
		return false;
	}

	bool Application::isUIActive() const
	{
		return parameters.uiActive;
	}
}
