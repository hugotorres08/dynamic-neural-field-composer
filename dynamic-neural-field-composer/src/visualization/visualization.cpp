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
		/*std::vector<std::vector<double>*> allDataToPlotPtr;
		allDataToPlotPtr.reserve(data.size());
		for (const auto& d : data)
		{
			const auto singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
			allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
		}*/

		PlotCommonParameters parameters;
		LinePlotParameters linePlotParameters; 
		/*std::vector<std::string> legends;
		legends.reserve(data.size());
		for (const auto& d : data)
		{
			legends.emplace_back(d.first + " - " + d.second);
		}*/
		//plots.emplace_back(std::make_shared<LinePlot>(parameters, linePlotParameters, allDataToPlotPtr, legends));
		//LinePlot plot(parameters, linePlotParameters);
		//plots[plot] = data;
		LinePlot plot(parameters, linePlotParameters);
		plots[std::make_shared<LinePlot>(plot)] = data;
		log(tools::logger::LogLevel::INFO, "Plot " + std::to_string(plots.size() - 1) + " added to visualization.");
	}

	void Visualization::plot(const std::string& name, const std::string& component)
	{
		const std::vector<std::pair<std::string, std::string>> data = { {name, component} };
		plot(data);
	}

	void Visualization::plot(const PlotCommonParameters& parameters, const PlotSpecificParameters& specificParameters, const std::vector<std::pair<std::string, std::string>>& data)
	{
		/*std::vector<std::vector<double>*> allDataToPlotPtr;
		allDataToPlotPtr.reserve(data.size());
		for (const auto& d : data)
		{
			const auto singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
			allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
		}*/

		/*std::vector<std::string> legends;
		legends.reserve(data.size());
		for (const auto& d : data)
		{
			legends.emplace_back(d.first + " - " + d.second);
		}*/

		switch (parameters.type)
		{
			case PlotType::LINE_PLOT:
			{
				const auto linePlotParameters = dynamic_cast<const LinePlotParameters*>(&specificParameters);
					LinePlot plot(parameters, *linePlotParameters);
					plots[std::make_shared<LinePlot>(plot)] = data;
					//plots.emplace_back(std::make_shared<LinePlot>(parameters, *linePlotParameters, allDataToPlotPtr, legends));
				break;
			}
			case PlotType::HEATMAP:
			{
				const auto heatmapParameters = dynamic_cast<const HeatmapParameters*>(&specificParameters);
					Heatmap plot(parameters, *heatmapParameters);
					plots[std::make_shared<Heatmap>(plot)] = data;
					//plots.emplace_back(std::make_shared<Heatmap>(parameters, *heatmapParameters, allDataToPlotPtr, legends));
				break;
			}
		}
		log(tools::logger::LogLevel::INFO, "Plot " + std::to_string(plots.size() - 1) + " added to visualization.");
	}

	void Visualization::plot(const PlotCommonParameters& parameters, const PlotSpecificParameters& specificParameters, const std::string& name, const std::string& component)
	{
		const std::vector<std::pair<std::string, std::string>> dataVec = { {name, component} };
		plot(parameters, specificParameters, dataVec);
	}

	void Visualization::plot(int plotId, const std::vector<std::pair<std::string, std::string>>& data)
	{
		//// Find the plot with the specified unique identifier
		//const auto it = std::find_if(plots.begin(), plots.end(), [plotId](const std::shared_ptr<Plot>& plot) {
		//	return plot->getUniqueIdentifier() == plotId;
		//	});

		//// Check if the plot was found
		//if (it == plots.end()) {
		//	log(tools::logger::LogLevel::ERROR, "Plot with ID " + std::to_string(plotId) + " not found.");
		//	return;
		//}

		// Collect data to plot
		/*std::vector<std::vector<double>*> allDataToPlotPtr;
		allDataToPlotPtr.reserve(data.size());
		std::vector<std::string> legends;
		legends.reserve(data.size());
		for (const auto& d : data)
		{
			std::vector<double>* singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
			allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
			legends.emplace_back(d.first + " - " + d.second);
		}*/

		// Add data to the found plot
		//(*it)->addPlottingData(allDataToPlotPtr, legends);
		
		//log(tools::logger::LogLevel::INFO, "Data plotted on plot with ID " + std::to_string(plotId) + ".");
	}

	void Visualization::plot(int plotId, const std::string& name, const std::string& component)
	{
		//const std::vector<std::pair<std::string, std::string>> dataVec = { {name, component} };
		//plot(plotId, dataVec);
	}

	//void Visualization::changePlotParameters(int plotId, const PlotParameters& parameters)
	//{
	//	// Find the plot with the specified unique identifier
	//	const auto it = std::find_if(plots.begin(), plots.end(), [plotId](const Plot& plot) {
	//		return plot.getUniqueIdentifier() == plotId;
	//		});

	//	// Check if the plot was found
	//	if (it == plots.end()) {
	//		log(tools::logger::LogLevel::ERROR, "Plot with ID " + std::to_string(plotId) + " not found.");
	//		return;
	//	}

	//	// Update the plot parameters
	//	it->setParameters(parameters);
	//	log(tools::logger::LogLevel::INFO, "Parameters updated for plot with ID " + std::to_string(plotId) + ".");
	//}

	void Visualization::removePlot(int plotId)
	{
		//// Find the plot with the specified unique identifier
		//const auto it = std::find_if(plots.begin(), plots.end(), [plotId](const std::shared_ptr<Plot>& plot) {
		//	return plot->getUniqueIdentifier() == plotId;
		//	});

		//if (it != plots.end()) {
		//	// If found, remove the plot
		//	plots.erase(it);
		//	log(tools::logger::LogLevel::INFO, "Plot with ID " + std::to_string(plotId) + " removed from visualization.");
		//}
		//else {
		//	// If not found, log an error
		//	log(tools::logger::LogLevel::ERROR, "Plot with ID " + std::to_string(plotId) + " not found.");
		//}
	}

	void Visualization::removeAllPlots()
	{
		plots.clear();
		log(tools::logger::LogLevel::INFO, "All plots removed from visualization.");
	}

	void Visualization::removePlottingDataFromPlot(int plotId, const std::pair<std::string, std::string>& data)
	{
		//// Find the plot with the specified unique identifier
		//const auto it = std::find_if(plots.begin(), plots.end(), [plotId](const std::shared_ptr<Plot>& plot) {
		//	return plot->getUniqueIdentifier() == plotId;
		//	});

		//// Check if the plot was found
		//if (it == plots.end()) {
		//	log(tools::logger::LogLevel::ERROR, "Plot with ID " + std::to_string(plotId) + " not found.");
		//	return;
		//}

		//// Get the data pointer
		//const auto dataPtr = simulation->getComponentPtr(data.first, data.second);

		//// Get the initial size of the plot's data
		//const size_t initialSize = (*it)->getData().size();

		//// Attempt to remove the data
		//(*it)->removePlottingData(dataPtr);

		//// Check if the size of the data changed
		//if (initialSize == (*it)->getData().size()) {
		//	log(tools::logger::LogLevel::WARNING, "Data '" + data.first + " - " + data.second + "' not found in plot " + std::to_string(plotId) + ".");
		//}
		//else {
		//	log(tools::logger::LogLevel::INFO, "Data '" + data.first + " - " + data.second + "' removed from plot " + std::to_string(plotId) + ".");
		//}
	}

	void Visualization::render()
	{
		for (auto& entry : plots) 
		{
			//Plot& key = const_cast<Plot&>(entry.first); // Access the key with const_cast
			std::vector<std::pair<std::string, std::string>> data = entry.second;

			std::vector<std::vector<double>*> allDataToPlotPtr;
			allDataToPlotPtr.reserve(data.size());
			for (const auto& d : data)
			{
				const auto singleDataToPlotPtr = simulation->getComponentPtr(d.first, d.second);
				allDataToPlotPtr.emplace_back(singleDataToPlotPtr);
			}

			std::vector<std::string> legends;
			legends.reserve(data.size());
			for (const auto& d : data)
			{
				legends.emplace_back(d.first + " - " + d.second);
			}

			const int plotID = entry.first->getUniqueIdentifier();
			const std::string plotWindowTitle = "Plot #" + std::to_string(plotID);
			bool open = true;

			if (ImGui::Begin(plotWindowTitle.c_str(), &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
			{
				entry.first->render(allDataToPlotPtr, legends);
			}
			ImGui::End();

			//if (!open)
				//visualization->removePlot(plotID);
		}

	}
}
