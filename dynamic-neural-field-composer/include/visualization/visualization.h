#pragma once

#include <iostream>
#include <vector>

#include "simulation/simulation.h"
#include "exceptions/exception.h"

namespace dnf_composer
{
	struct PlotDimensions
	{
		int xMin, xMax, yMin, yMax;
		double dx;

		PlotDimensions()
			: xMin(0), xMax(100), yMin(-5), yMax(15), dx(1.0)
		{}

		PlotDimensions(int xMin, int xMax, int yMin, int yMax, double dx)
			: xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), dx(dx)
		{}

		bool isNull() const
		{
			return xMin == 0 && xMax == 0 && yMin == 0 && yMax == 0 && dx == 0.0;
		}
	};

	struct PlotAnnotations
	{
		std::string title, x_label, y_label;

		PlotAnnotations()
			:title(), x_label(), y_label()
		{}

		PlotAnnotations(std::string title, std::string x_label, std::string y_label)
			:title(std::move(title)), x_label(std::move(x_label)), y_label(std::move(y_label))
		{}
	};

	struct PlotParameters
	{
		PlotDimensions dimensions;
		PlotAnnotations annotations;

		PlotParameters()
			: dimensions(), annotations()
		{}

		PlotParameters(const PlotDimensions& dimensions, PlotAnnotations annotations)
			: dimensions(dimensions), annotations(std::move(annotations))
		{}
	};


	struct Plot
	{
		static inline int uniqueIdentifierCounter = 0;
		int uniqueIdentifier;
		PlotParameters parameters;
		std::vector<std::pair<std::string, std::string>> data;

		Plot()
			: uniqueIdentifier(uniqueIdentifierCounter++), parameters(), data()
		{}

		Plot(const std::string& elementName, const std::string& componentId> )
			: uniqueIdentifier(uniqueIdentifierCounter++), parameters(), 
			data({ std::make_pair(elementName, componentId) })
		{}

		Plot(const std::vector<std::pair<std::string, std::string>>& data)
			: uniqueIdentifier(uniqueIdentifierCounter++), parameters()
		{
			for (const auto& [elementName, componentId] : data)
			{
				if(doesDataAlreadyExistInThePlottingVector(elementName, componentId))
				{
					log(tools::logger::LogLevel::ERROR, "Data already exists in the plotting vector.");
					return;
				}
				else
				{
					this->data.push_back(std::make_pair(elementName, componentId));
				}
			}
		}

		Plot(const PlotParameters& parameters, const std::vector<std::pair<std::string, std::string>> data)
			: uniqueIdentifier(uniqueIdentifierCounter++), parameters(parameters)
		{}

		bool doesDataAlreadyExistInThePlottingVector(const std::string& elementName, const std::string& componentId)
		{
			if (std::find(this->data.begin(), this->data.end(), std::make_pair(elementName, componentId)) != this->data.end())
			{
				return true;
			}
			return false;
		}
	};

	class Visualization
	{
	private:
		std::shared_ptr<Simulation> simulation;
		std::vector<Plot> plots;
	public:
		Visualization(std::shared_ptr<Simulation> simulation);
		Visualization(std::shared_ptr<Simulation> simulation, const Plot& plot);
		Visualization(std::shared_ptr<Simulation> simulation, const std::vector<Plot> plots);

		void addPlots(const std::vector<Plot>& plots);
		void addPlot(const Plot& plot);
		void addPlottingDataToPlot(const std::string& elementName, const std::string& componentId, 
			const PlotParameters& parameters = PlotParameters());

	};
}

