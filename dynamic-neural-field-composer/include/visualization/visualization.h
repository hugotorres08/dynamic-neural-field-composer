#pragma once

#include <iostream>
#include <vector>

#include "simulation/simulation.h"
#include "exceptions/exception.h"
#include "plot.h"

namespace dnf_composer
{
	class Visualization
	{
	private:
		std::shared_ptr<Simulation> simulation;
		std::vector<Plot> plots;
	public:
		Visualization(std::shared_ptr<Simulation> simulation);

		void plot(std::vector<double>* data);
		void plot(std::vector<std::vector<double>*> data);
		void plot(const std::pair<std::string, std::string>& elementComponentPair);
		void plot(const std::vector<std::pair<std::string, std::string>>& elementComponentPairs);
		void addPlottingDataToPlot(int PlotId, std::vector<double>* data);
		void addPlottingDataToPlot(int PlotId, const std::pair<std::string, std::string>& elementComponentPair, Plot& plot);
		void addPlottingDataToPlot(int PlotId, const std::vector<std::pair<std::string, std::string>>& elementComponentPairs, Plot& plot);
	
	private:
		void addPlot(const Plot& plot);
	};
}

