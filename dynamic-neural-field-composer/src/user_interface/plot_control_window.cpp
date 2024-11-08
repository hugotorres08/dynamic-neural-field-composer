#include "user_interface/plot_control_window.h"

namespace dnf_composer
{
	namespace user_interface
	{
		PlotControlWindow::PlotControlWindow(const std::shared_ptr<Visualization>& visualization)
			:visualization(visualization), simulation(visualization->getSimulation())
		{}

		void PlotControlWindow::render()
		{
			/*if (ImGui::Begin("Plot Control"))
			{
				renderElementPlotTable();
			}
			if (ImGui::Begin("Plot table"))
			{
				renderPlotTable();
			}*/
			renderPlots();
		}


		void PlotControlWindow::renderPlots()
		{
			const std::vector<Plot> plots = visualization->getPlots();
			for (const auto& plot : plots)
			{
				const std::string plotWindowTitle = "Plot #" + std::to_string(plot.getUniqueIdentifier());
				if (ImGui::Begin(plotWindowTitle.c_str()))//, &open, ImGuiWindowFlags_NoCollapse))
				{
					renderPlot(plot);
				}
				ImGui::End();
			}
		}

		void PlotControlWindow::renderPlot(const Plot& plot)
		{

			ImVec2 plotSize = ImGui::GetContentRegionAvail();  // Get available size in the ImGui window
			plotSize.x -= 5.0f; // Subtract some padding
			plotSize.y -= 5.0f; // Subtract some padding

			static constexpr ImPlotFlags flags = ImPlotFlags_Crosshairs | ImPlotFlags_Equal;
			const PlotParameters& parameters = plot.getParameters();
			configurePlot(parameters.dimensions);

			const std::string uniquePlotID = parameters.annotations.title + "##" + std::to_string(plot.getUniqueIdentifier());
			if (ImPlot::BeginPlot(uniquePlotID.c_str(), plotSize, flags))
			{
				ImPlot::SetupAxes(parameters.annotations.x_label.c_str(), parameters.annotations.y_label.c_str(),
					ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
				ImPlot::SetupLegend(ImPlotLocation_South, ImPlotLegendFlags_Horizontal);

				const std::vector<std::vector<double>*> allData = plot.getData();
				for (size_t j = 0; j < allData.size(); j++)
				{
					std::string label = parameters.annotations.legends[j];
					std::vector<double> data = *allData[j];

					// Shift x-values by 1 unit and scale
					std::vector<double> shiftedXValues(data.size());
					for (int i = 0; i < data.size(); ++i) 
						shiftedXValues[i] = (i + 1) * parameters.dimensions.dx;

					ImPlot::PlotLine(label.c_str(), shiftedXValues.data(), data.data(), static_cast<int>(data.size()));
				}

			}
			ImPlot::EndPlot();
		}

		void PlotControlWindow::configurePlot(const PlotDimensions& dimensions)
		{
			constexpr static int safeMargin = 1;
			ImPlot::SetNextAxesLimits(dimensions.xMin - safeMargin, dimensions.xMax + safeMargin,
			dimensions.yMin - safeMargin, dimensions.yMax + safeMargin);
			ImPlotStyle& style = ImPlot::GetStyle();
			style.LineWeight = 3.0f;
		}


		void PlotControlWindow::renderElementPlotTable() const
		{
			if (ImGui::BeginTable("PlotControlTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				// Set column headers
				ImGui::TableSetupColumn("Elements");
				ImGui::TableSetupColumn("Components");
				ImGui::TableSetupColumn("Plot");
				ImGui::TableSetupColumn("Plot ID");
				ImGui::TableHeadersRow();

				// Example loop for each element
				for (const auto& element : simulation->getElements())
				{
					// For each element, iterate over its components
					//auto components = element->getComponentNames(); // Assuming this method gives component names for the element
					std::vector<std::string> components = element->getComponentList();
					for (const auto& component : components)
					{
						// Start a new row for each component
						ImGui::TableNextRow();

						// Element column
						ImGui::TableSetColumnIndex(0);
						ImGui::TextUnformatted(element->getUniqueName().c_str()); // Display element ID

						// Component column
						ImGui::TableSetColumnIndex(1);
						ImGui::TextUnformatted(component.c_str()); // Display component name

						// Plot checkbox column
						ImGui::TableSetColumnIndex(2);
						//bool plotEnabled = element->isPlotEnabled(component); // Example method to get current plot state
						bool plotEnabled = true;
						if (ImGui::Checkbox(("##PlotCheckbox" + element->getUniqueName() + component).c_str(), &plotEnabled))
						{
							// Toggle plot state when checkbox changes
							//element->setPlotEnabled(component, plotEnabled);
						}

						// Plot ID column
						ImGui::TableSetColumnIndex(3);
						if (plotEnabled)
						{
							//int plotId = element->getPlotId(component); // Example method to get plot ID
							int plotId = 0;
							ImGui::InputInt(("##PlotID" + element->getUniqueName() + component).c_str(), &plotId, 0, 0, ImGuiInputTextFlags_ReadOnly);
						}
						else
						{
							ImGui::TextDisabled("_"); // Grayed out when plot is unchecked
						}
					}
				}
				ImGui::EndTable();
			}
		}

		void PlotControlWindow::renderPlotTable()
		{
			// Split left and right panes with ImGui::BeginChild for separate regions
			ImGui::BeginChild("PlotList", ImVec2(150, 0), true); // Left side for plot list

			// Display each plot ID in a list format
			//for (size_t i = 0; i < visualization->plots.size(); ++i)
			std::vector<Plot> plots = visualization->getPlots();
			for (size_t i = 0; i < plots.size(); ++i)
			{
				Plot& plot = plots[i]; // Assuming plots is a public or accessible member
				int plotId = plot.getUniqueIdentifier(); // Assuming getId() returns plot ID

				if (ImGui::Selectable(("Plot " + std::to_string(plotId)).c_str(), selectedPlotId == plotId))
				{
					selectedPlotId = plotId;
				}
			}
			ImGui::EndChild();

			ImGui::SameLine();

			// Right side: Display properties of the selected plot
			ImGui::BeginChild("PlotProperties", ImVec2(0, 0), true);

			Plot* selectedPlot = getSelectedPlot();
			if (selectedPlot)
			{
				ImGui::Text("Plot ID: %d", selectedPlot->getUniqueIdentifier());
				ImGui::Separator();

				// Assuming PlotParameters is a struct that holds plot properties (e.g., title, color, etc.)
				PlotParameters params = selectedPlot->getParameters();

				// Editable plot properties
				char title[128];
				strncpy(title, params.annotations.title.c_str(), sizeof(title));
				if (ImGui::InputText("Title", title, IM_ARRAYSIZE(title)))
				{
					params.annotations.title = title; // Update plot title
				}

				//ImGui::ColorEdit3("Plot Color", (float*)&params.color); // Assuming color is ImVec3

				//ImGui::DragFloat("Line Thickness", &params.lineThickness, 0.1f, 0.1f, 10.0f);
				//ImGui::DragFloat("Plot Scale", &params.scale, 0.1f, 0.1f, 10.0f);

				// Add other parameter widgets as needed (e.g., markers, axis limits, etc.)
			}
			else
			{
				ImGui::Text("Select a plot from the left panel to edit its properties.");
			}

			ImGui::EndChild();
		}

		// Helper function to get the currently selected plot
		Plot* PlotControlWindow::getSelectedPlot() const
		{
			for (auto& plot : visualization->getPlots())
			{
				if (plot.getUniqueIdentifier() == selectedPlotId)
				{
					return &plot;
				}
			}
			return nullptr;
		}

	}
}
