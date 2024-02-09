// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This setup read and writes a simulation to a .json file.


std::shared_ptr<dnf_composer::Simulation> getExperimentSimulation()
{
	// create simulation object
	std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>(5, 0, 0);

	const dnf_composer::element::ElementSpatialDimensionParameters inputFieldSpatialDimensionParameters{ 100, 1.0 };
	const dnf_composer::element::ElementSpatialDimensionParameters outputFieldSpatialDimensionParameters{ 50, 0.5 };

	// create neural fields
	const dnf_composer::element::HeavisideFunction activationFunction1{ 0 };
	const dnf_composer::element::NeuralFieldParameters nfp1 = { 25, -10 , activationFunction1 };
	const std::shared_ptr<dnf_composer::element::NeuralField> input_field(new dnf_composer::element::NeuralField({ "input field", inputFieldSpatialDimensionParameters }, nfp1));

	const dnf_composer::element::SigmoidFunction activationFunction2{0, 1000};
	const dnf_composer::element::NeuralFieldParameters nfp2 = { 20, -5 , activationFunction2 };
	const std::shared_ptr<dnf_composer::element::NeuralField> output_field(new dnf_composer::element::NeuralField({ "output field", outputFieldSpatialDimensionParameters }, nfp2));

	simulation->addElement(input_field);
	simulation->addElement(output_field);

	// create interactions and add them to the simulation
	dnf_composer::element::GaussKernelParameters gkp1;
	gkp1.amplitude = 25;
	gkp1.sigma = 5;
	gkp1.amplitudeGlobal = -0.1;
	const std::shared_ptr<dnf_composer::element::GaussKernel> k_in_in(new dnf_composer::element::GaussKernel({ "in - in", inputFieldSpatialDimensionParameters }, gkp1));
	simulation->addElement(k_in_in);

	dnf_composer::element::MexicanHatKernelParameters gkp2;
	gkp2.amplitudeExc = 10;
	gkp2.amplitudeInh = 5;
	gkp2.sigmaExc = 15;
	gkp2.sigmaInh = 10;
	gkp2.amplitudeGlobal = -0.05;
	const std::shared_ptr<dnf_composer::element::MexicanHatKernel> k_out_out(new dnf_composer::element::MexicanHatKernel({ "out - out", outputFieldSpatialDimensionParameters }, gkp2)); // self-excitation v-v
	simulation->addElement(k_out_out);

	dnf_composer::element::FieldCouplingParameters fcp;
	fcp.inputFieldSize = inputFieldSpatialDimensionParameters.size;
	fcp.learningRate = 0.01;
	fcp.scalar = 0.25;
	fcp.learningRule = dnf_composer::LearningRule::HEBBIAN;
	const std::shared_ptr<dnf_composer::element::FieldCoupling> w_in_out
	(new dnf_composer::element::FieldCoupling({ "in - out", outputFieldSpatialDimensionParameters }, fcp));
	simulation->addElement(w_in_out);

	// create noise stimulus and noise kernel
	const std::shared_ptr<dnf_composer::element::NormalNoise> noise_per(new dnf_composer::element::NormalNoise({ "noise in", inputFieldSpatialDimensionParameters }, { 1 }));
	const std::shared_ptr<dnf_composer::element::NormalNoise> noise_dec(new dnf_composer::element::NormalNoise({ "noise out", outputFieldSpatialDimensionParameters }, { 1 }));
	const std::shared_ptr<dnf_composer::element::GaussKernel> noise_kernel_per(new dnf_composer::element::GaussKernel({ "noise kernel in", inputFieldSpatialDimensionParameters }, { 0.25, 0.2 }));
	const std::shared_ptr<dnf_composer::element::GaussKernel> noise_kernel_dec(new dnf_composer::element::GaussKernel({ "noise kernel out", outputFieldSpatialDimensionParameters }, { 0.25, 0.2 }));

	simulation->addElement(noise_per);
	simulation->addElement(noise_dec);
	simulation->addElement(noise_kernel_per);
	simulation->addElement(noise_kernel_dec);

	// define the interactions between the elements
	input_field->addInput(k_in_in); // self-excitation
	input_field->addInput(noise_kernel_per); // noise

	output_field->addInput(k_out_out); // self-excitation
	output_field->addInput(noise_kernel_dec); // noise
	output_field->addInput(w_in_out); // coupling

	k_in_in->addInput(input_field);
	k_out_out->addInput(output_field);

	w_in_out->addInput(input_field, "activation");

	noise_kernel_per->addInput(noise_per);
	noise_kernel_dec->addInput(noise_dec);

	return simulation;
}


int main(int argc, char* argv[])
{
	// After defining the simulation, we can create the application.
	const auto simulation = getExperimentSimulation();
	// You can run the application without the user interface by setting the second parameter to false.
	constexpr bool activateUserInterface = false;
	const dnf_composer::Application app{ simulation, activateUserInterface };

	// Create a simulation file manager to read and write simulations from .json files
	const dnf_composer::SimulationFileManager sfm{simulation};
	sfm.saveElementsToJson();

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
		log(dnf_composer::LogLevel::FATAL, errorMessage, dnf_composer::LogOutputMode::CONSOLE);
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex)
	{
		log(dnf_composer::LogLevel::FATAL, "Exception caught: " + std::string(ex.what()) + ". \n", dnf_composer::LogOutputMode::CONSOLE);
		return 1;
	}
	catch (...)
	{
		log(dnf_composer::LogLevel::FATAL, "Unknown exception occurred. \n", dnf_composer::LogOutputMode::CONSOLE);
		return 1;
	}
}