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
			renderElementPlotTable();
			renderPlotTable();
			renderPlots();
			updatePlotDimensions = false;
		}

		void PlotControlWindow::renderPlots() const
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

		void PlotControlWindow::renderPlot(const Plot& plot) const
		{
			ImVec2 plotSize = ImGui::GetContentRegionAvail();  // Get available size in the ImGui window
			plotSize.x -= 5.0f; // Subtract some padding
			plotSize.y -= 5.0f; // Subtract some padding
			constexpr static double safeMargin = 0.01;
			ImPlotStyle& style = ImPlot::GetStyle();
			style.LineWeight = 3.0f;
			
			ImPlotFlags flags = ImPlotFlags_Crosshairs;
			const PlotParameters& parameters = plot.getParameters();
			if (parameters.dimensions.autoFit)
			{
				flags |= ImPlotFlags_Equal;
			}
			else
			{
				ImPlot::SetNextAxesLimits(parameters.dimensions.xMin - safeMargin, parameters.dimensions.xMax + safeMargin,
					parameters.dimensions.yMin - safeMargin, parameters.dimensions.yMax + safeMargin);
			}

			const std::string uniquePlotID = parameters.annotations.title + "##" + std::to_string(plot.getUniqueIdentifier());
			if (ImPlot::BeginPlot(uniquePlotID.c_str(), plotSize, flags))
			{
				if (parameters.dimensions.autoFit)
				{
					ImPlot::SetupAxes(parameters.annotations.x_label.c_str(), parameters.annotations.y_label.c_str(),
						ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
				}
				else
				{
					ImPlot::SetupAxes(parameters.annotations.x_label.c_str(), parameters.annotations.y_label.c_str());
					if (updatePlotDimensions)
					{
						const auto imPlot = ImPlot::GetCurrentPlot();
						imPlot->Axes[0].Range.Min = parameters.dimensions.xMin - safeMargin;
						imPlot->Axes[0].Range.Max = parameters.dimensions.xMax + safeMargin;
						imPlot->Axes[3].Range.Min = parameters.dimensions.yMin - safeMargin;
						imPlot->Axes[3].Range.Max = parameters.dimensions.yMax + safeMargin;
					}
				}

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

		void PlotControlWindow::renderElementPlotTable() const
		{
			if (ImGui::Begin("Element Plot Control"))
			{
				widgets::renderHelpMarker("Select the data you want to plot and where to plot it.");

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

						std::unordered_map<std::string, std::vector<double>> components = element->getComponents();
						
						for (const auto& component : components)
						{
							// Start a new row for each component
							ImGui::TableNextRow();

							// Element column
							ImGui::TableSetColumnIndex(0);
							ImGui::TextUnformatted(element->getUniqueName().c_str()); // Display element ID

							// Component column
							ImGui::TableSetColumnIndex(1);
							ImGui::TextUnformatted(component.first.c_str()); // Display component name

							// Plot checkbox column
							ImGui::TableSetColumnIndex(2);

							// Check if the plot is enabled
							int plotId = discoverPlotIdIfComponentIsPlotted(component);

							bool plotEnabled = false;

							if (plotId > -1)
							{
								plotEnabled = true;
							}

							if (ImGui::Checkbox(("##PlotCheckbox" + element->getUniqueName() + component.first).c_str(), &plotEnabled))
							{
								plotEnabled = true;
							}

							// Plot ID column
							ImGui::TableSetColumnIndex(3);
							if (plotEnabled)
							{
								ImGui::InputInt(("##PlotID" + element->getUniqueName() + component.first).c_str(), &plotId, 0, 0, ImGuiInputTextFlags_ReadOnly);
							}
							else
							{
								ImGui::TextDisabled("_"); // Grayed out when plot is unchecked
							}
						}
					}
					ImGui::EndTable();
				}
				ImGui::End();
			}
			
		}

		void PlotControlWindow::renderPlotTable()
		{
			static int selectedPlotId = -1;

			if (ImGui::Begin("Plot Property Editor"))
			{
				widgets::renderHelpMarker("Select a plot from the left panel to edit its properties.");

				// Split left and right panes with ImGui::BeginChild for separate regions
				ImGui::BeginChild("PlotList", ImVec2(150, 0), true); // Left side for plot list

				// Display each plot ID in a list format
				const std::vector<Plot> plots = visualization->getPlots();
				for (const auto& plot : plots)
				{
					const int plotId = plot.getUniqueIdentifier();

					if (ImGui::Selectable(("Plot #" + std::to_string(plotId)).c_str(), selectedPlotId == plotId))
					{
						selectedPlotId = plotId;
					}
				}
				ImGui::EndChild();

				ImGui::SameLine();

				// Right side: Display properties of the selected plot
				ImGui::BeginChild("PlotProperties", ImVec2(0, 0), true);

				Plot* selectedPlot = getSelectedPlot(selectedPlotId);
				if (selectedPlot)
				{
					bool parametersChanged = false;

					ImGui::Text("Plot ID: %d", selectedPlot->getUniqueIdentifier());
					ImGui::Separator();

					const PlotParameters oldParams = selectedPlot->getParameters();
					PlotParameters newParams = oldParams;

					// Editable plot properties
					char title[128];
					strncpy_s(title, oldParams.annotations.title.c_str(), sizeof(title));
					if (ImGui::InputText("Title", title, IM_ARRAYSIZE(title)))
					{
						newParams.annotations.title = title;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					char xLabel[128];
					strncpy_s(xLabel, oldParams.annotations.x_label.c_str(), sizeof(xLabel));
					if (ImGui::InputText("X Label", xLabel, IM_ARRAYSIZE(xLabel)))
					{
						newParams.annotations.x_label = xLabel;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					char yLabel[128];
					strncpy_s(yLabel, oldParams.annotations.y_label.c_str(), sizeof(yLabel));
					if (ImGui::InputText("Y Label", yLabel, IM_ARRAYSIZE(yLabel)))
					{
						newParams.annotations.y_label = yLabel;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					double xMin = oldParams.dimensions.xMin;
					if (ImGui::InputDouble("X Min", &xMin, 0.1, 1.0)) // Set step and step_fast
					{
						newParams.dimensions.xMin = xMin;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					double xMax = oldParams.dimensions.xMax;
					if (ImGui::InputDouble("X Max", &xMax, 0.1, 1.0))
					{
						newParams.dimensions.xMax = xMax;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					double yMin = oldParams.dimensions.yMin;
					if (ImGui::InputDouble("Y Min", &yMin, 0.1, 1.0))
					{
						newParams.dimensions.yMin = yMin;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					double yMax = oldParams.dimensions.yMax;
					if (ImGui::InputDouble("Y Max", &yMax, 0.1, 1.0))
					{
						newParams.dimensions.yMax = yMax;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					double dx = oldParams.dimensions.dx;
					if (ImGui::InputDouble("dx", &dx, 0.01, 0.1))
					{
						newParams.dimensions.dx = dx;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					bool autoFit = oldParams.dimensions.autoFit;
					if (ImGui::Checkbox("Auto Fit", &autoFit))
					{
						newParams.dimensions.autoFit = autoFit;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					if (parametersChanged)
					{
						if (newParams != oldParams)
						{
							selectedPlot->setParameters(newParams);
							updatePlotDimensions = true;
						}
						
					}
				}

				ImGui::EndChild();
			}
			ImGui::End();
		}

		Plot* PlotControlWindow::getSelectedPlot(int id) const
		{
			for (auto& plot : visualization->getPlots())
			{
				if (plot.getUniqueIdentifier() == id)
				{
					return &plot;
				}
			}
			return nullptr;
		}

		int PlotControlWindow::discoverPlotIdIfComponentIsPlotted(const std::pair<std::string, std::vector<double>>& component) const
		{
			for (const auto& plot : visualization->getPlots())
			{
				for (const auto& data : plot.getData())
				{
					if (!data->empty())
					{
						if (component.second == *data)
						{
							return plot.getUniqueIdentifier();
						}
					}
				}
			}
			return -1;
		}


	}
}
