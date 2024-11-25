#include "application/application.h"
#include "user_interface/main_window.h"
#include "user_interface/field_metrics_window.h"
#include "user_interface/element_window.h"
#include "user_interface/plot_control_window.h"
#include "user_interface/simulation_window.h"
#include "elements/element_factory.h"
#include "user_interface/plots_window.h"


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
		app.addWindow<user_interface::PlotsWindow>();

		element::ElementFactory factory;
		const auto elem1 = factory.createElement(element::NEURAL_FIELD);
		const auto elem2 = factory.createElement(element::GAUSS_STIMULUS);
		const auto elem3 = factory.createElement(element::GAUSS_KERNEL);

		const auto elem4 = factory.createElement(element::NEURAL_FIELD);
		const auto elem5 = factory.createElement(element::GAUSS_STIMULUS);
		const auto elem6 = factory.createElement(element::GAUSS_KERNEL);

		simulation->addElement(elem1);
		simulation->addElement(elem2);
		simulation->addElement(elem3);
		simulation->addElement(elem4);
		simulation->addElement(elem5);
		simulation->addElement(elem6);

		elem1->addInput(elem2);
		elem1->addInput(elem3);
		elem3->addInput(elem1);

		elem4->addInput(elem5);
		elem4->addInput(elem6);
		elem6->addInput(elem4);

		visualization->plot({ { elem1->getUniqueName(), "output"}, {elem1->getUniqueName(), "activation"} });
		visualization->plot( elem3->getUniqueName(), "output");

		visualization->plot(
			PlotCommonParameters{
				PlotType::LINE_PLOT,
				PlotDimensions{0.0, 100.0, -10.0, 15.0, 1.0, 1.0},
				PlotAnnotations{"This is a title", "x", "y"}
			}, 
			LinePlotParameters{5.0, false},
			{ { elem4->getUniqueName(), "output"}, {elem4->getUniqueName(), "activation"}
			}
		);

		visualization->plot(
			PlotCommonParameters{
				PlotType::LINE_PLOT,
				PlotDimensions{0.0, 100.0, -10.0, 15.0, 1.0, 1.0},
				PlotAnnotations{"This is another title", "x", "y"}
			},
			LinePlotParameters{ 5.0, false },
			elem6->getUniqueName(), "output"
		);

		visualization->plot(0, { { elem2->getUniqueName(), "output"} });
		visualization->plot(2, elem5->getUniqueName(), "output");

		visualization->removePlot(1);
		visualization->removePlottingDataFromPlot(0, { elem1->getUniqueName(), "output" });

		visualization->removeAllPlots();

		visualization->plot(PlotType::LINE_PLOT);
		visualization->plot(PlotType::HEATMAP);

		const auto elem7 = factory.createElement(element::GAUSS_FIELD_COUPLING,
			element::ElementCommonParameters{element::GAUSS_FIELD_COUPLING},
			element::GaussFieldCouplingParameters{ element::ElementDimensions{100, 1.0},
			true, false, {{50.0, 50.0, 5, 5}}});

		simulation->addElement(elem7);

		elem7->addInput(elem1);
		elem4->addInput(elem7);

		visualization->plot({ { elem1->getUniqueName(), "activation"}, {elem1->getUniqueName(), "output"} });
		visualization->plot(4, { {elem4->getUniqueName(), "output"} });

		visualization->plot(5, elem7->getUniqueName(), "weights");

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
