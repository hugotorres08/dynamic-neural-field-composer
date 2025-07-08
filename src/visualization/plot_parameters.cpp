#include "visualization/plot_parameters.h"


namespace dnf_composer
{
	PlotDimensions::PlotDimensions()
		: xMin(0), xMax(100), yMin(-10.0), yMax(10.0), xStep(1.0), yStep(1.0)
	{}

	PlotDimensions::PlotDimensions(const double& x_min, const double& x_max, 
		const double& y_min, const double& y_max, 
		const double& x_step, const double& y_step)
		: xMin(x_min), xMax(x_max), yMin(y_min), yMax(y_max), xStep(x_step), yStep(y_step)
	{
		if (xMin >= xMax)
		{
			xMin = 0;
			xMax = 100;
			log(tools::logger::LogLevel::WARNING, "xMin must be less than xMax.");
			return;
		}
		if (yMin >= yMax)
		{
			yMin = -10.0;
			yMax = 10.0;
			log(tools::logger::LogLevel::WARNING, "yMin must be less than yMax.");
			return;
		}
		if (xStep <= 0)
		{
			this->xStep = 1.0;
			log(tools::logger::LogLevel::WARNING, "xStep must be positive.");
			return;
		}
		if (yStep <= 0)
		{
			this->yStep = 1.0;
			log(tools::logger::LogLevel::WARNING, "yStep must be positive.");
			return;
		}
	}

	PlotDimensions::PlotDimensions(double xStep)
		: xMin(0), xMax(100), yMin(0), yMax(1), xStep(xStep), yStep(1.0)
	{
		if (xStep <= 0)
		{
			this->xStep = 1.0;
			log(tools::logger::LogLevel::WARNING, "xStep must be positive.");
		}
	}

	bool PlotDimensions::isLegal() const
	{
		if (xMin >= xMax || yMin >= yMax || xStep <= 0 || yStep <= 0)
			return false;
		return true;
	}

	std::string PlotDimensions::toString() const
	{
		std::string result;
		result += "Plot dimensions: {";
		result += "xMin: " + std::to_string(xMin) + ", ";
		result += "xMax: " + std::to_string(xMax) + ", ";
		result += "yMin: " + std::to_string(yMin) + ", ";
		result += "yMax: " + std::to_string(yMax) + ", ";
		result += "xStep: " + std::to_string(xStep) + ", ";
		result += "yStep: " + std::to_string(yStep) + "}";
		return result;
	}

	bool PlotDimensions::operator==(const PlotDimensions& other) const
	{
		constexpr double epsilon = 1e-6;

		if (std::fabs(xMin - other.xMin) > epsilon ||
			std::fabs(xMax - other.xMax) > epsilon ||
			std::fabs(yMin - other.yMin) > epsilon ||
			std::fabs(yMax - other.yMax) > epsilon ||
			std::fabs(xStep - other.xStep) > epsilon ||
			std::fabs(yStep - other.yStep) > epsilon)
		{
			return false;
		}
		return true;
	}

	PlotAnnotations::PlotAnnotations()
		:title("title"), x_label("x label"), y_label("y label")
	{}

	PlotAnnotations::PlotAnnotations(std::string title, std::string x_label, std::string y_label)
		:title(std::move(title)), x_label(std::move(x_label)), y_label(std::move(y_label))
	{}

	std::string PlotAnnotations::toString() const
	{
		std::string result;
		result += "Plot annotations: {";
		result += "title: " + title + ", ";
		result += "x_label: " + x_label + ", ";
		result += "y_label: " + y_label + "}";
		return result;
	}

	bool PlotAnnotations::operator==(const PlotAnnotations& other) const
	{
		if (title != other.title || x_label != other.x_label || y_label != other.y_label )
			return false;
		return true;
	}

	PlotCommonParameters::PlotCommonParameters()
		: type(PlotType::LINE_PLOT), dimensions(), annotations()
	{}

	PlotCommonParameters::PlotCommonParameters(const PlotType type, const PlotDimensions& dimensions, PlotAnnotations annotations)
		: type(type), dimensions(dimensions), annotations(std::move(annotations))
	{}

	std::string PlotCommonParameters::toString() const
	{
		std::string result;
		result += "Plot parameters: [ ";
		result += "type: " + PlotTypeToString.at(type) + ", ";
		result += dimensions.toString() + ", ";
		result += annotations.toString() + " ]";
		return result;
	}

	bool PlotCommonParameters::operator==(const PlotCommonParameters& other) const
	{
		if (dimensions != other.dimensions || annotations != other.annotations)
			return false;
		return true;
	}

}
