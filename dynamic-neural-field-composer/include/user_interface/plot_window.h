#pragma once


#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/visualization.h"


namespace dnf_composer
{
	namespace user_interface
	{
		static int current_id = 0;

		

		class PlotWindow : public imgui_kit::UserInterfaceWindow
		{
		private:
			std::vector<PlotParameters> plots;
			std::shared_ptr<Visualization> visualization;
		public:
			PlotWindow(const std::shared_ptr<Visualization>& visualization);
			PlotWindow(const std::shared_ptr<Visualization>& visualization, PlotParameters parameters);

			void addPlottingData(const std::string& elementId, const std::string& componentId) const;

			void render() override;
			~PlotWindow() override = default;
		private:
			void createPlot(PlotParameters& parameters);
			void renderPlot(const PlotParameters& parameters) const;
			void renderElementSelector(const PlotParameters& parameters) const;
			static void configure(const PlotDimensions& dimensions);
		};
	}
}


