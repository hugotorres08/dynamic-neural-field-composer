#pragma once


#include <imgui-platform-kit/user_interface_window.h>

#include "visualization/visualization.h"
#include "widgets.h"


namespace dnf_composer
{
	namespace user_interface
	{
		class PlotControlWindow : public imgui_kit::UserInterfaceWindow
		{
		private:
			std::shared_ptr<Visualization> visualization;
			std::shared_ptr<Simulation> simulation;
			bool updatePlotDimensions = false;
		public:
			PlotControlWindow(const std::shared_ptr<Visualization>& visualization);
			virtual void render() override;
		private:
			void renderPlots() const;
			void renderPlot(const Plot& plot) const;
			void renderElementPlotTable() const;
			void renderPlotTable();
			Plot* getSelectedPlot(int id) const;
			int discoverPlotIdIfComponentIsPlotted(const std::pair<std::string, std::vector<double>>& component) const;
		};
	}
}