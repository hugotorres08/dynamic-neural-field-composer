#include "application/application.h"
#include "user_interface/main_window.h"
#include "user_interface/field_metrics_window.h"
#include "user_interface/element_window.h"
#include "user_interface/plot_control_window.h"
#include "user_interface/simulation_window.h"
#include "elements/element_factory.h"


int main()
{
	try
	{
		using namespace dnf_composer;

		const auto simulation = std::make_shared<Simulation>("default", 1.0, 0.0, 0.0);
		const auto visualization = std::make_shared<Visualization>(simulation);
		const Application app{ simulation, visualization};

		app.addWindow<user_interface::MainWindow>();
		app.addWindow<imgui_kit::LogWindow>();
		app.addWindow<user_interface::FieldMetricsWindow>();
		app.addWindow<user_interface::ElementWindow>();
		app.addWindow<user_interface::SimulationWindow>();
		app.addWindow<user_interface::PlotControlWindow>();

		element::ElementFactory factory;
		const auto elem1 = factory.createElement(element::NEURAL_FIELD, element::ElementCommonParameters(), element::NeuralFieldParameters());
		const auto elem2 = factory.createElement(element::GAUSS_STIMULUS, element::ElementCommonParameters(), element::GaussStimulusParameters());
		const auto elem3 = factory.createElement(element::GAUSS_KERNEL, element::ElementCommonParameters(), element::GaussKernelParameters());

		simulation->addElement(elem1);
		simulation->addElement(elem2);
		simulation->addElement(elem3);

		visualization->plot({ { elem1->getUniqueName(), "output"}, {elem2->getUniqueName(), "output"} });
		visualization->plot({ { elem1->getUniqueName(), "output"}, {elem2->getUniqueName(), "output"} });


		/*const PlotCommonParameters plotParams{PlotType::LINE_PLOT, {0, 31, 0, 1, 1.0}, {"Gauss kernel", "Space", "Amplitude"}};
		const LinePlotParameters linePlotParams{ 5.0f, true };
		visualization->plot(plotParams, linePlotParams, elem3->getUniqueName(), "kernel");*/


		element::ElementDimensions esdp = { 100, 1.0 };
		element::ElementCommonParameters ecp = { "gc", esdp };
		element::GaussCoupling gc1{ 5, 10, 2, 4 };
		//element::GaussFieldCouplingParameters gfcp = { true, false, {gc1} };
		/*const auto elem4 = std::make_shared<element::GaussFieldCoupling>(ecp, gfcp);
		simulation->addElement(elem4);
		const auto elem5 = factory.createElement(element::NEURAL_FIELD, element::ElementCommonParameters(), element::NeuralFieldParameters());
		simulation->addElement(elem5);
		elem4->addInput(elem5, "output");*/
		//visualization->plot(PlotCommonParameters{ PlotType::HEATMAP, {0, 100, 0, 100, 1.0}, {"x", "y", "title"} }, HeatmapParameters{ 0.0, 0.5 }, { { elem4->getUniqueName(), "kernel"} });

		app.init();

		while (!app.hasGUIBeenClosed())
		{
			app.step();
		}

		app.close();
	}
	catch (const dnf_composer::Exception& ex)
	{
		const std::string errorMessage = "Exception: " + std::string(ex.what()) + " ErrorCode: " + std::to_string(static_cast<int>(ex.getErrorCode())) + ". ";
		log(dnf_composer::tools::logger::LogLevel::FATAL, errorMessage, dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(dnf_composer::tools::logger::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". ", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(dnf_composer::tools::logger::LogLevel::FATAL, "Unknown exception occurred. ", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
}