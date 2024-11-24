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
			//renderPlotTable();
			renderPlotWindows();
			updatePlotDimensions = false;
		}

		void PlotControlWindow::renderPlotWindows() const
		{
			visualization->render();
			/*for (const auto& plot : visualization->getPlots())
			{
				const int plotID = plot->getUniqueIdentifier();
				const std::string plotWindowTitle = "Plot #" + std::to_string(plotID);
				bool open = true;

				if (ImGui::Begin(plotWindowTitle.c_str(), &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
				{
					plot->render();
				}
				ImGui::End();

				if (!open)
					visualization->removePlot(plotID);
			}*/
		}

		void PlotControlWindow::renderElementPlotTable() const
		{
			if (ImGui::Begin("Element Plot Control"))
			{
				widgets::renderHelpMarker("Select the data you want to plot and where to plot it.");

				// Add a new plot button
				ImGui::Text("Add a new plot:"); ImGui::SameLine();
				static PlotType selectedPlotType = PlotType::LINE_PLOT;
				ImGui::Combo("Plot type", reinterpret_cast<int*>(&selectedPlotType), "Line plot\0Heatmap\0\0");
				ImGui::SameLine();
				if (ImGui::Button("Add"))
				{
					visualization->plot(selectedPlotType);
				}

				if (ImGui::BeginTable("PlotControlTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
				{
					// Set column headers
					ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 50);
					ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100);
					ImGui::TableSetupColumn("Data", ImGuiTableColumnFlags_WidthFixed, 300);
					ImGui::TableSetupColumn("Change plotted data", ImGuiTableColumnFlags_WidthFixed, 300);
					ImGui::TableHeadersRow();

					const auto& plots = visualization->getPlots();
					for (const auto& plot : plots)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::TextUnformatted(std::to_string(plot.first->getUniqueIdentifier()).c_str());
						ImGui::TableSetColumnIndex(1);
						ImGui::TextUnformatted(PlotTypeToString.at(plot.first->getType()).c_str());

						const std::string listbox_label = "##" + std::to_string(plot.first->getUniqueIdentifier());
						ImGui::TableSetColumnIndex(2);
						if (ImGui::BeginListBox(listbox_label.c_str(), ImVec2(300, 25)))
						{
							for (const auto& data : plot.second)
							{
								ImGui::TextUnformatted((data.first + " " + data.second).c_str());
							}
						}
						ImGui::EndListBox();

						// Add data button and popup
						ImGui::TableSetColumnIndex(3);
						const std::string popup_id = "AddDataPopup_" + std::to_string(plot.first->getUniqueIdentifier());
						if (ImGui::Button(("Add data##" + std::to_string(plot.first->getUniqueIdentifier())).c_str()))
						{
							ImGui::OpenPopup(popup_id.c_str());
						}

						if (ImGui::BeginPopup(popup_id.c_str()))
						{
							ImGui::Text("Select data to add:");
							ImGui::Separator();
							for (const auto& element : simulation->getElements())
							{
								const auto* components = element->getComponents();
								for (const auto& [name, value] : *components)
								{
									const std::string item_label = element->getUniqueName() + " " + name;
									if (ImGui::Selectable(item_label.c_str()))
									{
										visualization->plot(plot.first->getUniqueIdentifier(), element->getUniqueName(), name);
										ImGui::CloseCurrentPopup();
									}
								}
							}
							ImGui::EndPopup();
						}

						ImGui::SameLine();
						// Remove data button and popup
						const std::string remove_popup_id = "RemoveDataPopup_" + std::to_string(plot.first->getUniqueIdentifier());
						if (ImGui::Button(("Remove data##" + std::to_string(plot.first->getUniqueIdentifier())).c_str()))
						{
							ImGui::OpenPopup(remove_popup_id.c_str());
						}

						if (ImGui::BeginPopup(remove_popup_id.c_str()))
						{
							ImGui::Text("Select data to remove:");
							ImGui::Separator();
							for (const auto& data : plot.second)
							{
								const std::string item_label = data.first + " " + data.second;
								if (ImGui::Selectable(item_label.c_str()))
								{
									visualization->removePlottingDataFromPlot(plot.first->getUniqueIdentifier(), data);
									ImGui::CloseCurrentPopup();
								}
							}
							ImGui::EndPopup();
						}

						ImGui::SameLine();
						// Remove plot button
						if (ImGui::Button(("Remove plot##" + std::to_string(plot.first->getUniqueIdentifier())).c_str()))
						{
							visualization->removePlot(plot.first->getUniqueIdentifier());
						}
					}

				}
				ImGui::EndTable();
			}
			ImGui::End();
		}


		//void PlotControlWindow::renderElementPlotTable() const
		//{
		//	if (ImGui::Begin("Element Plot Control"))
		//	{
		//		widgets::renderHelpMarker("Select the data you want to plot and where to plot it.");

		//		if (ImGui::BeginTable("PlotControlTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		//		{
		//			// Set column headers
		//			ImGui::TableSetupColumn("Elements");
		//			ImGui::TableSetupColumn("Components");
		//			ImGui::TableSetupColumn("Plot");
		//			ImGui::TableSetupColumn("Plot IDs");
		//			ImGui::TableHeadersRow();

		//			for (const auto& element : simulation->getElements())
		//			{
		//				// For each element, iterate over its components
		//				const auto* components = element->getComponents();
		//				for (const auto& [name, value] : *components) 
		//				{
		//					// Start a new row for each component
		//					ImGui::TableNextRow();
		//					// Element column
		//					ImGui::TableSetColumnIndex(0);
		//					ImGui::TextUnformatted(element->getUniqueName().c_str()); // Display element ID
		//					// Component column
		//					ImGui::TableSetColumnIndex(1);
		//					ImGui::TextUnformatted(name.c_str()); // Display component name
		//					// Plot checkbox column
		//					ImGui::TableSetColumnIndex(2);

		//					// Check if the component is already plotted and where
		//					std::vector<int> componentPlotIds = discoverPlotIdsIfComponentIsPlotted(&value);
		//					std::string componentPlotIdStr = {};
		//					for (const auto& id : componentPlotIds)
		//						componentPlotIdStr += std::to_string(id) + " ";

		//					bool isComponentInAPlot = !componentPlotIds.empty();
		//					bool usrToggledPlot = false;
		//					if (ImGui::Checkbox(("##PlotCheckbox" + element->getUniqueName() + name).c_str(), &isComponentInAPlot))
		//						usrToggledPlot = !usrToggledPlot;

		//					// Plot ID column
		//					ImGui::TableSetColumnIndex(3);

		//					if (componentPlotIds.empty())
		//					{
		//						if (usrToggledPlot)
		//						{
		//							visualization->plot({ {element->getUniqueName(), name} });
		//							ImGui::TextDisabled("_");
		//						}
		//						else
		//						{
		//							ImGui::TextDisabled("_");
		//						}
		//					}
		//					else
		//					{
		//						if (usrToggledPlot)
		//						{
		//							for (const auto& id : componentPlotIds)
		//								visualization->removePlottingDataFromPlot(id, { element->getUniqueName(), name });
		//							ImGui::TextDisabled("_");
		//						}
		//						else
		//						{
		//							std::string newComponentPlotIdsStr = componentPlotIdStr;
		//							std::vector<char> buffer(newComponentPlotIdsStr.begin(), newComponentPlotIdsStr.end());
		//							buffer.resize(newComponentPlotIdsStr.size() + 10);  // Ensure extra space for user input
		//							if (ImGui::InputText(("##PlotID" + element->getUniqueName() + name).c_str(), buffer.data(), buffer.size())) 
		//							{
		//								newComponentPlotIdsStr = buffer.data();
		//							}

		//							if (ImGui::IsItemDeactivatedAfterEdit())
		//							{
		//								std::vector<int> newComponentPlotIds;
		//								newComponentPlotIds.reserve(componentPlotIds.size());
		//								std::istringstream stream(newComponentPlotIdsStr);  // Create an input string stream

		//								int num;
		//								while (stream >> num)  // Extract integers from the stream
		//									newComponentPlotIds.emplace_back(num);  // Store each integer in the vector

		//								// Sort both vectors (required for set_difference)
		//								std::sort(componentPlotIds.begin(), componentPlotIds.end());
		//								std::sort(newComponentPlotIds.begin(), newComponentPlotIds.end());

		//								// Vectors to store differences
		//								std::vector<int> in_vec1_not_in_vec2;
		//								in_vec1_not_in_vec2.reserve(componentPlotIds.size());
		//								std::vector<int> in_vec2_not_in_vec1;
		//								in_vec2_not_in_vec1.reserve(newComponentPlotIds.size());

		//								// Get elements in vec1 but not in vec2
		//								std::set_difference(componentPlotIds.begin(), componentPlotIds.end(), newComponentPlotIds.begin(), newComponentPlotIds.end(),
		//									std::back_inserter(in_vec1_not_in_vec2));

		//								// Get elements in vec2 but not in vec1
		//								std::set_difference(newComponentPlotIds.begin(), newComponentPlotIds.end(), componentPlotIds.begin(), componentPlotIds.end(),
		//									std::back_inserter(in_vec2_not_in_vec1));

		//								if (!in_vec1_not_in_vec2.empty())
		//									for (int plotId : in_vec1_not_in_vec2) 
		//										visualization->removePlottingDataFromPlot(plotId, { element->getUniqueName(), name });

		//								if (!in_vec2_not_in_vec1.empty())
		//									for (int plotId : in_vec2_not_in_vec1)
		//										visualization->plot(plotId, { {element->getUniqueName(), name} });
		//								
		//							}

		//						}
		//					}
		//				}
		//			}
		//			ImGui::EndTable();
		//		}
		//		ImGui::End();
		//	}
		//	
		//}

		void PlotControlWindow::renderPlotTable()
		{
			//static int selectedPlotId = -1;

			//if (ImGui::Begin("Plot Property Editor"))
			//{
			//	widgets::renderHelpMarker("Select a plot from the left panel to edit its properties.");

			//	// Split left and right panes with ImGui::BeginChild for separate regions
			//	ImGui::BeginChild("PlotList", ImVec2(150, 0), true); // Left side for plot list

			//	// Display each plot ID in a list format
			//	const auto plots = visualization->getPlots();
			//	for (const auto& plot : plots)
			//	{
			//		const int plotId = plot->getUniqueIdentifier();

			//		if (ImGui::Selectable(("Plot #" + std::to_string(plotId)).c_str(), selectedPlotId == plotId))
			//		{
			//			selectedPlotId = plotId;
			//		}
			//	}
			//	ImGui::EndChild();

			//	ImGui::SameLine();

			//	// Right side: Display properties of the selected plot
			//	ImGui::BeginChild("PlotProperties", ImVec2(0, 0), true);

			//	auto selectedPlot = getSelectedPlot(selectedPlotId);
			//	if (selectedPlot)
			//	{
			//		bool parametersChanged = false;

			//		ImGui::Text("Plot ID: %d", selectedPlot->getUniqueIdentifier());
			//		ImGui::Separator();

			//		const PlotParameters oldParams = selectedPlot->getParameters();
			//		PlotParameters newParams = oldParams;

			//		// Editable plot properties
			//		char title[128];
			//		strncpy_s(title, oldParams.annotations.title.c_str(), sizeof(title));
			//		if (ImGui::InputText("Title", title, IM_ARRAYSIZE(title)))
			//		{
			//			newParams.annotations.title = title;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		char xLabel[128];
			//		strncpy_s(xLabel, oldParams.annotations.x_label.c_str(), sizeof(xLabel));
			//		if (ImGui::InputText("X Label", xLabel, IM_ARRAYSIZE(xLabel)))
			//		{
			//			newParams.annotations.x_label = xLabel;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		char yLabel[128];
			//		strncpy_s(yLabel, oldParams.annotations.y_label.c_str(), sizeof(yLabel));
			//		if (ImGui::InputText("Y Label", yLabel, IM_ARRAYSIZE(yLabel)))
			//		{
			//			newParams.annotations.y_label = yLabel;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		double xMin = oldParams.dimensions.xMin;
			//		if (ImGui::InputDouble("X Min", &xMin, 0.1, 1.0, "%.2f"))
			//		{
			//			newParams.dimensions.xMin = xMin;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		double xMax = oldParams.dimensions.xMax;
			//		if (ImGui::InputDouble("X Max", &xMax, 0.1, 1.0, "%.2f"))
			//		{
			//			newParams.dimensions.xMax = xMax;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		double yMin = oldParams.dimensions.yMin;
			//		if (ImGui::InputDouble("Y Min", &yMin, 0.1, 1.0, "%.2f"))
			//		{
			//			newParams.dimensions.yMin = yMin;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		double yMax = oldParams.dimensions.yMax;
			//		if (ImGui::InputDouble("Y Max", &yMax, 0.1, 1.0, "%.2f"))
			//		{
			//			newParams.dimensions.yMax = yMax;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		double xStep = oldParams.dimensions.xStep;
			//		if (ImGui::InputDouble("xStep", &xStep, 0.01, 0.1, "%.2f"))
			//		{
			//			newParams.dimensions.xStep = xStep;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		/*bool autoFit = oldParams.dimensions.autoFit;
			//		if (ImGui::Checkbox("Auto Fit", &autoFit))
			//		{
			//			newParams.dimensions.autoFit = autoFit;
			//			if (ImGui::IsItemDeactivatedAfterEdit())
			//				parametersChanged = true;
			//		}
			//		ImGui::Separator();
			//		if (ImGui::Button("Remove Plot"))
			//		{
			//			visualization->removePlot(selectedPlotId);
			//			selectedPlotId = -1;
			//		}*/
			//		if (parametersChanged)
			//		{
			//			if (newParams != oldParams)
			//			{
			//				selectedPlot->setParameters(newParams);
			//				updatePlotDimensions = true;
			//			}
			//		}
			//	}

			//	ImGui::EndChild();
			//}
			//ImGui::End();
		}

		Plot* PlotControlWindow::getSelectedPlot(int id) const
		{
			//for (const auto& plot : visualization->getPlots())
			//{
			//	if (plot->getUniqueIdentifier() == id)
			//	{
			//		return plot.get();  // Return a raw pointer to avoid unique_ptr copy issues
			//	}
			//}
			return nullptr;
		}

		//std::vector<int> PlotControlWindow::discoverPlotIdsIfComponentIsPlotted(const std::vector<double>* componentPtr) const
		//{
		//	std::vector<int> plotIds;
		//	plotIds.reserve(visualization->getPlots().size());
		//	for (const auto& plot : visualization->getPlots())
		//	{
		//		for (const auto& data : plot->getData())
		//		{
		//			if (data != nullptr && !data->empty())
		//			{
		//				if (componentPtr == data) // Compare addresses directly
		//				{
		//					plotIds.emplace_back(plot->getUniqueIdentifier());
		//				}
		//			}
		//		}
		//	}
		//	return plotIds;
		//}
	}
}
