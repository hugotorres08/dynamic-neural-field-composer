// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "visualization/visualization.h"

namespace dnf_composer
{
	Visualization::Visualization(const std::shared_ptr<Simulation>& simulation)
	{
		if (simulation == nullptr)
			throw Exception(ErrorCode::VIS_INVALID_SIM);

		this->simulation = simulation;
		plots = {};
		log(tools::logger::LogLevel::INFO, "Visualization object created.");
	}

	void Visualization::plot(const std::vector<std::pair<std::string, std::string>>& data)
	{
		std::vector<std::vector<double>*> allDataToPlotPtr;

		for (const auto& d : data)
		{
			const auto singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
			allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
		}

		PlotParameters parameters;
		std::vector<std::string> legends;
		for (const auto& d : data)
		{
			legends.emplace_back(d.first + " - " + d.second);
		}
		plots.emplace_back(parameters, allDataToPlotPtr, legends);
		log(tools::logger::LogLevel::INFO, "Plot " + std::to_string(plots.size() - 1) + " added to visualization.");
	}

	void Visualization::plot(const std::string& name, const std::string& component)
	{
		const std::vector<std::pair<std::string, std::string>> data = { {name, component} };
		plot(data);
	}

	void Visualization::plot(const PlotParameters& parameters, const std::vector<std::pair<std::string, std::string>>& data)
	{
		std::vector<std::vector<double>*> allDataToPlotPtr;

		for (const auto& d : data)
		{
			const auto singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
			allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
		}

		std::vector<std::string> legends;
		for (const auto& d : data)
		{
			legends.emplace_back(d.first + " - " + d.second);
		}
		plots.emplace_back(parameters, allDataToPlotPtr, legends);
		log(tools::logger::LogLevel::INFO, "Plot " + std::to_string(plots.size() - 1) + " added to visualization.");
	}

	void Visualization::plot(const PlotParameters& parameters, const std::string& name, const std::string& component)
	{
		const std::vector<std::pair<std::string, std::string>> dataVec = { {name, component} };
		plot(parameters, dataVec);
	}

	void Visualization::plot(int plotId, const std::vector<std::pair<std::string, std::string>>& data)
	{
		// Find the plot with the specified unique identifier
		const auto it = std::find_if(plots.begin(), plots.end(), [plotId](const Plot& plot) {
			return plot.getUniqueIdentifier() == plotId;
			});

		// Check if the plot was found
		if (it == plots.end()) {
			log(tools::logger::LogLevel::ERROR, "Plot with ID " + std::to_string(plotId) + " not found.");
			return;
		}

		// Collect data to plot
		std::vector<std::vector<double>*> allDataToPlotPtr;
		std::vector<std::string> legends;
		for (const auto& d : data)
		{
			std::vector<double>* singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
			allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
			legends.emplace_back(d.first + " - " + d.second);
		}

		// Add data to the found plot
		it->addPlottingData(allDataToPlotPtr, legends);
		log(tools::logger::LogLevel::INFO, "Data plotted on plot with ID " + std::to_string(plotId) + ".");
	}

	void Visualization::plot(int plotId, const std::string& name, const std::string& component)
	{
		const std::vector<std::pair<std::string, std::string>> dataVec = { {name, component} };
		plot(plotId, dataVec);
	}

	void Visualization::changePlotParameters(int plotId, const PlotParameters& parameters)
	{
		// Find the plot with the specified unique identifier
		const auto it = std::find_if(plots.begin(), plots.end(), [plotId](const Plot& plot) {
			return plot.getUniqueIdentifier() == plotId;
			});

		// Check if the plot was found
		if (it == plots.end()) {
			log(tools::logger::LogLevel::ERROR, "Plot with ID " + std::to_string(plotId) + " not found.");
			return;
		}

		// Update the plot parameters
		it->setParameters(parameters);
		log(tools::logger::LogLevel::INFO, "Parameters updated for plot with ID " + std::to_string(plotId) + ".");
	}

	void Visualization::removePlot(int plotId)
	{
		// Find the plot with the specified unique identifier
		const auto it = std::find_if(plots.begin(), plots.end(), [plotId](const Plot& plot) {
			return plot.getUniqueIdentifier() == plotId;
			});

		if (it != plots.end()) {
			// If found, remove the plot
			plots.erase(it);
			log(tools::logger::LogLevel::INFO, "Plot with ID " + std::to_string(plotId) + " removed from visualization.");
		}
		else {
			// If not found, log an error
			log(tools::logger::LogLevel::ERROR, "Plot with ID " + std::to_string(plotId) + " not found.");
		}
	}

	void Visualization::removeAllPlots()
	{
		plots.clear();
		log(tools::logger::LogLevel::INFO, "All plots removed from visualization.");
	}

	void Visualization::removePlottingDataFromPlot(int plotId, const std::pair<std::string, std::string>& data)
	{
		// Find the plot with the specified unique identifier
		auto it = std::find_if(plots.begin(), plots.end(), [plotId](const Plot& plot) {
			return plot.getUniqueIdentifier() == plotId;
			});

		// Check if the plot was found
		if (it == plots.end()) {
			log(tools::logger::LogLevel::ERROR, "Plot with ID " + std::to_string(plotId) + " not found.");
			return;
		}

		// Get the data pointer
		const auto dataPtr = simulation->getComponentPtr(data.first, data.second);

		// Get the initial size of the plot's data
		const size_t initialSize = it->getData().size();

		// Attempt to remove the data
		it->removePlottingData(dataPtr);

		// Check if the size of the data changed
		if (initialSize == it->getData().size()) {
			log(tools::logger::LogLevel::WARNING, "Data '" + data.first + " - " + data.second + "' not found in plot " + std::to_string(plotId) + ".");
		}
		else {
			log(tools::logger::LogLevel::INFO, "Data '" + data.first + " - " + data.second + "' removed from plot " + std::to_string(plotId) + ".");
		}
	}
}