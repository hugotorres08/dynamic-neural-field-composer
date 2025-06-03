#include "application/application.h"
#include "user_interface/main_window.h"
#include "user_interface/field_metrics_window.h"
#include "user_interface/element_window.h"
#include "user_interface/plot_control_window.h"
#include "user_interface/simulation_window.h"
#include "user_interface/node_graph_window.h"
#include "elements/element_factory.h"
#include "user_interface/plots_window.h"


int main()
{
	try
	{
		using namespace dnf_composer;

		const auto simulation = std::make_shared<Simulation>("testbench", 20.0, 0.0, 0.0);
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

		const auto gscp_1 = element::ElementCommonParameters{ "Gauss stimulus 1" };
		const auto gsp_1 = element::GaussStimulusParameters{ 5, 15, 20 };
		const auto gs_1 = std::make_shared < element::GaussStimulus > (gscp_1, gsp_1);

		const auto gscp_2 = element::ElementCommonParameters{ "Gauss stimulus 2" };
		const auto gsp_2 = element::GaussStimulusParameters{ 5, 15, 50 };
		const auto gs_2 = std::make_shared < element::GaussStimulus > (gscp_2, gsp_2);

		const auto gscp_3 = element::ElementCommonParameters{ "Gauss stimulus 3" };
		const auto gsp_3 = element::GaussStimulusParameters{ 5, 15, 80 };
		const auto gs_3 = std::make_shared < element::GaussStimulus > (gscp_3, gsp_3);

		const auto gkcp = element::ElementCommonParameters{ "Gauss coupling" };
		const auto gkp = element::GaussKernelParameters{ 3, 5.652 };
		const auto gk = std::make_shared < element::GaussKernel > ( gkcp, gkp );

		const auto gkcp_1 = element::ElementCommonParameters{ "Self-excitation mexican-hat kernel 1" };
		const auto gkp_1 = element::MexicanHatKernelParameters{};
		const auto gk_1 = std::make_shared < element::MexicanHatKernel > ( gkcp_1, gkp_1 );

		const auto gkcp_2 = element::ElementCommonParameters{ "Self-excitation gauss kernel 2" };
		const auto gkp_2 = element::GaussKernelParameters{ 3, 6.405, -0.313 };
		const auto gk_2 = std::make_shared < element::GaussKernel > ( gkcp_2, gkp_2 );

		const auto nfcp_1 = element::ElementCommonParameters{ "Neural field 1" };
		const auto nfp_1 = element::NeuralFieldParameters{};
		const auto nf_1 = std::make_shared < element::NeuralField > ( nfcp_1, nfp_1 );

		const auto nfcp_2 = element::ElementCommonParameters{ "Neural field 2" };
		const auto nfp_2 = element::NeuralFieldParameters{};
		const auto nf_2 = std::make_shared < element::NeuralField > (nfcp_2, nfp_2 );

		const auto nncp_1 = element::ElementCommonParameters{ "Normal noise 1" };
		const auto nnp_1 = element::NormalNoiseParameters{};
		const auto nn_1 = std::make_shared < element::NormalNoise > (nncp_1, nnp_1);

		const auto nncp_2 = element::ElementCommonParameters{ "Normal noise 2" };
		const auto nnp_2 = element::NormalNoiseParameters{};
		const auto nn_2 = std::make_shared < element::NormalNoise > (nncp_2, nnp_2);

		simulation->addElement(gs_1);
		simulation->addElement(gs_2);
		simulation->addElement(gs_3);
		simulation->addElement(gk);
		simulation->addElement(gk_1);
		simulation->addElement(gk_2);
		simulation->addElement(nf_1);
		simulation->addElement(nf_2);
		simulation->addElement(nn_1);
		simulation->addElement(nn_2);

		nf_1->addInput(gs_1);
		nf_1->addInput(gs_2);
		nf_1->addInput(gs_3);
		nf_1->addInput(gk_1);
		gk_1->addInput(nf_1);
		nf_2->addInput(gk_2);
		gk_2->addInput(nf_2);
		nf_2->addInput(gk);
		gk->addInput(nf_1);
		nf_1->addInput(nn_1);
		nf_2->addInput(nn_2);

		visualization->plot({ {nf_1->getUniqueName(), "activation"}, {nf_1->getUniqueName(), "output"}, {nf_1->getUniqueName(), "input"} });
		visualization->plot({ {nf_2->getUniqueName(), "activation"}, {nf_2->getUniqueName(), "output"}, {nf_2->getUniqueName(), "input"} });
		visualization->plot({ {gk->getUniqueName(), "kernel"} });
		visualization->plot({ {gk_1->getUniqueName(), "kernel"} });
		visualization->plot({ {gk_2->getUniqueName(), "kernel"} });

		
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
