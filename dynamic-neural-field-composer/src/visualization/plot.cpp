#include "visualization/plot.h"

namespace dnf_composer
{
	PlotDimensions::PlotDimensions()
		: xMin(0), xMax(1), yMin(0), yMax(1), dx(1.0)
	{}

	PlotDimensions::PlotDimensions(int xMin, int xMax, int yMin, int yMax, double dx)
		: xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), dx(dx)
	{
		if (xMin >= xMax)
		{
			this->xMin = 0;
			this->xMax = 1;
			log(tools::logger::LogLevel::WARNING, "xMin must be less than xMax.");
		}
		if (yMin >= yMax)
		{
			this->yMin = 0;
			this->yMax = 1;
			log(tools::logger::LogLevel::WARNING, "yMin must be less than yMax.");
		}
		if (dx <= 0)
		{
			this->dx = 1.0;
			log(tools::logger::LogLevel::WARNING, "dx must be positive.");
		}
	}

	bool PlotDimensions::areUndefined() const
	{
		return xMin == 0 && xMax == 1 && yMin == 0 && yMax == 1 && dx == 1.0;
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
		if (xMin != other.xMin || xMax != other.xMax || yMin != other.yMin || yMax != other.yMax || dx != other.dx)
			return false;
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

	Plot::Plot(const PlotParameters& parameters, const std::vector<std::vector<double>*>& data, const std::vector<std::string>& legends)
		: uniqueIdentifier(uniqueIdentifierCounter++), parameters(parameters)
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

		for (auto& d : data)
		{
			addPlottingData(d, legends.empty() ? "" : legends[&d - &data[0]]);
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

	void Plot::removePlottingData(std::vector<double>* data)
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
		log(tools::logger::LogLevel::INFO, "Parameters changed for plot " + std::to_string(uniqueIdentifier) + ". New " + parameters.toString() + ".");
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