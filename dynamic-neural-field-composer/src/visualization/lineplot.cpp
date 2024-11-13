#include "visualization/lineplot.h"

namespace dnf_composer
{
	LinePlotParameters::LinePlotParameters()
		: lineThickness(3.0), autoFit(false)
	{}

	LinePlotParameters::LinePlotParameters(double lineThickness, bool autoFit)
		: lineThickness(lineThickness), autoFit(autoFit)
	{}

	std::string LinePlotParameters::toString() const
	{
		std::string result;
		result += "Line plot parameters: {";
		result += "Thickness: " + std::to_string(lineThickness) + ", ";
		result += "Auto-fit: " + std::string(autoFit ? "true" : "false") + "}";
		return result;
	}

	bool LinePlotParameters::operator==(const LinePlotParameters& other) const
	{
		static constexpr double epsilon = 1e-6;
		if (std::abs(lineThickness - other.lineThickness) > epsilon || autoFit != other.autoFit)
			return false;
		return true;
	}


	LinePlot::LinePlot(const PlotParameters& parameters, const LinePlotParameters& linePlotParameters, const std::vector<std::vector<double>*>& data, const std::vector<std::string>& legends)
		: Plot(parameters, data, legends), parameters(linePlotParameters)
	{
	}

	void LinePlot::setLineThickness(double lineThickness)
	{
		parameters.lineThickness = lineThickness;
	}

	void LinePlot::setAutoFit(bool autoFit)
	{
		parameters.autoFit = autoFit;
	}

	double LinePlot::getLineThickness() const
	{
		return parameters.lineThickness;
	}

	double LinePlot::getAutoFit() const
	{
		return parameters.autoFit;
	}

	std::string LinePlot::toString() const
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

	void LinePlot::render()
	{
        static constexpr double safeMargin = 0.01;
		bool whereDimensionsChangedByUser = false;

        ImPlotStyle& style = ImPlot::GetStyle();
        style.LineWeight = static_cast<float>(parameters.lineThickness);

        const ImVec2 availableRegionSize = ImGui::GetContentRegionAvail();
        const ImVec2 plotSize = ImVec2(availableRegionSize.x - 5.0f, availableRegionSize.y - 5.0f);

        ImPlotFlags flags = ImPlotFlags_Crosshairs;
        const std::string uniquePlotID = parameters.annotations.title + "##" + std::to_string(uniqueIdentifier);

		auto x_max = static_cast<float>(parameters.dimensions.xMax);
		auto x_min = static_cast<float>(parameters.dimensions.xMin);
		auto y_max = static_cast<float>(parameters.dimensions.yMax);
		auto y_min = static_cast<float>(parameters.dimensions.yMin);
		auto lineWeight = static_cast<float>(parameters.lineThickness);
		bool autoFit = parameters.autoFit;

		std::string title = parameters.annotations.title;
		std::string x_label = parameters.annotations.x_label;
		std::string y_label = parameters.annotations.y_label;
		char titleBuffer[128];
		char xLabelBuffer[128];
		char yLabelBuffer[128];
		strncpy_s(titleBuffer, title.c_str(), sizeof(titleBuffer) - 1);
		strncpy_s(xLabelBuffer, x_label.c_str(), sizeof(xLabelBuffer) - 1);
		strncpy_s(yLabelBuffer, y_label.c_str(), sizeof(yLabelBuffer) - 1);


        if (ImGui::BeginMenuBar())
        {
			if (ImGui::BeginMenu("Dimensions"))
			{
				if (ImGui::DragFloat("X max", &x_max, 0.1f, x_min, 1000))
				{
					parameters.dimensions.xMax = x_max;
					whereDimensionsChangedByUser = true;
				}
				if (ImGui::DragFloat("Y max", &y_max, 0.1f, y_min, 1000))
				{
					parameters.dimensions.yMax = y_max;
					whereDimensionsChangedByUser = true;
				}
				if (ImGui::DragFloat("X min", &x_min, 0.1f, -1000, x_max))
				{
					parameters.dimensions.xMin = x_min;
					whereDimensionsChangedByUser = true;
				}
				if (ImGui::DragFloat("Y min", &y_min, 0.1f, -10000, y_max))
				{
					parameters.dimensions.yMin = y_min;
					whereDimensionsChangedByUser = true;
				}
				if (ImGui::Checkbox("Auto-fit", &autoFit))
					parameters.autoFit = autoFit;
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
                ImPlot::ShowColormapSelector("##ColormapSelector");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Line Thickness"))
            {
                ImGui::SliderFloat("##LineWeight", &lineWeight, 0.1f, 10.0f);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (parameters.autoFit) {
            flags |= ImPlotFlags_Equal;
        }
        else {
            ImPlot::SetNextAxesLimits(
                parameters.dimensions.xMin - safeMargin,
                parameters.dimensions.xMax + safeMargin,
                parameters.dimensions.yMin - safeMargin,
                parameters.dimensions.yMax + safeMargin
            );
        }

        if (!ImPlot::BeginPlot(uniquePlotID.c_str(), plotSize, flags)) {
            return; 
        }

        if (parameters.autoFit) {
            ImPlot::SetupAxes(
                parameters.annotations.x_label.c_str(),
                parameters.annotations.y_label.c_str(),
                ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit
            );
        }
        else {
            ImPlot::SetupAxes(parameters.annotations.x_label.c_str(), parameters.annotations.y_label.c_str());
            if (whereDimensionsChangedByUser) {
                auto* currentPlot = ImPlot::GetCurrentPlot();
                if (currentPlot) {
                    currentPlot->Axes[0].Range.Min = parameters.dimensions.xMin - safeMargin;
                    currentPlot->Axes[0].Range.Max = parameters.dimensions.xMax + safeMargin;
                    currentPlot->Axes[3].Range.Min = parameters.dimensions.yMin - safeMargin;
                    currentPlot->Axes[3].Range.Max = parameters.dimensions.yMax + safeMargin;
                }
            }
        }

        ImPlot::SetupLegend(ImPlotLocation_South, ImPlotLegendFlags_Horizontal);

        for (size_t j = 0; j < data.size(); ++j) {
            std::string label = parameters.annotations.legends[j];
            const std::vector<double>& line_data = *data[j];

            std::vector<double> shiftedXValues(line_data.size());
            for (size_t i = 0; i < line_data.size(); ++i) {
                shiftedXValues[i] = (i + 1) * parameters.dimensions.dx;
            }

            ImPlot::PlotLine(label.c_str(), shiftedXValues.data(), line_data.data(), static_cast<int>(line_data.size()));
        }
        ImPlot::EndPlot();
	}

}
