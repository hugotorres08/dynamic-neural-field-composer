// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

const dnf_composer::element::ElementSpatialDimensionParameters fieldDimensions{ 100, 1.0};

std::shared_ptr<dnf_composer::Simulation> getExperimentSimulation()
{
	std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>("test sim", 1, 0, 0);

	const dnf_composer::element::GaussStimulusParameters gcp_a = { 5, 10, 50, true};
	const std::shared_ptr<dnf_composer::element::GaussStimulus> gauss_stimulus(new dnf_composer::element::GaussStimulus({ "gauss stimulus", fieldDimensions }, gcp_a));simulation->addElement(gauss_stimulus);

	//const dnf_composer::element::LateralInteractionsParameters lip1 = { 3,7.2,12,6.4, -0.51 };
	//const std::shared_ptr<dnf_composer::element::LateralInteractions> k_1(new dnf_composer::element::LateralInteractions({ "k 1", fieldDimensions }, lip1));
	//simulation->addElement(k_1);

	const dnf_composer::element::MexicanHatKernelParameters mhkp = { 3,7.2,12,6.4 };
	const std::shared_ptr<dnf_composer::element::MexicanHatKernel> k_1(new dnf_composer::element::MexicanHatKernel({ "k 1", fieldDimensions }, mhkp));
	simulation->addElement(k_1);

	const dnf_composer::element::SigmoidFunction activationFunction{ 0, 4 };
	const dnf_composer::element::NeuralFieldParameters nfp1 = { 20, -10 , activationFunction };
	const std::shared_ptr<dnf_composer::element::NeuralField> neural_field_1(new dnf_composer::element::NeuralField({ "neural field 1", fieldDimensions }, nfp1));
	simulation->addElement(neural_field_1);

	neural_field_1->addInput(k_1);
	neural_field_1->addInput(gauss_stimulus);
	k_1->addInput(neural_field_1);

	//const dnf_composer::element::LateralInteractionsParameters lip2 = { 3,7.2,12,6.4, -0.51 };
	//const std::shared_ptr<dnf_composer::element::LateralInteractions> k_2(new dnf_composer::element::LateralInteractions({ "k 2", fieldDimensions }, lip2));
	//simulation->addElement(k_2);

	//const dnf_composer::element::NeuralFieldParameters nfp2 = { 20, -10 , activationFunction };
	//const std::shared_ptr<dnf_composer::element::NeuralField> neural_field_2(new dnf_composer::element::NeuralField({ "neural field 2", fieldDimensions }, nfp2));
	//simulation->addElement(neural_field_2);

	//neural_field_2->addInput(k_2);
	//k_2->addInput(neural_field_2);

	//// excitatory connections between fields
	//dnf_composer::element::GaussKernelParameters gkp1;
	//gkp1.amplitude = 10;
	//gkp1.sigma = 8;
	//const std::shared_ptr<dnf_composer::element::GaussKernel> k_1_2(new dnf_composer::element::GaussKernel({ "k 1 2", fieldDimensions }, gkp1));
	//simulation->addElement(k_1_2);

	//neural_field_2->addInput(k_1_2);
	//k_1_2->addInput(neural_field_1);

	return simulation;
}


int main(int argc, char* argv[])
{
	try
	{
		// After defining the simulation, we can create the application.
		const auto simulation = getExperimentSimulation();
		// You can run the application without the user interface by setting the second parameter to false.
		constexpr bool activateUserInterface = true;
		const dnf_composer::Application app{ simulation, activateUserInterface };

		// After creating the application, we can add the windows we want to display.
		app.activateUserInterfaceWindow(dnf_composer::user_interface::SIMULATION_WINDOW);
		app.activateUserInterfaceWindow(dnf_composer::user_interface::LOG_WINDOW);
		app.activateUserInterfaceWindow(dnf_composer::user_interface::ELEMENT_WINDOW);
		app.activateUserInterfaceWindow(dnf_composer::user_interface::MONITORING_WINDOW);

		dnf_composer::user_interface::PlotParameters plotParameters;
		plotParameters.annotations = { "Neural field monitoring", "Spatial dimension", "Amplitude" };
		plotParameters.dimensions = { 0, fieldDimensions.x_max, -15, 14, fieldDimensions.d_x };
		const auto plotWindow = std::make_shared<dnf_composer::user_interface::PlotWindow>(simulation, plotParameters);
		app.activateUserInterfaceWindow(plotWindow);

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