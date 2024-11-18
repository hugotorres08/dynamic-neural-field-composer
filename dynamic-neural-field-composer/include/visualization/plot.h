#pragma once
#include "plot_parameters.h"


namespace dnf_composer
{
	class Plot
	{
	protected:
		static inline int uniqueIdentifierCounter = 0;
		int uniqueIdentifier;
		PlotType type;
		PlotCommonParameters commonParameters;
		std::vector<std::vector<double>*> data;
	public:
		Plot(PlotCommonParameters parameters = PlotCommonParameters(),
			const std::vector<std::vector<double>*>& data = {},
			const std::vector<std::string>& legends = {});

		void addPlottingData(const std::vector<std::vector<double>*>& data, const std::vector<std::string>& legends = {});
		void addPlottingData(std::vector<double>* data, const std::string& legend = {});

		void removePlottingData(const std::vector<double>* data);

		int getUniqueIdentifier() const;
		PlotType getType() const;
		std::vector<std::vector<double>*> getData() const;
		PlotDimensions getDimensions() const;
		PlotAnnotations getAnnotations() const;

		void setDimensions(const PlotDimensions& dimensions);
		void setAnnotations(const PlotAnnotations& annotations);

		virtual std::string toString() const = 0;

		virtual void render() = 0;
	};
}
