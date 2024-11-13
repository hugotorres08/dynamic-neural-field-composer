#include "visualization/plot_parameters.h"


namespace dnf_composer
{
	PlotDimensions::PlotDimensions()
		: xMin(0), xMax(100), yMin(-10.0), yMax(10.0), dx(1.0)
	{}

	PlotDimensions::PlotDimensions(const double& x_min, const double& x_max, const double& y_min, const double& y_max, const double& d_x)
		: xMin(x_min), xMax(x_max), yMin(y_min), yMax(y_max), dx(d_x)
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
		if (dx <= 0)
		{
			this->dx = 1.0;
			log(tools::logger::LogLevel::WARNING, "dx must be positive.");
			return;
		}
	}

	PlotDimensions::PlotDimensions(double dx)
		: xMin(0), xMax(100), yMin(0), yMax(1), dx(dx)
	{
		if (dx <= 0)
		{
			this->dx = 1.0;
			log(tools::logger::LogLevel::WARNING, "dx must be positive.");
		}
	}

	bool PlotDimensions::isLegal() const
	{
		if (xMin >= xMax || yMin >= yMax || dx <= 0)
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
		result += "dx: " + std::to_string(dx) + "}";
		return result;
	}

	bool PlotDimensions::operator==(const PlotDimensions& other) const
	{
		constexpr double epsilon = 1e-6;

		if (std::fabs(xMin - other.xMin) > epsilon ||
			std::fabs(xMax - other.xMax) > epsilon ||
			std::fabs(yMin - other.yMin) > epsilon ||
			std::fabs(yMax - other.yMax) > epsilon ||
			std::fabs(dx - other.dx) > epsilon)
		{
			return false;
		}
		return true;
	}

	PlotAnnotations::PlotAnnotations()
		:title("title"), x_label("x label"), y_label("y label"), legends()
	{}

	PlotAnnotations::PlotAnnotations(std::string title, std::string x_label, std::string y_label)
		:title(std::move(title)), x_label(std::move(x_label)), y_label(std::move(y_label)), legends()
	{}

	std::string PlotAnnotations::toString() const
	{
		std::string result;
		result += "Plot annotations: {";
		result += "title: " + title + ", ";
		result += "x_label: " + x_label + ", ";
		result += "y_label: " + y_label + "}";
		result += "legends: ";
		if (legends.empty())
			result += "none";
		else
			for (const auto& l : legends)
				result += ", " + l;
		return result;
	}

	bool PlotAnnotations::operator==(const PlotAnnotations& other) const
	{
		if (title != other.title || x_label != other.x_label || y_label != other.y_label || legends != other.legends)
			return false;
		return true;
	}

	PlotParameters::PlotParameters()
		: dimensions(), annotations(), type(PlotType::LINE_PLOT)
	{}

	PlotParameters::PlotParameters(const PlotType type, const PlotDimensions& dimensions, PlotAnnotations annotations)
		: dimensions(dimensions), annotations(std::move(annotations)), type(type)
	{}

	std::string PlotParameters::toString() const
	{
		std::string result;
		result += "Plot parameters: [ ";
		result += "Type: " + PlotTypeToString.at(type) + ", ";
		result += dimensions.toString() + ", ";
		result += annotations.toString() + " ]";
		return result;
	}

	bool PlotParameters::operator==(const PlotParameters& other) const
	{
		if (dimensions != other.dimensions || annotations != other.annotations)
			return false;
		return true;
	}

}
