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
			const auto singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
			allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
		}

		plots[plotId].addPlottingData(allDataToPlotPtr);
	}

	void Visualization::plot(int plotId, const std::string& name, const std::string& component)
	{
		std::vector<std::pair<std::string, std::string>> dataVec = { {name, component} };
		plot(plotId, dataVec);
	}
}




//void Visualization::addPlots(const std::vector<Plot>& plots)
//{
//	for (const Plot& plot : plots)
//	{
//		addPlottingData(plot);
//	}
//}
//
//void Visualization::addPlot(const Plot& plot)
//{
//	for (const auto& indData : plot.data)
//	{
//		addPlottingData(indData.first, indData.second, plot.parameters);
//	}
//}
//
//void Visualization::addPlottingData(const std::string& elementName, const std::string& componentId,
//	const PlotParameters& parameters)
//{
//	if (elementName.empty() || componentId.empty())
//	{
//		const std::string message = "Tried to add an invalid element component '" + elementName + "' - '" + componentId + "' to the plot.";
//		log(tools::logger::LogLevel::WARNING, message);
//		return;
//	}
//
//	const auto data = simulation->getComponentPtr(elementName, componentId);
//	if (!data)
//	{
//		const std::string message = "Tried to add an invalid element component '" + elementName + "' - '" + componentId + "' to the plot.";
//		log(tools::logger::LogLevel::WARNING, message);
//	}
//
//	log(tools::logger::LogLevel::INFO, "Added element component '" + elementName + "' - '" + componentId +
//		"' to the plot.");
//}