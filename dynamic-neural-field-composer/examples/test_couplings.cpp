#include "application/application.h"
#include "user_interface/main_window.h"
#include "user_interface/field_metrics_window.h"
#include "user_interface/element_window.h"
#include "user_interface/plot_control_window.h"
#include "user_interface/simulation_window.h"
#include "elements/element_factory.h"
#include "wizards/learning_wizard.h"


int main()
{
	try
	{
		using namespace dnf_composer;

		const auto simulation = std::make_shared<Simulation>("default", 25.0, 0.0, 0.0);
		const auto visualization = std::make_shared<Visualization>(simulation);
		const Application app{ simulation, visualization };

		app.addWindow<user_interface::MainWindow>();
		app.addWindow<imgui_kit::LogWindow>();
		app.addWindow<user_interface::FieldMetricsWindow>();
		app.addWindow<user_interface::ElementWindow>();
		app.addWindow<user_interface::SimulationWindow>();
		app.addWindow<user_interface::PlotControlWindow>();

		element::ElementFactory factory;
		const element::ElementDimensions input_dimensions{200, 0.7};
		const auto nf_1 = factory.createElement(element::ElementLabel::NEURAL_FIELD,
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::NEURAL_FIELD}, input_dimensions},
			element::NeuralFieldParameters{});
		const auto gk_1 = factory.createElement(element::GAUSS_KERNEL,
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::GAUSS_KERNEL}, input_dimensions},
			element::GaussKernelParameters{});
		const auto nn_1 = factory.createElement(element::NORMAL_NOISE,
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::NORMAL_NOISE}, input_dimensions},
			element::NormalNoiseParameters{});
		const auto gs_1 = factory.createElement(element::GAUSS_STIMULUS,
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::GAUSS_STIMULUS}, input_dimensions},
			element::GaussStimulusParameters{});

		const element::ElementDimensions output_dimensions{100, 1.0};
		const auto nf_2 = factory.createElement(element::NEURAL_FIELD,
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::NEURAL_FIELD}, output_dimensions},
			element::NeuralFieldParameters{});
		const auto mhk_2 = factory.createElement(element::MEXICAN_HAT_KERNEL,
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::MEXICAN_HAT_KERNEL}, output_dimensions},
			element::MexicanHatKernelParameters{});
		const auto nn_2 = factory.createElement(element::NORMAL_NOISE,
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::NORMAL_NOISE}, output_dimensions},
			element::NormalNoiseParameters{});
		const auto gs_2 = factory.createElement(element::GAUSS_STIMULUS,
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::GAUSS_STIMULUS}, output_dimensions},
			element::GaussStimulusParameters{});
		//const auto fc_1 = factory.createElement(element::FIELD_COUPLING,
			//element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::FIELD_COUPLING}, output_dimensions},
			//element::FieldCouplingParameters{input_dimensions});
		const auto gfc_1 = factory.createElement(element::GAUSS_FIELD_COUPLING, 
			element::ElementCommonParameters{element::ElementIdentifiers{element::ElementLabel::GAUSS_FIELD_COUPLING}, output_dimensions},
			element::GaussFieldCouplingParameters{input_dimensions,true, false, {{50.0, 50.0, 5.0, 5.0}}});

		simulation->addElement(nf_1);
		simulation->addElement(gk_1);
		simulation->addElement(nn_1);
		simulation->addElement(nf_2);
		simulation->addElement(mhk_2);
		simulation->addElement(nn_2);
		simulation->addElement(gs_1);
		simulation->addElement(gs_2);
		//simulation->addElement(fc_1);
		simulation->addElement(gfc_1);

		nf_1->addInput(gk_1);
		gk_1->addInput(nf_1);
		nf_1->addInput(nn_1);
		nf_1->addInput(gs_1);

		nf_2->addInput(mhk_2);
		mhk_2->addInput(nf_2);
		nf_2->addInput(nn_2);
		nf_2->addInput(gs_2);

		//fc_1->addInput(nf_1);
		//nf_2->addInput(fc_1);
		gfc_1->addInput(nf_1);
		nf_2->addInput(gfc_1);

		visualization->plot(
			PlotCommonParameters{
			PlotType::LINE_PLOT,
			PlotDimensions{ 0.0, 200, -20.0, 20, 0.7, 1.0},
			PlotAnnotations{ "Input neural field", "Spatial dimension", "Amplitude" } },
			LinePlotParameters{},
			{ { nf_1->getUniqueName(), "activation" }, { nf_1->getUniqueName(), "output" }, { nf_1->getUniqueName(), "input" } });

		visualization->plot(
			PlotCommonParameters{
			PlotType::LINE_PLOT,
			PlotDimensions{ 0.0, 100, -20.0, 20, 1.0, 1.0 },
			PlotAnnotations{ "Output neural field", "Spatial dimension", "Amplitude" } },
			LinePlotParameters{},
			{ {nf_2->getUniqueName(), "activation"}, {nf_2->getUniqueName(), "output"}, {nf_2->getUniqueName(), "input"} });

		visualization->plot(
			PlotCommonParameters{
				PlotType::HEATMAP, 
				PlotDimensions{0.0, 100, 0.0, 200.0, 1.0, 0.7}, 
				PlotAnnotations{"Field coupling", "Output field dimension", "Input field dimension"} }, 
				HeatmapParameters{},
			{ {gfc_1->getUniqueName(), "weights"} }
		);

		visualization->plot(
			PlotCommonParameters{
			PlotType::LINE_PLOT,
			PlotDimensions{ 0.0, 100, -20.0, 20, 1.0, 1.0},
			PlotAnnotations{ "Field coupling output", "Spatial dimension", "Amplitude" } },
			LinePlotParameters{},
			{{gfc_1->getUniqueName(), "output"}});

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