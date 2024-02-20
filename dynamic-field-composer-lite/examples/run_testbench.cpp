// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"


const dnf_composer::element::ElementSpatialDimensionParameters fieldDimensions{ 200, 0.5 };

std::shared_ptr<dnf_composer::Simulation> getExperimentSimulation()
{
	std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>("test sim", 1, 0, 0);


	const dnf_composer::element::GaussStimulusParameters gcp_a = { 5, 10, 50 };
	const std::shared_ptr<dnf_composer::element::GaussStimulus> gauss_stimulus(new dnf_composer::element::GaussStimulus({ "gauss stimulus", fieldDimensions }, gcp_a));simulation->addElement(gauss_stimulus);

	//dnf_composer::element::GaussKernelParameters gkp1;
	//gkp1.amplitude = 10;
	//gkp1.sigma = 8;
	//const std::shared_ptr<dnf_composer::element::GaussKernel> k(new dnf_composer::element::GaussKernel({ "k", fieldDimensions }, gkp1));
	//simulation->addElement(k);

	//const dnf_composer::element::MexicanHatKernelParameters mhkp = { 3,20,12,10 };
	//const std::shared_ptr<dnf_composer::element::MexicanHatKernel> k_(new dnf_composer::element::MexicanHatKernel({ "k_", fieldDimensions }, mhkp));
	//simulation->addElement(k_);

	const dnf_composer::element::LateralInteractionsParameters lip = { 3,7.2,12,6.4, -0.51 };
	const std::shared_ptr<dnf_composer::element::LateralInteractions> k(new dnf_composer::element::LateralInteractions({ "k", fieldDimensions }, lip));
	simulation->addElement(k);

	// create neural field
	const dnf_composer::element::SigmoidFunction activationFunction{ 0, 4 };
	const dnf_composer::element::NeuralFieldParameters nfp1 = { 20, -10 , activationFunction };
	const std::shared_ptr<dnf_composer::element::NeuralField> neural_field(new dnf_composer::element::NeuralField({ "neural field", fieldDimensions }, nfp1));

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
		const auto simulation = getExperimentSimulation();
		// You can run the application without the user interface by setting the second parameter to false.
		constexpr bool activateUserInterface = true;
		const dnf_composer::Application app{ simulation, activateUserInterface };

		// After creating the application, we can add the windows we want to display.
		app.activateUserInterfaceWindow(dnf_composer::user_interface::SIMULATION_WINDOW);
		app.activateUserInterfaceWindow(dnf_composer::user_interface::LOG_WINDOW);
		app.activateUserInterfaceWindow(dnf_composer::user_interface::ELEMENT_WINDOW);
		app.activateUserInterfaceWindow(dnf_composer::user_interface::MONITORING_WINDOW);

		//auto visualization = std::make_shared<dnf_composer::Visualization>(simulation);
		dnf_composer::user_interface::PlotParameters plotParameters;
		plotParameters.annotations = { "Neural field monitoring", "Spatial dimension", "Amplitude" };
		plotParameters.dimensions = { 0, fieldDimensions.x_max, -15, 14, fieldDimensions.d_x };
		/*visualization->addPlottingData("neural field", "activation");
		visualization->addPlottingData("neural field", "input");
		visualization->addPlottingData("neural field", "output");
		visualization->addPlottingData("k", "output");
		visualization->addPlottingData("k", "kernel");
		visualization->addPlottingData("gauss stimulus", "output");*/
		app.activateUserInterfaceWindow(dnf_composer::user_interface::PLOT_WINDOW, plotParameters);

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