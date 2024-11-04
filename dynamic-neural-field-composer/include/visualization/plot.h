#pragma once

#include <vector>
#include <exception>
#include <stdexcept>
#include <string>
#include <algorithm>

namespace dnf_composer
{
	struct PlotDimensions
	{
		int xMin, xMax, yMin, yMax;
		double dx;

		PlotDimensions();
		PlotDimensions(int xMin, int xMax, int yMin, int yMax, double dx);
		bool isNull() const;
	};

	struct PlotAnnotations
	{
		std::string title, x_label, y_label;

		PlotAnnotations();
		PlotAnnotations(std::string title, std::string x_label, std::string y_label);
	};

	struct PlotParameters
	{
		PlotDimensions dimensions;
		PlotAnnotations annotations;

		PlotParameters();
		PlotParameters(const PlotDimensions& dimensions, PlotAnnotations annotations);
	};

	class Plot
	{
		static inline int uniqueIdentifierCounter = 0;
		int uniqueIdentifier;
		PlotParameters parameters;
		std::vector<std::vector<double>*> data;
	public:
		Plot(const PlotParameters& parameters = PlotParameters(), 
			std::vector<std::vector<double>*> data = {});
		void addPlottingData(std::vector<std::vector<double>*> data);
		void addPlottingData(std::vector<double>* data);
		void removePlottingData(std::vector<double>* data);
		void setParameters(const PlotParameters& parameters);
		int getUniqueIdentifier() const;
		PlotParameters getParameters() const;
		std::vector<std::vector<double>*> getData() const;
	};
}