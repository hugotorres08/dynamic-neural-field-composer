#pragma once

#include "plot.h"

namespace dnf_composer
{
	struct HeatmapParameters : public PlotParameters
	{
		double scaleMin, scaleMax;

		HeatmapParameters();
		HeatmapParameters(double scaleMin, bool scaleMax);
		std::string toString() const;
		bool operator==(const HeatmapParameters& other) const;
	};

	class Heatmap : public Plot
	{
		HeatmapParameters parameters;
	public:
		Heatmap(const PlotParameters& parameters = PlotParameters(),
						const HeatmapParameters& heatmapParameters = HeatmapParameters(),
						const std::vector<std::vector<double>*>& data = {},
						const std::vector<std::string>& legends = {});
		void setScale(double min, double max);
		std::pair<double, double> getScale() const;
		std::string toString() const override;
		void render() override;
	};
}