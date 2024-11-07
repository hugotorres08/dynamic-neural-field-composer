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
		plots.emplace_back(parameters, allDataToPlotPtr);
		log(tools::logger::LogLevel::INFO, "Plot " + std::to_string(plots.size() - 1) + " added to visualization.");
	}

	void Visualization::plot(const std::string& name, const std::string& component)
	{
		std::vector<std::pair<std::string, std::string>> data = { {name, component} };
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

		plots.emplace_back(parameters, allDataToPlotPtr);
		log(tools::logger::LogLevel::INFO, "Plot " + std::to_string(plots.size() - 1) + " added to visualization.");
	}

	void Visualization::plot(const PlotParameters& parameters, const std::string& name, const std::string& component)
	{
		std::vector<std::pair<std::string, std::string>> dataVec = { {name, component} };
		plot(parameters, dataVec);
	}

	void Visualization::plot(int plotId, const std::vector<std::pair<std::string, std::string>>& data)
	{
		if (plotId < 0 || plotId >= plots.size())
			throw Exception(ErrorCode::VIS_INVALID_PLOTTING_INDEX);

		std::vector<std::vector<double>*> allDataToPlotPtr;

		for (const auto& d : data)
		{
			std::vector<double>* singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
			allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
		}

		for (auto& plot : plots)
		{
			if (plot.getUniqueIdentifier() == plotId)
				plot.addPlottingData(allDataToPlotPtr);
		}
	}

	void Visualization::plot(int plotId, const std::string& name, const std::string& component)
	{
		std::vector<std::pair<std::string, std::string>> dataVec = { {name, component} };
		plot(plotId, dataVec);
	}

	void Visualization::changePlotParameters(int plotId, const PlotParameters& parameters)
	{
		if (plotId < 0 || plotId >= plots.size())
			throw Exception(ErrorCode::VIS_INVALID_PLOTTING_INDEX);

		for (auto plot : plots)
		{
			if (plot.getUniqueIdentifier() == plotId)
				plot.setParameters(parameters);
		}
	}

	void Visualization::removePlot(int plotId)
	{
		if (plotId < 0 || plotId >= plots.size())
			throw Exception(ErrorCode::VIS_INVALID_PLOTTING_INDEX);

		plots.erase(plots.begin() + plotId);
		log(tools::logger::LogLevel::INFO, "Plot " + std::to_string(plotId) + " removed from visualization.");
	}

	void Visualization::removeAllPlots()
	{
		plots.clear();
		log(tools::logger::LogLevel::INFO, "All plots removed from visualization.");
	}

	void Visualization::removePlottingDataFromPlot(int plotId, const std::pair<std::string, std::string>& data)
	{
		if (plotId < 0 || plotId >= plots.size())
			throw Exception(ErrorCode::VIS_INVALID_PLOTTING_INDEX);

		const auto dataPtr = simulation->getComponentPtr(data.first, data.second);
		for (auto& plot : plots)
		{
			if (plot.getUniqueIdentifier() == plotId)
			{
				const size_t initialSize = plot.getData().size();
				plot.removePlottingData(dataPtr);
				if (initialSize == plot.getData().size())
				{
					log(tools::logger::LogLevel::WARNING, "Data '" + data.first + " - " + data.second + "' not found in plot " + std::to_string(plotId) + ".");
					return;
				}
				log(tools::logger::LogLevel::INFO, "Data '" + data.first + " - " + data.second + "' removed from plot " + std::to_string(plotId) + ".");
				return;
			}
		}
		log(tools::logger::LogLevel::WARNING, "Data '" + data.first + " - " + data.second + "' not found in plot " + std::to_string(plotId) + ".");
	}
}