#include "visualization/heatmap.h"

namespace dnf_composer
{
	HeatmapParameters::HeatmapParameters()
		: scaleMin(0), scaleMax(1)
	{}

	HeatmapParameters::HeatmapParameters(double scaleMin, bool scaleMax)
		: scaleMin(scaleMin), scaleMax(scaleMax)
	{}

	std::string HeatmapParameters::toString() const
	{
		std::string result;
		result += "Heatmap parameters: {";
		result += "Scale min: " + std::to_string(scaleMin) + ", ";
		result += "Scale max: " + std::to_string(scaleMax) + "}";
		return result;
	}

	bool HeatmapParameters::operator==(const HeatmapParameters& other) const
	{
		static constexpr double epsilon = 1e-6;
		if (std::abs(scaleMin - other.scaleMin) > epsilon || scaleMax != other.scaleMax)
			return false;
		return true;
	}

	Heatmap::Heatmap(const PlotParameters& parameters, const HeatmapParameters& heatmapParameters, const std::vector<std::vector<double>*>& data, const std::vector<std::string>& legends)
		: Plot(parameters, data, legends), parameters(heatmapParameters)
	{
	}

	void Heatmap::setScale(double min, double max)
	{
		if (min >= max)
		{
			parameters.scaleMin = 0;
			parameters.scaleMax = 1;
			log(tools::logger::LogLevel::WARNING, "Scale min must be less than scale max.");
			return;
		}
		parameters.scaleMin = min;
		parameters.scaleMax = max;
	}

	std::pair<double, double> Heatmap::getScale() const
	{
		return { parameters.scaleMin, parameters.scaleMax };
	}

	std::string Heatmap::toString() const
	{
		std::ostringstream result;
		result << "Plot: { ";
		result << "Unique identifier: " << uniqueIdentifier << ", ";
		result << parameters.toString() << ", ";
		result << "Data*: [ ";
		for (auto& d : data)
			result << reinterpret_cast<void*>(d) << ", ";
		result << " ] }";
		result << parameters.toString();
		return result.str();
	}

	void Heatmap::render()
	{
		if (data.size() != 1)
		{
			return;
		}

		const ImVec2 availableRegionSize = ImGui::GetContentRegionAvail();
		const ImVec2 plotSize = ImVec2(availableRegionSize.x - 90.0f, availableRegionSize.y - 5.0f);

		const auto flattened_matrix = data[0];
		const std::string uniquePlotID = parameters.annotations.title + "##" + std::to_string(uniqueIdentifier);

		auto x_max = static_cast<int>(parameters.dimensions.xMax);
		auto x_min = static_cast<int>(parameters.dimensions.xMin);
		auto y_max = static_cast<int>(parameters.dimensions.yMax);
		auto y_min = static_cast<int>(parameters.dimensions.yMin);
		auto scaleMin = static_cast<float>(parameters.scaleMin);
		auto scaleMax = static_cast<float>(parameters.scaleMax);

		std::string title = parameters.annotations.title;
		std::string x_label = parameters.annotations.x_label;
		std::string y_label = parameters.annotations.y_label;
		char titleBuffer[128];
		char xLabelBuffer[128];
		char yLabelBuffer[128];
		strncpy_s(titleBuffer, title.c_str(), sizeof(titleBuffer) - 1);
		strncpy_s(xLabelBuffer, x_label.c_str(), sizeof(xLabelBuffer) - 1);
		strncpy_s(yLabelBuffer, y_label.c_str(), sizeof(yLabelBuffer) - 1);

		static ImPlotColormap map = ImPlotColormap_Deep;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Dimensions"))
			{
				if(ImGui::DragInt("X max", &x_max, 1, x_min, 1000))
					parameters.dimensions.xMax = x_max;
				if(ImGui::DragInt("Y max", &y_max, 1, y_min, 1000))
					parameters.dimensions.yMax = y_max;
				if(ImGui::DragInt("X min", &x_min, 1, 0, x_max))
					parameters.dimensions.xMin = x_min;
				if(ImGui::DragInt("Y min", &y_min, 1, 0, y_max))
					parameters.dimensions.yMin = y_min;
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Annotations"))
			{
				if (ImGui::InputText("Title", titleBuffer, sizeof(titleBuffer)))
				{
					title = titleBuffer;
					parameters.annotations.title = title;
				}
				if (ImGui::InputText("X label", xLabelBuffer, sizeof(xLabelBuffer)))
				{
					x_label = xLabelBuffer;
					parameters.annotations.x_label = x_label;
				}
				if (ImGui::InputText("Y label", yLabelBuffer, sizeof(yLabelBuffer)))
				{
					y_label = yLabelBuffer;
					parameters.annotations.y_label = y_label;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Colormap"))
			{
				if (ImPlot::ColormapButton(ImPlot::GetColormapName(map), ImVec2(availableRegionSize.x - 90.0f, 0.0f), map))
				{
					map = (map + 1) % ImPlot::GetColormapCount();
					ImPlot::BustColorCache(uniquePlotID.c_str());
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scale"))
			{
				ImGui::DragFloatRange2("Min / Max", &scaleMin, &scaleMax, 0.01f, -20, 20);
				parameters.scaleMin = scaleMin;
				parameters.scaleMax = scaleMax;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		static constexpr ImPlotFlags hm_flags = ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
		if (ImPlot::BeginPlot(uniquePlotID.c_str(), plotSize, hm_flags)) {
			ImPlot::PushColormap(map);
			static constexpr ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit;
			ImPlot::SetupAxes(parameters.annotations.x_label.c_str(), parameters.annotations.y_label.c_str(), flags, flags);

			ImPlot::PlotHeatmap(
				parameters.annotations.legends[0].c_str(),
				flattened_matrix->data(),
				y_max, x_max,
				scaleMin, scaleMax, nullptr,
				ImPlotPoint(y_min, y_max), ImPlotPoint(x_max, x_min),
				hm_flags
			);
			ImPlot::EndPlot();
		}

		// Add color scale next to the heatmap
		ImGui::SameLine();
		ImPlot::ColormapScale("##HeatScale", scaleMin, scaleMax, ImVec2(60, plotSize.y));
		ImPlot::PopColormap();
	}

}
