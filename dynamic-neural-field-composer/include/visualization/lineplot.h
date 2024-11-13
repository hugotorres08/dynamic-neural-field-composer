#pragma once

#include "plot.h"

namespace dnf_composer
{
	struct LinePlotParameters : public PlotParameters
	{
		double lineThickness;
		bool autoFit;

		LinePlotParameters();
		LinePlotParameters(double lineThickness, bool autoFit);
		std::string toString() const;
		bool operator==(const LinePlotParameters& other) const;
	};


	class LinePlot : public Plot
	{
		LinePlotParameters parameters;
	public:
		LinePlot(const PlotParameters& parameters = PlotParameters(),
			const LinePlotParameters& linePlotParameters = LinePlotParameters(),
			const std::vector<std::vector<double>*>& data = {},
			const std::vector<std::string>& legends = {});

		void setLineThickness(double lineThickness);
		void setAutoFit(bool autoFit);
		double getLineThickness() const;
		double getAutoFit() const;
		std::string toString() const override;
		void render() override;
	};
}
