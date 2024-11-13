#include "visualization/plot.h"

namespace dnf_composer
{

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

		if (parameters.type == PlotType::HEATMAP && data.size() > 1)
		{
			log(tools::logger::LogLevel::WARNING, "Heatmap plot can only have one data series. Skipping adding data to plot " + std::to_string(uniqueIdentifier) + ".");
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

	int Plot::getUniqueIdentifier() const
	{
		return uniqueIdentifier;
	}

	PlotType Plot::getType() const
	{
		return parameters.type;
	}

	std::vector<std::vector<double>*> Plot::getData() const
	{
		return data;
	}

	PlotDimensions Plot::getDimensions() const
	{
		return parameters.dimensions;
	}

	PlotAnnotations Plot::getAnnotations() const
	{
		return parameters.annotations;
	}

	void Plot::setDimensions(const PlotDimensions& dimensions)
	{
		parameters.dimensions = dimensions;
	}

	void Plot::setAnnotations(const PlotAnnotations& annotations)
	{
		parameters.annotations = annotations;
	}
}