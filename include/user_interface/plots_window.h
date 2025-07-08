#pragma once


#include <imgui-platform-kit/user_interface_window.h>

#include "visualization/visualization.h"

namespace dnf_composer
{
	namespace user_interface
	{
		class PlotsWindow : public imgui_kit::UserInterfaceWindow
		{
		private:
			std::shared_ptr<Visualization> visualization;
			std::shared_ptr<Simulation> simulation;
		public:
			PlotsWindow(const std::shared_ptr<Visualization>& visualization);
			void render() override;
		};
	}
}