#include "visualization/plot.h"

namespace dnf_composer
{
	PlotDimensions::PlotDimensions()
		: xMin(0), xMax(100), yMin(-10.0), yMax(10.0), dx(1.0), autoFit(true)
	{}

	PlotDimensions::PlotDimensions(const double& x_min, const double& x_max, const double& y_min, const double& y_max, const double& d_x)
		: xMin(x_min), xMax(x_max), yMin(y_min), yMax(y_max), dx(d_x), autoFit(false)
	{
		if (xMin >= xMax)
		{
			autoFit = true;
			xMin = 0;
			xMax = 100;
			log(tools::logger::LogLevel::WARNING, "xMin must be less than xMax. Auto-fitting enabled.");
			return;
		}
		if (yMin >= yMax)
		{
			yMin = -10.0;
			yMax = 10.0;
			autoFit = true;
			log(tools::logger::LogLevel::WARNING, "yMin must be less than yMax. Auto-fitting enabled.");
			return;
		}
		if (dx <= 0)
		{
			this->dx = 1.0;
			autoFit = true;
			log(tools::logger::LogLevel::WARNING, "dx must be positive. Auto-fitting enabled and dx set to 1.0.");
			return;
		}
	}

	PlotDimensions::PlotDimensions(double dx)
		: xMin(0), xMax(100), yMin(0), yMax(1), dx(dx), autoFit(true)
	{
		if (dx <= 0)
		{
			this->dx = 1.0;
			autoFit = true;
			log(tools::logger::LogLevel::WARNING, "dx must be positive. Auto-fitting enabled and dx set to 1.0.");
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
			std::fabs(dx - other.dx) > epsilon ||
			autoFit != other.autoFit)
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
		: dimensions(), annotations()
	{}

	PlotParameters::PlotParameters(const PlotDimensions& dimensions, PlotAnnotations annotations)
		: dimensions(dimensions), annotations(std::move(annotations))
	{}

	std::string PlotParameters::toString() const
	{
		std::string result;
		result += "Plot parameters: [ ";
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

	Plot::Plot(PlotParameters parameters, const std::vector<std::vector<double>*>& data, const std::vector<std::string>& legends)
		: uniqueIdentifier(uniqueIdentifierCounter++), parameters(std::move(parameters)), data()
	{
		addPlottingData(data, legends);
	}

	void Plot::addPlottingData(const std::vector<std::vector<double>*>& data, const std::vector<std::string>& legends)
	{
		if (data.empty())
			return;

		if (!legends.empty() && data.size() != legends.size())
		{
			log(tools::logger::LogLevel::WARNING, "Data and legends sizes do not match. Skipping adding data to plot " + std::to_string(uniqueIdentifier) + ".");
			return;
		}

		for (size_t i = 0; i < data.size(); ++i)
		{
			addPlottingData(data[i], legends.empty() ? "" : legends[i]);
		}
	}

	void Plot::addPlottingData(std::vector<double>* data, const std::string& legend)
	{
		if (data == nullptr)
		{
			log(tools::logger::LogLevel::WARNING, "Data is nullptr. Skipping adding data to plot " + std::to_string(uniqueIdentifier) + ".");
			return;
		}

		const auto it = std::find(this->data.begin(), this->data.end(), data);
		if (it != this->data.end())
		{
			log(tools::logger::LogLevel::WARNING, "Data already exists in the plot. Skipping adding data to plot " + std::to_string(uniqueIdentifier) + ".");
			return;		
		}

		this->data.emplace_back(data);
		if (legend.empty())
		{
			std::ostringstream oss;
			oss << reinterpret_cast<void*>(data);
			parameters.annotations.legends.emplace_back(oss.str());
		}
		else
			parameters.annotations.legends.emplace_back(legend);
		log(tools::logger::LogLevel::INFO, "Data added to plot " + std::to_string(uniqueIdentifier) + ".");
	}

	void Plot::removePlottingData(const std::vector<double>* data)
	{
		if (data == nullptr)
		{
			log(tools::logger::LogLevel::WARNING, "Data is nullptr. Skipping removing data from plot " + std::to_string(uniqueIdentifier) + ".");
			return;
		}

		const auto it = std::find(this->data.begin(), this->data.end(), data);
		if (it != this->data.end())
		{
			const auto legendIt = parameters.annotations.legends.begin() + (it - this->data.begin());
			this->data.erase(it);
			parameters.annotations.legends.erase(legendIt);
			log(tools::logger::LogLevel::INFO, "Data removed from plot " + std::to_string(uniqueIdentifier) + ".");
			return;
		}
		log(tools::logger::LogLevel::WARNING, "Data does not exist in the plot. Skipping removing data from plot " + std::to_string(uniqueIdentifier) + ".");
	}

	void Plot::setParameters(const PlotParameters& parameters)
	{
		this->parameters = parameters;
		log(tools::logger::LogLevel::INFO, "Parameters changed for plot " + std::to_string(uniqueIdentifier) + ".");
	}

	int Plot::getUniqueIdentifier() const
	{
		return uniqueIdentifier;
	}

	PlotParameters Plot::getParameters() const
	{
		return parameters;
	}

	std::vector<std::vector<double>*> Plot::getData() const
	{
		return data;
	}

	std::string Plot::toString() const
	{
		std::ostringstream result;
		result << "Plot: { ";
		result << "Unique identifier: " << uniqueIdentifier << ", ";
		result << parameters.toString() << ", ";
		result << "Data*: [ ";
		for (auto& d : data)
			result << reinterpret_cast<void*>(d) << ", ";
		result << " ] }";
		return result.str();
	}
}