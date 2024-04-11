// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "application/application.h"


namespace dnf_composer
{
	ApplicationParameters::ApplicationParameters(bool activateUserInterface)
		:uiActive(activateUserInterface)
	{
		using namespace imgui_kit;
		using namespace imgui_kit::win32_directx12;
		const WindowParameters winParams{ L"Dynamic Neural Field Composer" };
		const FontParameters fontParams{ "../../../resources/fonts/Lexend-Light.ttf", 22 };
		const StyleParameters styleParams{ ImVec4(0.2f, 0.2f, 0.2f, 0.8f) };
		const IconParameters iconParams{ "../../../resources/icons/win_icon.ico" };
		const BackgroundImageParameters bgParams{};
		uiParameters = UserInterfaceParameters{ winParams, fontParams, styleParams, iconParams, bgParams };
	}

	ApplicationParameters::ApplicationParameters(imgui_kit::UserInterfaceParameters userInterfaceParameters)
		:uiParameters(std::move(userInterfaceParameters)), uiActive(true)
	{}

	Application::Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface)
		:simulation(simulation)
	{
		if (activateUserInterface)
			ui = std::make_shared<imgui_kit::win32_directx12::UserInterface>(parameters.uiParameters);
		parameters.uiActive = activateUserInterface;
	}

	Application::Application(const std::shared_ptr<Simulation>& simulation, ApplicationParameters uiParams)
		:simulation(simulation), parameters(std::move(uiParams))
	{
		ui = std::make_shared<imgui_kit::win32_directx12::UserInterface>(parameters.uiParameters);
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
			ui = std::make_shared<imgui_kit::win32_directx12::UserInterface>(parameters.uiParameters);
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
