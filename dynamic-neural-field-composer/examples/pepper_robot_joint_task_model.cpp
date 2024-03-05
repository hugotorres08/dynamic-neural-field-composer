// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

int main(int argc, char* argv[])
{
	try
	{
		using namespace dnf_composer;
		auto simulation = std::make_shared<Simulation>("Pepper robot joint task model", 10, 0, 0);
		constexpr bool activateUserInterface = true;
		const Application app{ simulation, activateUserInterface };

		element::ElementFactory factory;
		element::ElementSpatialDimensionParameters dim_params{ 360, 1.0 };

		// Intention layer // why this global inhibition? // Added inhibitory component to kernel
		element::SigmoidFunction il_af = { 0.0, 10.0 };
		element::NeuralFieldParameters il_params = { 20, -7, il_af };
		const auto il = factory.createElement(element::NEURAL_FIELD, { "intention field", dim_params }, { il_params });
		simulation->addElement(il);

		element::LateralInteractionsParameters il_il_k_params = { 6, 5, 15, 5, -0.0027777777777778 }; //-0.0027777777777778
		const auto il_k = factory.createElement(element::LATERAL_INTERACTIONS, { "il -> il", dim_params }, { il_il_k_params });
		simulation->addElement(il_k);

		element::NormalNoiseParameters il_nn_params = { 0.001 };
		const auto il_nn = factory.createElement(element::NORMAL_NOISE, { "normal noise il", dim_params }, il_nn_params);
		simulation->addElement(il_nn);

		simulation->createInteraction("intention field", "output", "il -> il");
		simulation->createInteraction("il -> il", "output", "intention field");
		simulation->createInteraction("normal noise il", "output", "intention field");

		// Action observation field // doesn't have kernel?? // added self-excitation kernel
		element::GaussStimulusParameters hand_position_gsp = { 6, 0, 0 };
		const auto hand_position_stimulus = factory.createElement(element::GAUSS_STIMULUS, { "hand position stimulus", dim_params }, { hand_position_gsp });
		simulation->addElement(hand_position_stimulus);

		element::SigmoidFunction aol_af = { 0.0, 10.0 };
		element::NeuralFieldParameters aol_params = { 20, -4, aol_af };
		const auto aol = factory.createElement(element::NEURAL_FIELD, { "action observation field", dim_params }, { aol_params });
		simulation->addElement(aol);

		element::GaussKernelParameters aol_aol_k_params = { 6, 3.5 };
		const auto aol_aol_k = factory.createElement(element::GAUSS_KERNEL, { "aol -> aol", dim_params }, { aol_aol_k_params });
		simulation->addElement(aol_aol_k);

		element::GaussKernelParameters aol_il_k_params = { 5, 5 };
		const auto aol_il_k = factory.createElement(element::GAUSS_KERNEL, { "aol -> il", dim_params }, { aol_il_k_params });
		simulation->addElement(aol_il_k);

		element::NormalNoiseParameters aol_nn_params = { 0.001 };
		const auto aol_nn = factory.createElement(element::NORMAL_NOISE, { "normal noise aol", dim_params }, aol_nn_params);
		simulation->addElement(aol_nn);

		simulation->createInteraction("action observation field", "output", "aol -> il");
		simulation->createInteraction("action observation field", "output", "aol -> aol");
		simulation->createInteraction("aol -> aol", "output", "action observation field");
		simulation->createInteraction("aol -> il", "output", "intention field");
		simulation->createInteraction("normal noise aol", "output", "action observation field");
		simulation->createInteraction("hand position stimulus", "output", "action observation field");

		// Common sub-goals field // Also no self-excitation component?
		element::GaussStimulusParameters csgl_gsp = { 6, 8, 72 };
		const auto csgl_stimulus_1 = factory.createElement(element::GAUSS_STIMULUS, { "common sub-goal stimulus 1", dim_params }, { csgl_gsp });
		simulation->addElement(csgl_stimulus_1);

		csgl_gsp = { 6, 8, 180 };
		const auto csgl_stimulus_2 = factory.createElement(element::GAUSS_STIMULUS, { "common sub-goal stimulus 2", dim_params }, { csgl_gsp });
		simulation->addElement(csgl_stimulus_2);

		csgl_gsp = { 6, 8, 288 };
		const auto csgl_stimulus_3 = factory.createElement(element::GAUSS_STIMULUS, { "common sub-goal stimulus 3", dim_params }, { csgl_gsp });
		simulation->addElement(csgl_stimulus_3);

		element::SigmoidFunction csgl_af = { 0.0, 10.0 };
		element::NeuralFieldParameters csgl_params = { 20, -5, csgl_af };
		const auto csgl = factory.createElement(element::NEURAL_FIELD, { "common sub-goals field", dim_params }, { csgl_params });
		simulation->addElement(csgl);

		element::GaussKernelParameters csgl_csgl_k_params = { 3, 3 };
		const auto csgl_csgl_k = factory.createElement(element::GAUSS_KERNEL, { "csgl -> csgl", dim_params }, { csgl_csgl_k_params });
		simulation->addElement(csgl_csgl_k);

		element::GaussKernelParameters csgl_il_k_params = { 6, 3 };
		const auto csgl_il_k = factory.createElement(element::GAUSS_KERNEL, { "csgl -> il", dim_params }, { csgl_il_k_params });
		simulation->addElement(csgl_il_k);

		element::NormalNoiseParameters csgl_nn_params = { 0.001 };
		const auto csgl_nn = factory.createElement(element::NORMAL_NOISE, { "normal noise csgl", dim_params }, csgl_nn_params);
		simulation->addElement(csgl_nn);

		simulation->createInteraction("common sub-goals field", "output", "csgl -> il");
		simulation->createInteraction("csgl -> il", "output", "intention field");
		simulation->createInteraction("common sub-goals field", "output", "csgl -> csgl");
		simulation->createInteraction("csgl -> csgl", "output", "common sub-goals field");
		simulation->createInteraction("normal noise csgl", "output", "common sub-goals field");
		simulation->createInteraction("common sub-goal stimulus 1", "output", "common sub-goals field");
		simulation->createInteraction("common sub-goal stimulus 2", "output", "common sub-goals field");
		simulation->createInteraction("common sub-goal stimulus 3", "output", "common sub-goals field");


		// Object-memory field // Again no self-excitation?
		element::GaussStimulusParameters oml_gsp = { 6, 8, 36 };
		const auto oml_stimulus_1 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 1", dim_params }, { oml_gsp });
		simulation->addElement(oml_stimulus_1);

		oml_gsp = { 6, 8, 72 };
		const auto oml_stimulus_2 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 2", dim_params }, { oml_gsp });
		simulation->addElement(oml_stimulus_2);

		oml_gsp = { 6, 8, 108 };
		const auto oml_stimulus_3 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 3", dim_params }, { oml_gsp });
		simulation->addElement(oml_stimulus_3);

		oml_gsp = { 6, 8, 144 };
		const auto oml_stimulus_4 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 4", dim_params }, { oml_gsp });
		simulation->addElement(oml_stimulus_4);

		oml_gsp = { 6, 8,180 };
		const auto oml_stimulus_5 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 5", dim_params }, { oml_gsp });
		simulation->addElement(oml_stimulus_5);

		element::SigmoidFunction oml_af = { 0.0, 10.0 };
		element::NeuralFieldParameters oml_params = { 20, -5, oml_af };
		const auto oml = factory.createElement(element::NEURAL_FIELD, { "object memory field", dim_params }, { oml_params });
		simulation->addElement(oml);

		element::GaussKernelParameters oml_oml_k_params = { 3, 3 };
		const auto oml_oml_k = factory.createElement(element::GAUSS_KERNEL, { "oml -> oml", dim_params }, { oml_oml_k_params });
		simulation->addElement(oml_oml_k);

		element::GaussKernelParameters oml_il_k_params = { 6, 3 };
		const auto oml_il_k = factory.createElement(element::GAUSS_KERNEL, { "oml -> il", dim_params }, { oml_il_k_params });
		simulation->addElement(oml_il_k);

		element::NormalNoiseParameters oml_nn_params = { 0.001 };
		const auto oml_nn = factory.createElement(element::NORMAL_NOISE, { "normal noise oml", dim_params }, oml_nn_params);
		simulation->addElement(oml_nn);

		simulation->createInteraction("object memory field", "output", "oml -> il");
		simulation->createInteraction("oml -> il", "output", "intention field");
		simulation->createInteraction("object memory field", "output", "oml -> oml");
		simulation->createInteraction("oml -> oml", "output", "object memory field");
		simulation->createInteraction("normal noise oml", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 1", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 2", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 3", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 4", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 5", "output", "object memory field");


		// Action execution field // global inhibition calc? // added inhibitory component to kernel and increased global inhibition
		element::SigmoidFunction ael_af = { 0.0, 10.0 };
		element::NeuralFieldParameters ael_params = { 20, -5, ael_af };
		const auto ael = factory.createElement(element::NEURAL_FIELD, { "action execution field", dim_params }, { ael_params });
		simulation->addElement(ael);

		element::GaussKernelParameters il_ael_k_params = { 4, 10 };
		const auto il_ael_k = factory.createElement(element::GAUSS_KERNEL, { "il -> ael", dim_params }, { il_ael_k_params });
		simulation->addElement(il_ael_k);

		element::LateralInteractionsParameters ael_ael_k_params = { 6, 5, 15, 5, -0.8 }; //h*AEL_ampl_exc/fieldSize -5/-5 / 360 = 1/360 = 0.0027777777777778
		const auto ael_ael_k = factory.createElement(element::LATERAL_INTERACTIONS, { "ael -> ael", dim_params }, { ael_ael_k_params });
		simulation->addElement(ael_ael_k);

		element::NormalNoiseParameters ael_nn_params = { 0.001 };
		const auto ael_nn = factory.createElement(element::NORMAL_NOISE, { "normal noise ael", dim_params }, ael_nn_params);
		simulation->addElement(ael_nn);

		simulation->createInteraction("action execution field", "output", "ael -> ael");
		simulation->createInteraction("ael -> ael", "output", "action execution field");
		simulation->createInteraction("normal noise ael", "output", "action execution field");
		simulation->createInteraction("intention field", "output", "il -> ael");
		simulation->createInteraction("il -> ael", "output", "action execution field");

		// Create User Interface windows
		app.activateUserInterfaceWindow(user_interface::SIMULATION_WINDOW);
		app.activateUserInterfaceWindow(user_interface::LOG_WINDOW);
		app.activateUserInterfaceWindow(user_interface::ELEMENT_WINDOW);
		app.activateUserInterfaceWindow(user_interface::MONITORING_WINDOW);

		constexpr int yMax = 10;
		constexpr int yMin = 8;

		// Create a plot for each neural field
		user_interface::PlotParameters aolPlotParameters;
		aolPlotParameters.annotations = { "Action observation field", "Spatial dimension", "Amplitude" };
		aolPlotParameters.dimensions = { 0, dim_params.x_max, -yMin, yMax, dim_params.d_x };
		const auto actionObservationPlotWindow = std::make_shared<user_interface::PlotWindow>(simulation, aolPlotParameters);
		actionObservationPlotWindow->addPlottingData("action observation field", "activation");
		actionObservationPlotWindow->addPlottingData("action observation field", "input");
		actionObservationPlotWindow->addPlottingData("action observation field", "output");
		app.activateUserInterfaceWindow(actionObservationPlotWindow);

		user_interface::PlotParameters ilPlotParameters;
		ilPlotParameters.annotations = { "Intention field", "Spatial dimension", "Amplitude" };
		ilPlotParameters.dimensions = { 0, dim_params.x_max, -yMin, yMax, dim_params.d_x };
		const auto intentionFieldPlotWindow = std::make_shared<user_interface::PlotWindow>(simulation, ilPlotParameters);
		intentionFieldPlotWindow->addPlottingData("intention field", "activation");
		intentionFieldPlotWindow->addPlottingData("intention field", "input");
		intentionFieldPlotWindow->addPlottingData("intention field", "output");
		app.activateUserInterfaceWindow(intentionFieldPlotWindow);

		user_interface::PlotParameters csglPlotParameters;
		csglPlotParameters.annotations = { "Common sub-goals field", "Spatial dimension", "Amplitude" };
		csglPlotParameters.dimensions = { 0, dim_params.x_max, -yMin, yMax, dim_params.d_x };
		const auto csglFieldPlotWindow = std::make_shared<user_interface::PlotWindow>(simulation, csglPlotParameters);
		csglFieldPlotWindow->addPlottingData("common sub-goals field", "activation");
		csglFieldPlotWindow->addPlottingData("common sub-goals field", "input");
		csglFieldPlotWindow->addPlottingData("common sub-goals field", "output");
		app.activateUserInterfaceWindow(csglFieldPlotWindow);

		user_interface::PlotParameters omlPlotParameters;
		omlPlotParameters.annotations = { "Object memory field", "Spatial dimension", "Amplitude" };
		omlPlotParameters.dimensions = { 0, dim_params.x_max, -yMin, yMax, dim_params.d_x };
		const auto omlFieldPlotWindow = std::make_shared<user_interface::PlotWindow>(simulation, omlPlotParameters);
		omlFieldPlotWindow->addPlottingData("object memory field", "activation");
		omlFieldPlotWindow->addPlottingData("object memory field", "input");
		omlFieldPlotWindow->addPlottingData("object memory field", "output");
		app.activateUserInterfaceWindow(omlFieldPlotWindow);

		user_interface::PlotParameters aelPlotParameters;
		aelPlotParameters.annotations = { "Action execution field", "Spatial dimension", "Amplitude" };
		aelPlotParameters.dimensions = { 0, dim_params.x_max, -yMin, yMax, dim_params.d_x };
		const auto aelFieldPlotWindow = std::make_shared<user_interface::PlotWindow>(simulation, aelPlotParameters);
		aelFieldPlotWindow->addPlottingData("action execution field", "activation");
		aelFieldPlotWindow->addPlottingData("action execution field", "input");
		aelFieldPlotWindow->addPlottingData("action execution field", "output");
		app.activateUserInterfaceWindow(aelFieldPlotWindow);

		app.init();

		bool userRequestClose = false;
		while (!userRequestClose)
		{
			app.step();
			userRequestClose = app.getCloseUI();
		}
		app.close();
		return 0;
	}
	catch (const dnf_composer::Exception& ex)
	{
		const std::string errorMessage = "Exception: " + std::string(ex.what()) + " ErrorCode: " + std::to_string(static_cast<int>(ex.getErrorCode())) + ". \n";
		log(dnf_composer::tools::logger::LogLevel::FATAL, errorMessage, dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(dnf_composer::tools::logger::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". \n", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(dnf_composer::tools::logger::LogLevel::FATAL, "Unknown exception occurred. \n", dnf_composer::tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
}