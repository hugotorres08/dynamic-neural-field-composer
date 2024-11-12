#include "user_interface/widgets.h"


namespace dnf_composer
{
	namespace user_interface
	{
		namespace widgets
		{
			void renderHelpMarker(const char* desc)
			{
				ImGui::TextDisabled("(?)");
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(desc);
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}
			}

			void renderLinePlot(const Plot& plot, bool whereDimensionsChangedByUser)
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
						if (whereDimensionsChangedByUser)
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

			void renderHeatmap(const Plot& plot)
			{
				const auto data = plot.getData()[0];
				const auto parameters = plot.getParameters();

				static const int rows = 100;
				static const int cols = 100;
				static const int x_max = 100;
				static const int y_max = 100;

				static constexpr ImPlotFlags hm_flags = ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
				//ImPlot::SetNextAxesToFit();
				ImVec2 plotSize = ImGui::GetContentRegionAvail();  // Get available size in the ImGui window
				plotSize.x -= 60.0f; // Subtract some padding
				plotSize.y -= 5.0f; // Subtract some padding

				constexpr float scaleMin = -0.0f;
				constexpr float scaleMax = 0.2f;

				if (ImPlot::BeginPlot("##Heatmap1", plotSize, hm_flags))
				{
					static constexpr ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoLabel;
					ImPlot::SetupAxes("x", "y" , flags, flags);
					/*ImPlot::PlotHeatmap("Heatmap Data", data, rows, cols,
						scaleMin, scaleMax, nullptr,
						ImPlotPoint(0, 0), ImPlotPoint(x_max, y_max), hm_flags);*/
					ImPlot::PlotHeatmap("Heatmap Data", data->data(), rows, cols,
						scaleMin, scaleMax, nullptr,
						ImPlotPoint(0, 0), ImPlotPoint(x_max, y_max), hm_flags);
					ImPlot::EndPlot();
				}

				ImGui::SameLine();
				ImPlot::ColormapScale("##HeatScale", scaleMin, scaleMax, ImVec2(60, plotSize.y));

				//ImPlot::PopColormap();
			}
		}
	}
}
