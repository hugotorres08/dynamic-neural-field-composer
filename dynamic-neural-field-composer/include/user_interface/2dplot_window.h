#pragma once


#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/simulation.h"
#include "elements/gauss_field_coupling.h"
#include "tools/math.h"


namespace dnf_composer
{
	namespace user_interface
	{
		class PlotWindow2D : public imgui_kit::UserInterfaceWindow
		{
		private:
			std::shared_ptr<Simulation> simulation;
		public:
			PlotWindow2D(const std::shared_ptr<Simulation>& simulation);
			void render() override;
			~PlotWindow2D() override = default;
		private:
			void renderPlottableElements() ;
		};
	}
}


