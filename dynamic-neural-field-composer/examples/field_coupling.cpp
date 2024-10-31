//#include "simulation/simulation.h"
//#include "application/application.h"
//#include "tools/logger.h"
//#include "exceptions/exception.h"
//#include "user_interface/main_window.h"

#include "visualization/visualization.h"

#include <iostream>

int main()
{
	try
	{
		dnf_composer::PlotDimensions dimensions = { 0, 100, -5, 9, 1.0 };
		dnf_composer::PlotAnnotations annotations;
		dnf_composer::PlotParameters parameters(dimensions, annotations);
		dnf_composer::Plot plot(parameters);

		std::vector<double> data = { 1.0, 2.0, 3.0, 4.0, 5.0 };

		plot.addPlottingData(&data);

		data = { 6.0, 7.0, 8.0, 9.0, 10.0 };




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

//int main(int argc, char* argv[])
//{
//	try
//	{
//		const std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>("field coupling example sim", 1.0, 0.0, 0.0);
//		const dnf_composer::Application app{ simulation };
//
//		app.addWindow<dnf_composer::user_interface::MainWindow>();
//		app.addWindow<imgui_kit::LogWindow>();
//
//		app.init();
//
//		bool userRequestClose = false;
//		while (!userRequestClose)
//		{
//			app.step();
//			userRequestClose = app.hasUIBeenClosed();
//		}
//		app.close();
//		return 0;
//	}
//	catch (const dnf_composer::Exception& ex)
//	{
//		const std::string errorMessage = "Exception: " + std::string(ex.what()) + " ErrorCode: " + std::to_string(static_cast<int>(ex.getErrorCode())) + ". ";
//		log(dnf_composer::tools::logger::LogLevel::FATAL, errorMessage, dnf_composer::tools::logger::LogOutputMode::CONSOLE);
//		return static_cast<int>(ex.getErrorCode());
//	}
//	catch (const std::exception& ex)
//	{
//		log(dnf_composer::tools::logger::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". ", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
//		return 1;
//	}
//	catch (...)
//	{
//		log(dnf_composer::tools::logger::LogLevel::FATAL, "Unknown exception occurred. ", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
//		return 1;
//	}
//}