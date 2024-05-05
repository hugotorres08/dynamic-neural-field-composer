#pragma once


#include <imgui-platform-kit/user_interface_window.h>

#include "simulation/visualization.h"


namespace dnf_composer
{
	namespace user_interface
	{
		static int current_id = 0;

		struct PlotDimensions
		{
			int xMin, xMax, yMin, yMax;
			double dx;

			PlotDimensions()
				: xMin(0), xMax(0), yMin(0), yMax(0), dx(0.0)
			{}

			PlotDimensions(int xMin, int xMax, int yMin, int yMax, double dx)
				: xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), dx(dx)
			{}
		};

		struct PlotAnnotations
		{
			std::string title, x_label, y_label;

			PlotAnnotations() 
				:title(), x_label(), y_label()
			{}

			PlotAnnotations(std::string title, std::string x_label, std::string y_label)
				:title(std::move(title)), x_label(std::move(x_label)), y_label(std::move(y_label))
			{}
		};

		struct PlotParameters : imgui_kit::UserInterfaceWindowParameters
		{
			int id;
			PlotDimensions dimensions;
			PlotAnnotations annotations;

			PlotParameters()
				: id(0), dimensions(), annotations()
			{}

			PlotParameters(const PlotDimensions& dimensions, PlotAnnotations annotations)
				: id(0), dimensions(dimensions), annotations(std::move(annotations))
			{}

			PlotParameters(const PlotDimensions& dimensions, PlotAnnotations annotations, bool renderElementSelector)
				: id(0), dimensions(dimensions), annotations(std::move(annotations))
			{}
		};

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
			//void renderPlotControl();
			void renderPlot(const PlotParameters& parameters) const;
			void renderElementSelector(const PlotParameters& parameters) const;
			static void configure(const PlotDimensions& dimensions);
		};
	}
}


