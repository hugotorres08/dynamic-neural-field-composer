// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

int main(int argc, char* argv[])
{
	try
	{
		using namespace dnf_composer;
		auto simulation = std::make_shared<Simulation>("Element factory example", 5, 0, 0);
		constexpr bool activateUserInterface = true;
		const Application app{ simulation, activateUserInterface };

		element::ElementFactory factory;
		element::ElementSpatialDimensionParameters dimension_parameters{ 360, 1.0 };


		// Intention field // why this global inhibition? // Added inhibitory component to kernel
		element::SigmoidFunction intention_field_af = { 0.0, 10.0 };
		element::NeuralFieldParameters intention_field_parameters = { 20, -5, intention_field_af };
		const auto intention_field = factory.createElement(element::NEURAL_FIELD, { "intention field", dimension_parameters }, { intention_field_parameters });
		simulation->addElement(intention_field);

		element::LateralInteractionsParameters intention_field_kernel_parameters = { 6, 5, 15, 5, -0.0027777777777778 };
		const auto intention_field_kernel = factory.createElement(element::LATERAL_INTERACTIONS, { "il -> il", dimension_parameters }, { intention_field_kernel_parameters });
		simulation->addElement(intention_field_kernel);

		element::NormalNoiseParameters intention_field_nnp = { 0.001 };
		const auto intention_field_normal_noise = factory.createElement(element::NORMAL_NOISE, { "normal noise il", dimension_parameters }, intention_field_nnp);
		simulation->addElement(intention_field_normal_noise);

		simulation->createInteraction("intention field", "output", "il -> il");
		simulation->createInteraction("il -> il", "output", "intention field");
		simulation->createInteraction("normal noise il", "output", "intention field");

		// Action observation field // doesn't have kernel??
		element::GaussStimulusParameters hand_position_gsp = { 6, 0, 0 };
		const auto hand_position_stimulus = factory.createElement(element::GAUSS_STIMULUS, { "hand position stimulus", dimension_parameters }, { hand_position_gsp });
		simulation->addElement(hand_position_stimulus);

		element::SigmoidFunction action_observation_field_af = { 0.0, 10.0 };
		element::NeuralFieldParameters action_observation_field_parameters = { 20, -2, action_observation_field_af };
		const auto action_observation_field = factory.createElement(element::NEURAL_FIELD, { "action observation field", dimension_parameters }, { action_observation_field_parameters });
		simulation->addElement(action_observation_field);

		element::GaussKernelParameters action_observation_field_kernel_parameters = { 6, 3.5 };
		const auto action_observation_field_kernel = factory.createElement(element::GAUSS_KERNEL, { "aol -> il", dimension_parameters }, { action_observation_field_kernel_parameters });
		simulation->addElement(action_observation_field_kernel);

		element::NormalNoiseParameters action_observation_field_nnp = { 0.001 };
		const auto action_observation_field_normal_noise = factory.createElement(element::NORMAL_NOISE, { "normal noise aol", dimension_parameters }, action_observation_field_nnp);
		simulation->addElement(action_observation_field_normal_noise);

		simulation->createInteraction("action observation field", "output", "aol -> il");
		simulation->createInteraction("aol -> il", "output", "intention field");
		simulation->createInteraction("normal noise aol", "output", "action observation field");
		simulation->createInteraction("hand position stimulus", "output", "action observation field");

		// Common sub-goals field // Also no self-excitation component?
		element::GaussStimulusParameters common_sub_goal_gsp = { 6, 8, 72 };
		const auto common_sub_goal_stimulus_1 = factory.createElement(element::GAUSS_STIMULUS, { "common sub-goal stimulus 1", dimension_parameters }, { common_sub_goal_gsp });
		simulation->addElement(common_sub_goal_stimulus_1);

		common_sub_goal_gsp = { 6, 8, 180 };
		const auto common_sub_goal_stimulus_2 = factory.createElement(element::GAUSS_STIMULUS, { "common sub-goal stimulus 2", dimension_parameters }, { common_sub_goal_gsp });
		simulation->addElement(common_sub_goal_stimulus_2);

		common_sub_goal_gsp = { 6, 8, 288 };
		const auto common_sub_goal_stimulus_3 = factory.createElement(element::GAUSS_STIMULUS, { "common sub-goal stimulus 3", dimension_parameters }, { common_sub_goal_gsp });
		simulation->addElement(common_sub_goal_stimulus_3);

		element::SigmoidFunction common_sub_goals_field_af = { 0.0, 10.0 };
		element::NeuralFieldParameters common_sub_goals_field_parameters = { 20, -5, common_sub_goals_field_af };
		const auto common_sub_goals_field = factory.createElement(element::NEURAL_FIELD, { "common sub-goals field", dimension_parameters }, { common_sub_goals_field_parameters });
		simulation->addElement(common_sub_goals_field);

		element::GaussKernelParameters common_sub_goals_field_kernel_parameters = { 6, 3 };
		const auto common_sub_goals_field_kernel = factory.createElement(element::GAUSS_KERNEL, { "csgl -> il", dimension_parameters }, { common_sub_goals_field_kernel_parameters });
		simulation->addElement(common_sub_goals_field_kernel);

		element::NormalNoiseParameters common_sub_goals_field_nnp = { 0.001 };
		const auto common_sub_goals_field_normal_noise = factory.createElement(element::NORMAL_NOISE, { "normal noise csgl", dimension_parameters }, common_sub_goals_field_nnp);
		simulation->addElement(common_sub_goals_field_normal_noise);

		simulation->createInteraction("common sub-goals field", "output", "csgl -> il");
		simulation->createInteraction("csgl -> il", "output", "intention field");
		simulation->createInteraction("normal noise csgl", "output", "common sub-goals field");
		simulation->createInteraction("common sub-goal stimulus 1", "output", "common sub-goals field");
		simulation->createInteraction("common sub-goal stimulus 2", "output", "common sub-goals field");
		simulation->createInteraction("common sub-goal stimulus 3", "output", "common sub-goals field");


		// Object-memory field // Again no self-excitation?
		element::GaussStimulusParameters object_memory_gsp = { 6, 8, 36 };
		const auto object_memory_stimulus_1 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 1", dimension_parameters }, { object_memory_gsp });
		simulation->addElement(object_memory_stimulus_1);

		object_memory_gsp = { 6, 8, 72 };
		const auto object_memory_stimulus_2 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 2", dimension_parameters }, { object_memory_gsp });
		simulation->addElement(object_memory_stimulus_2);

		object_memory_gsp = { 6, 8, 108 };
		const auto object_memory_stimulus_3 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 3", dimension_parameters }, { object_memory_gsp });
		simulation->addElement(object_memory_stimulus_3);

		object_memory_gsp = { 6, 8, 144 };
		const auto object_memory_stimulus_4 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 4", dimension_parameters }, { object_memory_gsp });
		simulation->addElement(object_memory_stimulus_4);

		object_memory_gsp = { 6, 8,180 };
		const auto object_memory_stimulus_5 = factory.createElement(element::GAUSS_STIMULUS, { "object memory stimulus 5", dimension_parameters }, { object_memory_gsp });
		simulation->addElement(object_memory_stimulus_5);

		element::SigmoidFunction object_memory_field_af = { 0.0, 10.0 };
		element::NeuralFieldParameters object_memory_field_parameters = { 20, -5, object_memory_field_af };
		const auto object_memory_field = factory.createElement(element::NEURAL_FIELD, { "object memory field", dimension_parameters }, { object_memory_field_parameters });
		simulation->addElement(object_memory_field);

		element::GaussKernelParameters object_memory_field_kernel_parameters = { 6, 3 };
		const auto object_memory_field_kernel = factory.createElement(element::GAUSS_KERNEL, { "oml -> il", dimension_parameters }, { object_memory_field_kernel_parameters });
		simulation->addElement(object_memory_field_kernel);

		element::NormalNoiseParameters object_memory_field_nnp = { 0.001 };
		const auto object_memory_field_normal_noise = factory.createElement(element::NORMAL_NOISE, { "normal noise oml", dimension_parameters }, object_memory_field_nnp);
		simulation->addElement(object_memory_field_normal_noise);

		simulation->createInteraction("object memory field", "output", "oml -> il");
		simulation->createInteraction("oml -> il", "output", "intention field");
		simulation->createInteraction("normal noise oml", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 1", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 2", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 3", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 4", "output", "object memory field");
		simulation->createInteraction("object memory stimulus 5", "output", "object memory field");


		// Action execution field // global inhibition calc? // added inhibitory component to kernel and increased global inhibition
		element::SigmoidFunction action_execution_field_af = { 0.0, 10.0 };
		element::NeuralFieldParameters action_execution_field_parameters = { 20, -5, action_execution_field_af };
		const auto action_execution_field = factory.createElement(element::NEURAL_FIELD, { "action execution field", dimension_parameters }, { action_execution_field_parameters });
		simulation->addElement(action_execution_field);

		element::GaussKernelParameters il_ael_gkp = { 6, 10 };
		const auto il_ael_gk = factory.createElement(element::GAUSS_KERNEL, { "il -> ael", dimension_parameters }, { il_ael_gkp });
		simulation->addElement(il_ael_gk);

		element::LateralInteractionsParameters ael_ael_likp = { 6, 5, 15, 5, -0.5 }; //h*AEL_ampl_exc/fieldSize -5/-5 / 360 = 1/360 = 0.0027777777777778
		const auto ael_ael_lik = factory.createElement(element::LATERAL_INTERACTIONS, { "ael -> ael", dimension_parameters }, { ael_ael_likp });
		simulation->addElement(ael_ael_lik);

		element::NormalNoiseParameters ael_nnp = { 0.001 };
		const auto ael_nn = factory.createElement(element::NORMAL_NOISE, { "normal noise ael", dimension_parameters }, ael_nnp);
		simulation->addElement(ael_nn);

		simulation->createInteraction("action execution field", "output", "ael -> ael");
		simulation->createInteraction("ael -> ael", "output", "action execution field");
		simulation->createInteraction("normal noise ael", "output", "action execution field");
		simulation->createInteraction("intention field", "output", "il -> ael");
		simulation->createInteraction("il -> ael", "output", "action execution field");


		app.activateUserInterfaceWindow(user_interface::SIMULATION_WINDOW);
		app.activateUserInterfaceWindow(user_interface::LOG_WINDOW);
		app.activateUserInterfaceWindow(user_interface::ELEMENT_WINDOW);
		app.activateUserInterfaceWindow(user_interface::MONITORING_WINDOW);
		user_interface::PlotParameters plotParameters{ {0, dimension_parameters.x_max, -15, 14, dimension_parameters.d_x},{ "Intention field", "Spatial dimension", "Amplitude" } };
		app.activateUserInterfaceWindow(user_interface::PLOT_WINDOW, plotParameters);

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
		log(tools::logger::LogLevel::FATAL, errorMessage, tools::logger::LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(tools::logger::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". \n", tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(tools::logger::LogLevel::FATAL, "Unknown exception occurred. \n", tools::logger::LogOutputMode::CONSOLE);
		return 1;
	}
}