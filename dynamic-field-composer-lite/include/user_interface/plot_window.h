#pragma once


#include "user_interface_window.h"
#include "simulation/visualization.h"


namespace dnf_composer
{
	namespace user_interface
	{
		static int current_id = 0;

		struct PlotDimensions
		{
			int xMin, xMax, yMin, yMax;
		};


		class PlotWindow : public UserInterfaceWindow
		{
		private:
			std::shared_ptr<Visualization> visualization;
			int id;
			PlotDimensions plotDimensions;
			std::string plotTitle;
			bool renderPlotSelector;
		public:
			PlotWindow(const std::shared_ptr<Simulation>& simulation, bool renderPlotSelector = true);
			PlotWindow(const std::shared_ptr<Simulation>& simulation, PlotDimensions dimensions, bool renderPlotSelector = true);
			PlotWindow(const std::shared_ptr<Visualization>& visualization, bool renderPlotSelector = true);
			PlotWindow(const std::shared_ptr<Visualization>& visualization, PlotDimensions dimensions, bool renderPlotSelector = true);
			PlotWindow(const std::shared_ptr<Simulation>& simulation, PlotDimensions dimensions, std::string title, bool renderPlotSelector = true);

			void render() override;
			~PlotWindow() override = default;
		private:
			void renderPlots() const;
			void renderElementSelector() const;
			void configure() const;
		};
	}
}


