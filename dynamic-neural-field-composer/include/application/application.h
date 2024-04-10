#pragma once

#include <imgui-platform-kit/user_interface.h>

#include "exceptions/exception.h"
#include "simulation/simulation.h"
#include "simulation/visualization.h"

namespace dnf_composer
{
	struct ApplicationParameters
	{
		imgui_kit::UserInterfaceParameters userInterfaceParameters;
		bool activateUserInterface;

		ApplicationParameters(bool activateUserInterface = true);
		ApplicationParameters(imgui_kit::UserInterfaceParameters userInterfaceParameters);
	};

	class Application
	{
	private:
		std::shared_ptr<Simulation> simulation;
		ApplicationParameters parameters;
		std::shared_ptr<imgui_kit::win32_directx12::UserInterface> userInterface;
	public:
		Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface = true);
		Application(const Application&) = delete;             
		Application& operator=(const Application&) = delete;  
		Application(Application&&) = delete;                  
		Application& operator=(Application&&) = delete;       

		void init() const;
		void step() const;
		void close() const;

		//void activateUserInterfaceWindow(const std::shared_ptr<imgui_kit::UserInterfaceWindow::UserInterfaceWindow>& window) const;
		//void setActivateUserInterfaceAs(bool activateUI);

		bool getCloseUI() const;
		bool getActivateUserInterface() const;

		~Application() = default;
	};
}

