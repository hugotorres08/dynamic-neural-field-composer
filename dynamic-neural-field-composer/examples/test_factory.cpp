#include "application/application.h"
#include "user_interface/main_window.h"
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

		const auto simulation = std::make_shared<Simulation>("default", 5.0, 0.0, 0.0);
		const auto visualization = std::make_shared<Visualization>(simulation);
		const Application app{ simulation, visualization };

		app.addWindow<user_interface::MainWindow>();
		app.addWindow<imgui_kit::LogWindow>();
		app.addWindow<user_interface::ElementWindow>();
		app.addWindow<user_interface::SimulationWindow>();
		app.addWindow<user_interface::PlotControlWindow>();

		//element::ElementFactory factory;
		//const auto nf_1 = factory.createElement(element::NEURAL_FIELD, element::ElementCommonParameters{element::NEURAL_FIELD}, element::NeuralFieldParameters{});
		//const auto gk_1 = factory.createElement(element::GAUSS_KERNEL, element::ElementCommonParameters{element::GAUSS_KERNEL}, element::GaussKernelParameters{});
		//const auto nn_1 = factory.createElement(element::NORMAL_NOISE, element::ElementCommonParameters{element::NORMAL_NOISE}, element::NormalNoiseParameters{});

		//const auto nf_2 = factory.createElement(element::NEURAL_FIELD, element::ElementCommonParameters{element::NEURAL_FIELD}, element::NeuralFieldParameters{});
		//const auto mhk_2 = factory.createElement(element::MEXICAN_HAT_KERNEL, element::ElementCommonParameters{element::MEXICAN_HAT_KERNEL}, element::MexicanHatKernelParameters{});
		//const auto nn_2 = factory.createElement(element::NORMAL_NOISE, element::ElementCommonParameters{element::NORMAL_NOISE}, element::NormalNoiseParameters{});

		//const auto gs_1 = factory.createElement(element::GAUSS_STIMULUS, element::ElementCommonParameters{element::GAUSS_STIMULUS}, element::GaussStimulusParameters{});
		//const auto gs_2 = factory.createElement(element::GAUSS_STIMULUS, element::ElementCommonParameters{element::GAUSS_STIMULUS}, element::GaussStimulusParameters{});

		//const auto gfc_1 = factory.createElement(element::GAUSS_FIELD_COUPLING, element::ElementCommonParameters{element::GAUSS_FIELD_COUPLING}, element::GaussFieldCouplingParameters{true, false, {{50.0, 50.0, 5.0, 5.0}}});
		//const auto fc_1 = factory.createElement(element::FIELD_COUPLING, element::ElementCommonParameters{element::FIELD_COUPLING}, element::FieldCouplingParameters{100, 1.0, 0.1, LearningRule::DELTA_KROGH_HERTZ});


		element::ElementFactory factory;
		const auto nf_1 = factory.createElement(element::NEURAL_FIELD);
		const auto gk_1 = factory.createElement(element::GAUSS_KERNEL);
		const auto nn_1 = factory.createElement(element::NORMAL_NOISE);
		const auto nf_2 = factory.createElement(element::NEURAL_FIELD);
		const auto mhk_2 = factory.createElement(element::MEXICAN_HAT_KERNEL);
		const auto nn_2 = factory.createElement(element::NORMAL_NOISE);
		const auto gs_1 = factory.createElement(element::GAUSS_STIMULUS);
		const auto gfc_1 = factory.createElement(element::GAUSS_FIELD_COUPLING);
		const auto fc_1 = factory.createElement(element::FIELD_COUPLING);

		simulation->addElement(nf_1);
		simulation->addElement(gk_1);
		simulation->addElement(nn_1);
		simulation->addElement(nf_2);
		simulation->addElement(mhk_2);
		simulation->addElement(nn_2);
		simulation->addElement(gs_1);
		simulation->addElement(gfc_1);
		simulation->addElement(fc_1);

		nf_1->addInput(gk_1);
		gk_1->addInput(nf_1);
		nf_1->addInput(nn_1);
		nf_1->addInput(gs_1);

		nf_2->addInput(mhk_2);
		mhk_2->addInput(nf_2);
		nf_2->addInput(nn_2);

		nf_2->addInput(gfc_1);
		gfc_1->addInput(nf_1);

		visualization->plot({ {nf_1->getUniqueName(), "activation"}, {nf_1->getUniqueName(), "output"}, {nf_1->getUniqueName(), "input"} });
		visualization->plot({ {gk_1->getUniqueName(), "kernel"} });
		visualization->plot({ {nf_2->getUniqueName(), "activation"}, {nf_2->getUniqueName(), "output"}, {nf_2->getUniqueName(), "input"} });
		visualization->plot({ {mhk_2->getUniqueName(), "kernel"} });

		visualization->plot(
			PlotCommonParameters{
				PlotType::HEATMAP,
				PlotDimensions{0.0, 100.0, 0.0, 100.0, 1.0},
				PlotAnnotations{"Gauss field coupling", "x", "y"} },
				HeatmapParameters{ 0.0, 0.2 },
			{ {gfc_1->getUniqueName(), "kernel"} }
		);

		visualization->plot({ {gfc_1->getUniqueName(), "output"} });

		visualization->plot(
			PlotCommonParameters{
				PlotType::HEATMAP,
				PlotDimensions{0.0, 100.0, 0.0, 100.0, 1.0},
				PlotAnnotations{"Field coupling", "x", "y"} },
				HeatmapParameters{ 0.0, 0.2 },
			{ {fc_1->getUniqueName(), "kernel"} }
		);

		visualization->plot({ {fc_1->getUniqueName(), "output"} });

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