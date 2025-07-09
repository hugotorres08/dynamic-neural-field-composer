#include "application/application.h"
#include "user_interface/main_window.h"
#include "user_interface/field_metrics_window.h"
#include "user_interface/element_window.h"
#include "user_interface/plot_control_window.h"
#include "user_interface/simulation_window.h"
#include "elements/element_factory.h"
#include "user_interface/plots_window.h"
#include "user_interface/node_graph_window.h"

int main()
{
	try
	{
		using namespace dnf_composer;

		const auto simulation = std::make_shared<Simulation>("wrong piece learning", 1.0, 0.0, 0.0);
		const auto visualization = std::make_shared<Visualization>(simulation);
		const Application app{ simulation, visualization };

		app.addWindow<user_interface::MainWindow>();
		app.addWindow<imgui_kit::LogWindow>();
		app.addWindow<user_interface::FieldMetricsWindow>();
		app.addWindow<user_interface::ElementWindow>();
		app.addWindow<user_interface::SimulationWindow>();
		app.addWindow<user_interface::PlotControlWindow>();
		app.addWindow<user_interface::PlotsWindow>();
		app.addWindow<user_interface::NodeGraphWindow>();

		element::ElementFactory factory;
		const element::ElementDimensions input_dimensions{280, 1.0};
		const auto nf_1 = factory.createElement(element::ElementLabel::NEURAL_FIELD,
			element::ElementCommonParameters{element::ElementIdentifiers{"parts not needed field" }, input_dimensions},
			element::NeuralFieldParameters{});
		const auto mhk_1 = factory.createElement(element::MEXICAN_HAT_KERNEL,
			element::ElementCommonParameters{element::ElementIdentifiers{"kernel parts not needed field"}, input_dimensions},
			element::MexicanHatKernelParameters{});
		const auto nn_1 = factory.createElement(element::NORMAL_NOISE,
			element::ElementCommonParameters{element::ElementIdentifiers{"normal noise parts not needed field"}, input_dimensions},
			element::NormalNoiseParameters{0.05});
		const auto gs_1 = factory.createElement(element::GAUSS_STIMULUS,
			element::ElementCommonParameters{element::ElementIdentifiers{"stimulus parts not needed field"}, input_dimensions},
			element::GaussStimulusParameters{5, 15, 20});

		const element::ElementDimensions output_dimensions{280, 1.0};
		const auto nf_2 = factory.createElement(element::NEURAL_FIELD,
			element::ElementCommonParameters{element::ElementIdentifiers{"wrong piece field"}, output_dimensions},
			element::NeuralFieldParameters{});
		const auto gk_2 = factory.createElement(element::MEXICAN_HAT_KERNEL,
			element::ElementCommonParameters{element::ElementIdentifiers{"kernel wrong piece field"}, output_dimensions},
			element::MexicanHatKernelParameters{});
		const auto nn_2 = factory.createElement(element::NORMAL_NOISE,
			element::ElementCommonParameters{element::ElementIdentifiers{"normal noise wrong piece field"}, output_dimensions},
			element::NormalNoiseParameters{0.05});
		const auto gs_2 = factory.createElement(element::GAUSS_STIMULUS,
			element::ElementCommonParameters{element::ElementIdentifiers{"stimulus wrong piece field"}, output_dimensions},
			element::GaussStimulusParameters{5, 15, 40});
		const auto fc_1 = factory.createElement(element::FIELD_COUPLING,
			element::ElementCommonParameters{element::ElementIdentifiers{"coupling parts not needed - wrong piece"}, output_dimensions},
			element::FieldCouplingParameters{input_dimensions});

		//const auto nf_3 = factory.createElement(element::ElementLabel::NEURAL_FIELD,
			//element::ElementCommonParameters{ element::ElementIdentifiers{"stkl next field" }, input_dimensions },
			//element::NeuralFieldParameters{});
		//const auto mhk_3 = factory.createElement(element::MEXICAN_HAT_KERNEL,
			//element::ElementCommonParameters{ element::ElementIdentifiers{"kernel stkl next field"}, input_dimensions },
			//element::MexicanHatKernelParameters{});
		//const auto nn_3 = factory.createElement(element::NORMAL_NOISE,
			//element::ElementCommonParameters{ element::ElementIdentifiers{"normal noise stkl next field"}, input_dimensions },
			//element::NormalNoiseParameters{0.05});
		//const auto gs_3 = factory.createElement(element::GAUSS_STIMULUS,
			//element::ElementCommonParameters{ element::ElementIdentifiers{"stimulus stkl next field"}, input_dimensions },
			//element::GaussStimulusParameters{5, 15, 60});
		//const auto fc_2 = factory.createElement(element::FIELD_COUPLING,
			//element::ElementCommonParameters{ element::ElementIdentifiers{"coupling present-next"}, output_dimensions },
			//element::FieldCouplingParameters{ input_dimensions });

		simulation->addElement(nf_1);
		simulation->addElement(mhk_1);
		simulation->addElement(nn_1);

		simulation->addElement(nf_2);
		simulation->addElement(gk_2);
		simulation->addElement(nn_2);

		simulation->addElement(gs_1);
		simulation->addElement(gs_2);
		simulation->addElement(fc_1);

		//simulation->addElement(nf_3);
		//simulation->addElement(mhk_3);
		//simulation->addElement(nn_3);

		//simulation->addElement(gs_3);
		//simulation->addElement(fc_2);

		nf_1->addInput(mhk_1);
		mhk_1->addInput(nf_1);
		nf_1->addInput(nn_1);
		nf_1->addInput(gs_1);

		nf_2->addInput(gk_2);
		gk_2->addInput(nf_2);
		nf_2->addInput(nn_2);
		nf_2->addInput(gs_2);

		fc_1->addInput(nf_1);
		nf_2->addInput(fc_1);

		//nf_3->addInput(mhk_3);
		//mhk_3->addInput(nf_3);
		//nf_3->addInput(nn_3);
		//nf_3->addInput(gs_3);

		//fc_2->addInput(nf_2);
		//nf_3->addInput(fc_2);

		visualization->plot(
			PlotCommonParameters{
			PlotType::LINE_PLOT,
			PlotDimensions{ 0.0, 280, -20.0, 20, 1.0, 1.0},
			PlotAnnotations{ "parts not needed field dynamics", "Spatial location", "Amplitude" } },
			LinePlotParameters{},
			{ { nf_1->getUniqueName(), "activation" }, { nf_1->getUniqueName(), "output" }, { nf_1->getUniqueName(), "input" } });

		visualization->plot(
			PlotCommonParameters{
			PlotType::LINE_PLOT,
			PlotDimensions{ 0.0, 280, -20.0, 20, 1.0, 1.0 },
			PlotAnnotations{ "EML (wrong piece) field dynamics", "Spatial location", "Activation" } },
			LinePlotParameters{},
			{ {nf_2->getUniqueName(), "activation"}, {nf_2->getUniqueName(), "output"}, {nf_2->getUniqueName(), "input"} });

		//visualization->plot(
			//PlotCommonParameters{
			//PlotType::LINE_PLOT,
			//PlotDimensions{ 0.0, 280, -20.0, 20, 1.0, 1.0 },
			//PlotAnnotations{ "stkl next field dynamics", "Spatial location", "Activation" } },
			//LinePlotParameters{},
			//{ {nf_3->getUniqueName(), "activation"}, {nf_3->getUniqueName(), "output"}, {nf_3->getUniqueName(), "input"} });

		visualization->plot(
			PlotCommonParameters{
				PlotType::HEATMAP, 
				PlotDimensions{0.0, 280, 0.0, 280, 1.0, 1.0}, 
				PlotAnnotations{"parts not needed coupling", "wrong piece spatial location", "parts not needed spatial location"} },
				HeatmapParameters{},
			{ {fc_1->getUniqueName(), "weights"} }
		);

		//visualization->plot(
			//PlotCommonParameters{
				//PlotType::HEATMAP,
				//PlotDimensions{0.0, 280, 0.0, 280, 1.0, 1.0},
				//PlotAnnotations{"stkl present-next coupling", "stkl next spatial location", "stkl present spatial location"} },
				//HeatmapParameters{},
			//{ {fc_2->getUniqueName(), "weights"} }
			//);

		app.init();

		while (!app.hasGUIBeenClosed())
		{
			//Exception thrown at 0x00007FF613F8D2E6 in ex_wp.exe: 0xC0000005 : Access violation reading location 0x0000000000000008.
				//Unhandled exception thrown : read access violation.
				//* *<_Args_1>**was nullptr.
			app.step();
		}

		app.close();

		return 0;
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
