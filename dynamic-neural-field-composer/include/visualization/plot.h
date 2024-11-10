#pragma once

#include <vector>
#include <exception>
#include <stdexcept>
#include <string>
#include <algorithm>

#include "tools/logger.h"

namespace dnf_composer
{
	struct PlotDimensions
	{
		int xMin, xMax, yMin, yMax;
		double dx;

		PlotDimensions();
		PlotDimensions(int xMin, int xMax, int yMin, int yMax, double dx);
		bool areUndefined() const;
		std::string toString() const;
		bool operator==(const PlotDimensions& other) const;
	};

	struct PlotAnnotations
	{
		std::string title, x_label, y_label;
		std::vector<std::string> legends;

		PlotAnnotations();
		PlotAnnotations(std::string title, std::string x_label, std::string y_label);
		std::string toString() const;
		bool operator==(const PlotAnnotations& other) const;
	};

	struct PlotParameters
	{
		PlotDimensions dimensions;
		PlotAnnotations annotations;

		PlotParameters();
		PlotParameters(const PlotDimensions& dimensions, PlotAnnotations annotations);
		std::string toString() const;
		bool operator==(const PlotParameters& other) const;
	};

	class Plot
	{
		static inline int uniqueIdentifierCounter = 0;
		int uniqueIdentifier;
		PlotParameters parameters;
		std::vector<std::vector<double>*> data;
	public:
		Plot(const PlotParameters& parameters = PlotParameters(), 
			const std::vector<std::vector<double>*>& data = {},
			const std::vector<std::string>& legends = {});
		void addPlottingData(const std::vector<std::vector<double>*>& data, const std::vector<std::string>& legends = {});
		void addPlottingData(std::vector<double>* data, const std::string& legend = {});
		void removePlottingData(std::vector<double>* data);
		void setParameters(const PlotParameters& parameters);
		int getUniqueIdentifier() const;
		PlotParameters getParameters() const;
		std::vector<std::vector<double>*> getData() const;
		std::string toString() const;
	};
}