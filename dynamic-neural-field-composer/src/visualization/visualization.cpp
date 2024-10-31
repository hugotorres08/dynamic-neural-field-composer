// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "visualization/visualization.h"

namespace dnf_composer
{
	Visualization::Visualization(std::shared_ptr<Simulation> simulation)
	{
		if (simulation == nullptr)
			throw Exception(ErrorCode::VIS_INVALID_SIM);

		simulation = std::move(simulation);
		plots = {};
	}	

	void Visualization::addPlot(const Plot& plot)
	{
		plots.emplace_back(plot);
	}

	void Visualization::plot(std::vector<double>* data)
	{
		Plot plot;
		plot.addPlottingData(data);
		addPlot(plot);
	}

	void Visualization::plot(std::vector<std::vector<double>*> data)
	{
		Plot plot;
		for (auto& d : data)
		{
			plot.addPlottingData(d);
		}
		addPlot(plot);
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