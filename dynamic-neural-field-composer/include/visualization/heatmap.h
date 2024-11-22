#pragma once

#include "plot.h"

namespace dnf_composer
{
	struct HeatmapParameters : PlotSpecificParameters
	{
		double scaleMin, scaleMax;
		bool autoScale;

		HeatmapParameters();
		HeatmapParameters(double scaleMin, double scaleMax);
		std::string toString() const override;
		bool operator==(const HeatmapParameters& other) const;
	};

	class Heatmap : public Plot
	{
		HeatmapParameters heatmapParameters;
	public:
		Heatmap(const PlotCommonParameters& parameters = PlotCommonParameters(),
						const HeatmapParameters& heatmapParameters = HeatmapParameters(),
						const std::vector<std::vector<double>*>& data = {},
						const std::vector<std::string>& legends = {});
		void setScale(double min, double max);
		std::pair<double, double> getScale() const;
		std::string toString() const override;
		void render() override;
	};
}