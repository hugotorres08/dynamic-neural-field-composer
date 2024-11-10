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
			}*/
			//ImGui::ShowDemoWindow();
			renderPlotTable();
			renderPlots();
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

		void PlotControlWindow::renderPlot(const Plot& plot)
		{
			ImVec2 plotSize = ImGui::GetContentRegionAvail();  // Get available size in the ImGui window
			plotSize.x -= 5.0f; // Subtract some padding
			plotSize.y -= 5.0f; // Subtract some padding
			constexpr static double safeMargin = 0.01;
			ImPlotStyle& style = ImPlot::GetStyle();
			style.LineWeight = 3.0f;

			
			ImPlotFlags flags = ImPlotFlags_Crosshairs;
			const PlotParameters& parameters = plot.getParameters();
			if (parameters.dimensions.areUndefined())
			{
				flags |= ImPlotFlags_Equal;
			}
			//else
			//{
			//	ImPlot::SetNextAxesLimits(parameters.dimensions.xMin - safeMargin, parameters.dimensions.xMax + safeMargin,
			//		parameters.dimensions.yMin - safeMargin, parameters.dimensions.yMax + safeMargin);
			//	auto aux = ImPlot::GetCurrentPlot()->Axes;
			//}

			const std::string uniquePlotID = parameters.annotations.title + "##" + std::to_string(plot.getUniqueIdentifier());
			if (ImPlot::BeginPlot(uniquePlotID.c_str(), plotSize, flags))
			{
				if (parameters.dimensions.areUndefined())
				{
					ImPlot::SetupAxes(parameters.annotations.x_label.c_str(), parameters.annotations.y_label.c_str(),
						ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
				}
				else
				{
					ImPlot::SetupAxes(parameters.annotations.x_label.c_str(), parameters.annotations.y_label.c_str());
					auto plot_ = ImPlot::GetCurrentPlot();
					auto aux = plot_->Axes;
					plot_->Axes[0].Range.Min = parameters.dimensions.xMin - safeMargin;
					plot_->Axes[0].Range.Max = parameters.dimensions.xMax + safeMargin;
					plot_->Axes[3].Range.Min = parameters.dimensions.yMin - safeMargin;
					plot_->Axes[3].Range.Max = parameters.dimensions.yMax + safeMargin;
					//plot_->Axes->Range.Max = parameters.dimensions.yMax + safeMargin;
					//plot_->XAxis = parameters.dimensions.xMin - safeMargin;
					//plot_->XAxis.Max = parameters.dimensions.xMax + safeMargin;
					//plot_->YAxis.Min = parameters.dimensions.yMin - safeMargin;
					//plot_->YAxis.Max = parameters.dimensions.yMax + safeMargin;
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
			static int selectedPlotId = -1;

			if (ImGui::Begin("Plot Property Editor"))
			{
				// Help marker [make this a static thing with an string arg.]
				ImGui::TextDisabled("(?)");
				if (ImGui::BeginItemTooltip())
				{
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted("Select a plot from the left panel to edit its properties.");
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}

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
					int xMin = oldParams.dimensions.xMin;
					if (ImGui::InputInt("X Min", &xMin))
					{
						newParams.dimensions.xMin = xMin;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					int xMax = oldParams.dimensions.xMax;
					if (ImGui::InputInt("X Max", &xMax))
					{
						newParams.dimensions.xMax = xMax;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					int yMin = oldParams.dimensions.yMin;
					if (ImGui::InputInt("Y Min", &yMin))
					{
						newParams.dimensions.yMin = yMin;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					int yMax = oldParams.dimensions.yMax;
					if (ImGui::InputInt("Y Max", &yMax))
					{
						newParams.dimensions.yMax = yMax;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					double dx = oldParams.dimensions.dx;
					if (ImGui::InputDouble("dx", &dx))
					{
						newParams.dimensions.dx = dx;
						if (ImGui::IsItemDeactivatedAfterEdit())
							parametersChanged = true;
					}
					if (parametersChanged)
						if (newParams != oldParams)
							selectedPlot->setParameters(newParams);
					
					//ImGui::ColorEdit3("Plot Color", (float*)&params.color); // Assuming color is ImVec3

					//ImGui::DragFloat("Line Thickness", &params.lineThickness, 0.1f, 0.1f, 10.0f);
					//ImGui::DragFloat("Plot Scale", &params.scale, 0.1f, 0.1f, 10.0f);

					// Add other parameter widgets as needed (e.g., markers, axis limits, etc.)
				}

				ImGui::EndChild();
			}
			ImGui::End();
		}

		// Helper function to get the currently selected plot
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

	}
}
