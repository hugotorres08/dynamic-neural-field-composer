// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"


// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This .exe creates a simulation object programatically defining its elements.
// This .exe creates the simulation objects using ElementFactory.

std::shared_ptr<dnf_composer::Simulation> getExampleSimulation()
{
	// create simulation object
	std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>(1, 0, 0);

	// create elements
	dnf_composer::element::ElementFactory elementFactory;
	dnf_composer::element::ElementParameters elementFactoryParameters;
	constexpr int size = 100;

	// create input
	dnf_composer::element::GaussStimulusParameters gaussStimulusParameters;
	gaussStimulusParameters.amplitude = 20.0;
	gaussStimulusParameters.position = 55.0;
	gaussStimulusParameters.sigma = 5.0;
	elementFactoryParameters.gsp = gaussStimulusParameters;
	const auto gauss_stimulus = elementFactory.create(dnf_composer::element::ElementLabel::GAUSS_STIMULUS, "GaussStimulus", size, elementFactoryParameters);

	dnf_composer::element::NeuralFieldParameters neuralFieldParameters;
	neuralFieldParameters.startingRestingLevel = -10;
	neuralFieldParameters.tau = 25;
	dnf_composer::element::ActivationFunctionParameters activationFunctionParameters;
	activationFunctionParameters.type = dnf_composer::element::ActivationFunctionType::Sigmoid;
	activationFunctionParameters.steepness = 5.0;
	activationFunctionParameters.xShift = 0.0;
	neuralFieldParameters.activationFunctionParameters = activationFunctionParameters;
	elementFactoryParameters.nfp = neuralFieldParameters;
	const auto neural_field = elementFactory.create(dnf_composer::element::ElementLabel::NEURAL_FIELD, "NeuralField", size, elementFactoryParameters);

	//dnf_composer::element::GaussKernelParameters gaussKernelParameters;
	//gaussKernelParameters.amplitude = 20.0;
	//gaussKernelParameters.sigma = 5.0;
	//gaussKernelParameters.amplitudeGlobal = -5.0;
	//elementFactoryParameters.gkp = gaussKernelParameters;
	//auto gauss_kernel = elementFactory.create(dnf_composer::element::ElementLabel::GAUSS_KERNEL, "GaussKernel", size, elementFactoryParameters);

	dnf_composer::element::MexicanHatKernelParameters mexicanHatKernelParameters;
	mexicanHatKernelParameters.amplitudeExc = 32.0;
	mexicanHatKernelParameters.sigmaExc = 4.0;
	mexicanHatKernelParameters.amplitudeInh = 15.0;
	mexicanHatKernelParameters.sigmaInh = 10.0;
	mexicanHatKernelParameters.amplitudeGlobal = -2.0;
	elementFactoryParameters.mhkp = mexicanHatKernelParameters;
	const auto mexican_hat_kernel = elementFactory.create(dnf_composer::element::ElementLabel::MEXICAN_HAT_KERNEL, "MexicanHatKernel", size, elementFactoryParameters);


	simulation->addElement(gauss_stimulus);
	simulation->addElement(neural_field);
	simulation->addElement(mexican_hat_kernel);

	simulation->createInteraction("GaussStimulus", "output", "NeuralField");
	simulation->createInteraction("MexicanHatKernel", "output", "NeuralField");
	simulation->createInteraction("NeuralField", "output", "MexicanHatKernel");

	return simulation;

}


int main(int argc, char* argv[])
{
	// After defining the simulation, we can create the application.
	auto simulation = getExampleSimulation();
	// You can run the application without the user interface by setting the second parameter to false.
	constexpr bool activateUserInterface = true;
	const dnf_composer::Application app{ simulation, activateUserInterface };

	// After creating the application, we can add the windows we want to display.
	app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::SimulationWindow>(simulation));
	dnf_composer::user_interface::PlotParameters plotParameters;
	plotParameters.annotations = { "Plot title", "Spatial dimension", "Amplitude" };
	plotParameters.dimensions = { 0, 100, -30, 40 };
	app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::PlotWindow>(simulation, plotParameters));
	app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::LoggerWindow>());

	try {
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
	catch (const dnf_composer::Exception& ex) {
		std::cerr << "Exception: " << ex.what() << " ErrorCode: " << static_cast<int>(ex.getErrorCode()) << std::endl;
		return static_cast<int>(ex.getErrorCode());
	}
	catch (const std::exception& ex) {
		std::cerr << "Exception caught: " << ex.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Unknown exception occurred." << std::endl;
		return 1;
	}
}