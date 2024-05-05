#pragma once


#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/simulation.h"


namespace dnf_composer
{
	namespace user_interface
	{
		class PlotControlWindow : public imgui_kit::UserInterfaceWindow
		{
		private:
			std::shared_ptr<Simulation> simulation;
		public:
			PlotControlWindow(const std::shared_ptr<Simulation>& simulation);
			virtual ~PlotControlWindow() = default;

			virtual void render() override;
		};
	}
}