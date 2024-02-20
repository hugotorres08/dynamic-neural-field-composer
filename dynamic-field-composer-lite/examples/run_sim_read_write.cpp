// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This setup read and writes a simulation to a .json file.


std::shared_ptr<dnf_composer::Simulation> getExperimentSimulation()
{
	// create simulation object
	std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>("run sim read write to json",5, 0, 0);

	const dnf_composer::element::ElementSpatialDimensionParameters inputFieldSpatialDimensionParameters{ 100, 1.0 };
	const dnf_composer::element::ElementSpatialDimensionParameters outputFieldSpatialDimensionParameters{ 50, 0.5 };

	// create neural fields
	const dnf_composer::element::HeavisideFunction activationFunction1{ 0 };
	const dnf_composer::element::NeuralFieldParameters nfp1 = { 25, -10 , activationFunction1 };
	const std::shared_ptr<dnf_composer::element::NeuralField> input_field(new dnf_composer::element::NeuralField({ "input field", inputFieldSpatialDimensionParameters }, nfp1));
	simulation->addElement(input_field); 

	const dnf_composer::element::SigmoidFunction activationFunction2{0, 1000};
	const dnf_composer::element::NeuralFieldParameters nfp2 = { 20, -5 , activationFunction2 };
	const std::shared_ptr<dnf_composer::element::NeuralField> output_field(new dnf_composer::element::NeuralField({ "output field", outputFieldSpatialDimensionParameters }, nfp2));
	simulation->addElement(output_field);

	// create interactions and add them to the simulation
	const dnf_composer::element::GaussKernelParameters gkp1{5, 5};
	const std::shared_ptr<dnf_composer::element::GaussKernel> k_in_in(new dnf_composer::element::GaussKernel({ "in - in", inputFieldSpatialDimensionParameters }, gkp1));
	simulation->addElement(k_in_in);

	const dnf_composer::element::MexicanHatKernelParameters gkp2{15, 10, 10, 5};
	const std::shared_ptr<dnf_composer::element::MexicanHatKernel> k_out_out(new dnf_composer::element::MexicanHatKernel({ "out - out", outputFieldSpatialDimensionParameters }, gkp2));
	simulation->addElement(k_out_out);

	const dnf_composer::element::FieldCouplingParameters fcp{ inputFieldSpatialDimensionParameters.size , 0.25, 0.01, dnf_composer::LearningRule::HEBBIAN };
	const std::shared_ptr<dnf_composer::element::FieldCoupling> w_in_out
	(new dnf_composer::element::FieldCoupling({ "in - out", outputFieldSpatialDimensionParameters }, fcp));
	simulation->addElement(w_in_out);

	// create noise stimulus and noise kernel
	const std::shared_ptr<dnf_composer::element::NormalNoise> noise_in(new dnf_composer::element::NormalNoise({ "noise in", inputFieldSpatialDimensionParameters }, { 1 }));
	const std::shared_ptr<dnf_composer::element::NormalNoise> noise_out(new dnf_composer::element::NormalNoise({ "noise out", outputFieldSpatialDimensionParameters }, { 1 }));
	const std::shared_ptr<dnf_composer::element::GaussKernel> noise_kernel_in(new dnf_composer::element::GaussKernel({ "noise kernel in", inputFieldSpatialDimensionParameters }, { 0.25, 0.2 }));
	const std::shared_ptr<dnf_composer::element::GaussKernel> noise_kernel_out(new dnf_composer::element::GaussKernel({ "noise kernel out", outputFieldSpatialDimensionParameters }, { 0.25, 0.2 }));

	// add two gaussian stimulus
	const std::shared_ptr<dnf_composer::element::GaussStimulus> gauss_stimulus_in(new dnf_composer::element::GaussStimulus({ "stimulus in", inputFieldSpatialDimensionParameters }, { 5, 5, 50}));
	const std::shared_ptr<dnf_composer::element::GaussStimulus> gauss_stimulus_out(new dnf_composer::element::GaussStimulus({ "stimulus out", outputFieldSpatialDimensionParameters }, { 5, 10, 25 }));


	// define the interactions between the elements
	simulation->addElement(noise_in);
	simulation->addElement(noise_out);
	simulation->addElement(noise_kernel_in);
	simulation->addElement(noise_kernel_out);
	simulation->addElement(gauss_stimulus_in);
	simulation->addElement(gauss_stimulus_out);

	input_field->addInput(k_in_in); // self-excitation
	input_field->addInput(noise_kernel_in); // noise
	input_field->addInput(gauss_stimulus_in);

	output_field->addInput(k_out_out); // self-excitation
	output_field->addInput(noise_kernel_out); // noise
	output_field->addInput(w_in_out); // coupling
	output_field->addInput(gauss_stimulus_out);

	k_in_in->addInput(input_field);
	k_out_out->addInput(output_field);

	w_in_out->addInput(input_field, "activation");

	noise_kernel_in->addInput(noise_in);
	noise_kernel_out->addInput(noise_out);


	return simulation;
}


int main(int argc, char* argv[])
{
	// After defining the simulation, we can create the application.
	//const auto simulation = getExperimentSimulation();
	const auto simulation = std::make_shared<dnf_composer::Simulation>("run sim read write to json",5, 0, 0);

	// Create a simulation file manager to read and write simulations from .json files
	const dnf_composer::SimulationFileManager sfm{ simulation };
	//sfm.saveElementsToJson();
	sfm.loadElementsFromJson();

	// You can run the application without the user interface by setting the second parameter to false.
	constexpr bool activateUserInterface = true;
	const dnf_composer::Application app{ simulation, activateUserInterface };

	// After creating the application, we can add the windows we want to display.
	app.activateUserInterfaceWindow(dnf_composer::user_interface::SIMULATION_WINDOW);
	app.activateUserInterfaceWindow(dnf_composer::user_interface::LOG_WINDOW);
	app.activateUserInterfaceWindow(dnf_composer::user_interface::ELEMENT_WINDOW);
	app.activateUserInterfaceWindow(dnf_composer::user_interface::MONITORING_WINDOW);

	dnf_composer::user_interface::PlotParameters plotParameters;
	plotParameters.annotations = { "Plot title", "Spatial dimension", "Amplitude" };
	plotParameters.dimensions = { 0, 100, -30, 40 , 1.0};
	app.activateUserInterfaceWindow(dnf_composer::user_interface::PLOT_WINDOW, plotParameters);
	plotParameters.annotations = { "Plot title", "Spatial dimension", "Amplitude" };
	plotParameters.dimensions = { 0, 50, -30, 40 , 0.5 };
	app.activateUserInterfaceWindow(dnf_composer::user_interface::PLOT_WINDOW, plotParameters);

	try
	{
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