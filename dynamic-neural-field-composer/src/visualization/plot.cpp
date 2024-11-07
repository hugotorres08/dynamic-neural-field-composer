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
			throw std::invalid_argument("xMin must be less than xMax.");
		if (yMin >= yMax)
			throw std::invalid_argument("yMin must be less than yMax.");
		if (dx <= 0)
			throw std::invalid_argument("dx must be positive.");
	}

	bool PlotDimensions::isNull() const
	{
		return xMin == 0 && xMax == 0 && yMin == 0 && yMax == 0 && dx == 0.0;
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

	Plot::Plot(const PlotParameters& parameters, std::vector<std::vector<double>*> data)
		: uniqueIdentifier(uniqueIdentifierCounter++), parameters(parameters)
	{
		log(tools::logger::LogLevel::INFO, "Plot created with id " + std::to_string(uniqueIdentifier) + ". " + parameters.toString() + ".");
		addPlottingData(data);
	}

	void Plot::addPlottingData(std::vector<std::vector<double>*> data)
	{
		if (data.empty())
			return;

		for (auto& d : data)
		{
			addPlottingData(d);
		}
	}

	void Plot::addPlottingData(std::vector<double>* data)
	{
		if (data == nullptr)
		{
			log(tools::logger::LogLevel::WARNING, "Data is nullptr. Skipping adding data to plot " + std::to_string(uniqueIdentifier) + ".");
			return;
		}

		auto it = std::find(this->data.begin(), this->data.end(), data);
		if (it != this->data.end())
		{
			log(tools::logger::LogLevel::WARNING, "Data already exists in the plot. Skipping adding data to plot " + std::to_string(uniqueIdentifier) + ".");
			return;		
		}

		this->data.emplace_back(data);
		log(tools::logger::LogLevel::INFO, "Data added to plot " + std::to_string(uniqueIdentifier) + ".");
	}

	void Plot::removePlottingData(std::vector<double>* data)
	{
		if (data == nullptr)
		{
			log(tools::logger::LogLevel::WARNING, "Data is nullptr. Skipping removing data from plot " + std::to_string(uniqueIdentifier) + ".");
			return;
		}

		auto it = std::find(this->data.begin(), this->data.end(), data);
		if (it != this->data.end())
		{
			this->data.erase(it);
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
}