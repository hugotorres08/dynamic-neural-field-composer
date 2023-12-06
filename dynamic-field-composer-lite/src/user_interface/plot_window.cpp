// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "./user_interface/plot_window.h"

namespace dnf_composer
{
	namespace user_interface
	{

		PlotWindow::PlotWindow(const std::shared_ptr<Simulation>& simulation, bool renderPlotSelector)
			:renderPlotSelector(renderPlotSelector)
		{
			this->visualization = std::make_shared<Visualization>(simulation);
			id = ++current_id;
			plotDimensions = { 0, 100, -30, 40 };
			plotTitle = "Plot window " + std::to_string(id);
		}

		PlotWindow::PlotWindow(const std::shared_ptr<Visualization>& visualization, bool renderPlotSelector)
			:renderPlotSelector(renderPlotSelector)
		{
			this->visualization = visualization;
			id = ++current_id;
			plotDimensions = { 0, 100, -30, 40 };
			plotTitle = "Plot window " + std::to_string(id);
		}

		PlotWindow::PlotWindow(const std::shared_ptr<Simulation>& simulation, PlotDimensions dimensions, bool renderPlotSelector)
			:plotDimensions(dimensions), renderPlotSelector(renderPlotSelector)
		{
			this->visualization = std::make_shared<Visualization>(simulation);
			id = ++current_id;
			plotTitle = "Plot window " + std::to_string(id);
		}

		PlotWindow::PlotWindow(const std::shared_ptr<Visualization>& visualization, PlotDimensions dimensions, bool renderPlotSelector)
			:plotDimensions(dimensions), renderPlotSelector(renderPlotSelector)
		{
			this->visualization = visualization;
			id = ++current_id;
			plotTitle = "Plot window " + std::to_string(id);
		}

		PlotWindow::PlotWindow(const std::shared_ptr<Simulation>& simulation, PlotDimensions dimensions, std::string title, bool renderPlotSelector)
			:plotDimensions(dimensions), plotTitle(std::move(title)), renderPlotSelector(renderPlotSelector)
		{
			this->visualization = std::make_shared<Visualization>(simulation);
			id = ++current_id;
		}

		void PlotWindow::render()
		{
			if (renderPlotSelector)
				renderElementSelector();
			renderPlots();
		}

		void PlotWindow::renderPlots() const
		{
			configure();

			static const std::string plotWindowTitle = plotTitle + " window";

			if (ImGui::Begin(plotWindowTitle.c_str()))
			{
				ImVec2 plotSize = ImGui::GetContentRegionAvail();  // Get available size in the ImGui window
				plotSize.x -= 5.0f; // Subtract some padding
				plotSize.y -= 5.00f; // Subtract some padding

				if (ImPlot::BeginPlot(plotTitle.c_str(), plotSize))
				{
					ImPlot::SetupAxes("Spatial dimension", "Amplitude");
					ImPlot::SetupLegend(ImPlotLocation_NorthEast, ImPlotLegendFlags_Outside);

					const int numOfPlots = visualization->getNumberOfPlots();
					for (int j = 0; j < numOfPlots; j++)
					{
						std::string label = visualization->getPlottingLabel(j);
						std::vector<double> data = *visualization->getPlottingData(j);
						ImPlot::PlotLine(label.c_str(), data.data(), static_cast<int>(data.size()));
					}

				}
				ImPlot::EndPlot();
			}
			ImGui::End();
		}

		void PlotWindow::renderElementSelector() const
		{
			const auto simulation = visualization->getAssociatedSimulationPtr();
			const int numberOfElementsInSimulation = simulation->getNumberOfElements();

			static std::string selectedElementId{};
			static int currentElementIdx = 0;

			const std::string selectorTitle = plotTitle + " plot selector";

			if (ImGui::Begin(selectorTitle.c_str()))
			{
				//ImGui::Columns(2, nullptr, false); // Use 2 columns

				// First column: List box for selecting an element
				ImGui::Text("Select element");
				if (ImGui::BeginListBox("##Element list", { 300.00f, 200.0f }))
				{
					for (int n = 0; n < numberOfElementsInSimulation; n++)
					{
						const auto element = simulation->getElement(n);
						std::string elementId = element->getUniqueName();
						const bool isSelected = (currentElementIdx == n);
						if (ImGui::Selectable(elementId.c_str(), isSelected))
						{
							selectedElementId = elementId;
							currentElementIdx = n;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				// Next column: List box for selecting a component
				//ImGui::NextColumn();
				const char* components[] = { "output", "activation", "input", "kernel" };
				static int currentComponentIdx = 0;

				ImGui::Text("Select component");
				if (ImGui::BeginListBox("##Component list", { 250.0f, 200.0f }))
				{
					for (int n = 0; n < IM_ARRAYSIZE(components); n++)
					{
						const bool isSelected = (currentComponentIdx == n);
						if (ImGui::Selectable(components[n], isSelected))
							currentComponentIdx = n;

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				// Reset columns
				ImGui::Columns(1);

				if (ImGui::Button("Add", { 100.0f, 30.0f }))
				{
					visualization->addPlottingData(selectedElementId, components[currentComponentIdx]);
				}
			}
			ImGui::End();
		}

		void PlotWindow::configure() const
		{
			constexpr static int safeMargin = 1;
			ImPlot::SetNextAxesLimits(plotDimensions.xMin - safeMargin, plotDimensions.xMax + safeMargin,
				plotDimensions.yMin - safeMargin, plotDimensions.yMax + safeMargin);
			ImPlotStyle& style = ImPlot::GetStyle();
			style.LineWeight = 3.0f;
		}
	}
}