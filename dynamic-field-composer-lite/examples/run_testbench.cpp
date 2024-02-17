// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

// This .cpp file is a test to profile the different convolution functions.

const dnf_composer::element::ElementSpatialDimensionParameters fieldDimensions{ 100, 1.0 };

std::shared_ptr<dnf_composer::Simulation> getExperimentSimulation()
{
	std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>("test sim", 1, 0, 0);

	const dnf_composer::element::ElementSpatialDimensionParameters spatial_dimensions{ 100, 1.0 };

	constexpr dnf_composer::element::GaussStimulusParameters gcp_a = { 5, 10, 50 };
	const std::shared_ptr<dnf_composer::element::GaussStimulus> gauss_stimulus(new dnf_composer::element::GaussStimulus({ "gauss stimulus", fieldDimensions }, gcp_a));simulation->addElement(gauss_stimulus);

	/*dnf_composer::element::GaussKernelParameters gkp1;
	gkp1.amplitude = 10;
	gkp1.sigma = 8;
	const std::shared_ptr<dnf_composer::element::GaussKernel> k(new dnf_composer::element::GaussKernel({ "k", fieldDimensions }, gkp1));
	simulation->addElement(k);*/

	dnf_composer::element::MexicanHatKernelParameters mhkp = { 3,20,12,10 };
	const std::shared_ptr<dnf_composer::element::MexicanHatKernel> k_(new dnf_composer::element::MexicanHatKernel({ "k_", fieldDimensions }, mhkp));
	simulation->addElement(k_);

	constexpr dnf_composer::element::LateralInteractionsParameters lip = { 3,7.2,12,6.4, -0.51 };
	const std::shared_ptr<dnf_composer::element::LateralInteractions> k(new dnf_composer::element::LateralInteractions({ "k", fieldDimensions }, lip));
	simulation->addElement(k);

	// create neural field
	const dnf_composer::element::SigmoidFunction activationFunction{ 0, 4 };
	const dnf_composer::element::NeuralFieldParameters nfp1 = { 20, -10 , activationFunction };
	const std::shared_ptr<dnf_composer::element::NeuralField> neural_field(new dnf_composer::element::NeuralField({ "neural field", spatial_dimensions }, nfp1));

	simulation->addElement(neural_field);

	neural_field->addInput(k);
	neural_field->addInput(gauss_stimulus);
	k->addInput(neural_field);

	return simulation;
}


int main(int argc, char* argv[])
{
	try
	{
		// After defining the simulation, we can create the application.
		auto simulation = getExperimentSimulation();
		// You can run the application without the user interface by setting the second parameter to false.
		constexpr bool activateUserInterface = true;
		const dnf_composer::Application app{ simulation, activateUserInterface };

		// After creating the application, we can add the windows we want to display.
		app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::SimulationWindow>(simulation));
		app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::LoggerWindow>());
		app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::CentroidMonitoringWindow>(simulation));
		app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::ElementWindow>(simulation));

		auto visualization = std::make_shared<dnf_composer::Visualization>(simulation);
		dnf_composer::user_interface::PlotParameters plotParameters;
		plotParameters.annotations = { "Neural field monitoring", "Spatial dimension", "Amplitude" };
		plotParameters.dimensions = { 0, fieldDimensions.size, -15, 14, fieldDimensions.d_x };
		visualization->addPlottingData("neural field", "activation");
		visualization->addPlottingData("neural field", "input");
		visualization->addPlottingData("neural field", "output");
		visualization->addPlottingData("k", "output");
		visualization->addPlottingData("k", "kernel");
		visualization->addPlottingData("gauss stimulus", "output");
		app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::PlotWindow>(visualization, plotParameters, true));


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