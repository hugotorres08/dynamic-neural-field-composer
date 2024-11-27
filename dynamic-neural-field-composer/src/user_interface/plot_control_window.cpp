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
			if (ImGui::Begin("Element Plot Control"))
			{
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
							ImGui::EndListBox();
						}

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
	}
}
