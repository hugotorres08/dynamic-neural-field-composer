#include "visualization/plot.h"

namespace dnf_composer
{
	PlotDimensions::PlotDimensions()
		: xMin(0), xMax(100), yMin(-5), yMax(15), dx(1.0)
	{}

	PlotDimensions::PlotDimensions(int xMin, int xMax, int yMin, int yMax, double dx)
		: xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), dx(dx)
	{
		if (xMin >= xMax)
			throw std::exception("xMin must be less than xMax");
		if (yMin >= yMax)
			throw std::exception("yMin must be less than yMax");
		if (dx <= 0)
			throw std::exception("dx must be positive");
	}

	bool PlotDimensions::isNull() const
	{
		return xMin == 0 && xMax == 0 && yMin == 0 && yMax == 0 && dx == 0.0;
	}

	PlotAnnotations::PlotAnnotations()
		:title("title"), x_label("x label"), y_label("y label")
	{}

	PlotAnnotations::PlotAnnotations(std::string title, std::string x_label, std::string y_label)
		:title(std::move(title)), x_label(std::move(x_label)), y_label(std::move(y_label))
	{}

	PlotParameters::PlotParameters()
		: dimensions(), annotations()
	{}

	PlotParameters::PlotParameters(const PlotDimensions& dimensions, PlotAnnotations annotations)
		: dimensions(dimensions), annotations(std::move(annotations))
	{}

	Plot::Plot(const PlotParameters& parameters)
		: uniqueIdentifier(uniqueIdentifierCounter++), parameters(parameters)
	{}

	void Plot::addPlottingData(std::vector<double>* data)
	{
		this->data.emplace_back(std::move(data));
	}
}