#include "visualization/visualization.h"

#include <iostream>

int main()
{
	try
	{
		dnf_composer::PlotDimensions dimensions = { 0, 100, -5, 9, 1.0 };
		dnf_composer::PlotAnnotations annotations;
		dnf_composer::PlotParameters parameters(dimensions, annotations);

		std::vector<double> data = { 1.0, 2.0, 3.0, 4.0, 5.0 };

		dnf_composer::Plot plot(parameters, { &data });

		plot.addPlottingData(&data);

		data = { 6.0, 7.0, 8.0, 9.0, 10.0 };

		plot.addPlottingData(&data);


		std::vector<double> data1 = { 1.0, 2.0, 3.0, 4.0, 5.0 };
		std::vector<double> data2 = { 6.0, 7.0, 8.0, 9.0, 10.0 };

		std::vector<std::vector<double>*> data3 = { &data1, &data2 };

		plot.addPlottingData(data3);

		dnf_composer::Plot plot2(parameters, data3);

		plot2.removePlottingData(&data1);

		return 0;
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception caught: " << ex.what() << ". " << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cout << "Unknown exception occurred. " << std::endl;
		return 1;
	}
	
}

