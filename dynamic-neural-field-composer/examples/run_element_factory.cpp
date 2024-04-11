// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This .exe shows you how to use the ElementFactory to create elements (less verbose way of creating elements).

int main(int argc, char* argv[])
{
	try
	{
		using namespace dnf_composer;
		auto simulation = std::make_shared<Simulation>("Element factory example", 5, 0, 0);
		constexpr bool activateUserInterface = true;
		const Application app{ simulation, activateUserInterface };

		element::ElementFactory factory;

		element::GaussStimulusParameters gsp = { 5, 5, 50 };
		const auto stimulus = factory.createElement(element::GAUSS_STIMULUS, { "stimulus", {100, 1.0} }, { gsp });
		simulation->addElement(stimulus);

		element::SigmoidFunction af = { 0.0, 10.0 };
		element::NeuralFieldParameters nfp = { 20, -5, af };
		const auto neural_field = factory.createElement(element::NEURAL_FIELD, { "field", {100, 1.0} }, { nfp });
		simulation->addElement(neural_field);

		element::GaussKernelParameters gkp = { 3, 5 };
		const auto gauss_kernel = factory.createElement(element::GAUSS_KERNEL, { "gauss kernel", {100, 1.0} }, { gkp });
		simulation->addElement(gauss_kernel);

		element::MexicanHatKernelParameters mhkp = { 5, 10, 2, 8 };
		const auto mexican_hat_kernel = factory.createElement(element::MEXICAN_HAT_KERNEL, { "mexican hat kernel", {100, 1.0} }, { mhkp });
		simulation->addElement(mexican_hat_kernel);

		element::LateralInteractionsParameters lip = { 5, 10, 2, 8, -0.1 };
		const auto lateral_interactions = factory.createElement(element::LATERAL_INTERACTIONS, { "lateral int. kernel", {100, 1.0} }, { lip });
		simulation->addElement(lateral_interactions);

		element::NormalNoiseParameters nnp = { 0.01 };
		const auto normal_noise = factory.createElement(element::NORMAL_NOISE, { "normal noise", {100, 1.0} }, nnp);
		simulation->addElement(normal_noise);

		// Here you can obviously use addInput() or createInteraction() methods
		simulation->createInteraction("stimulus", "output", "field");
		simulation->createInteraction("field", "output", "gauss kernel");
		simulation->createInteraction("gauss kernel", "output", "field");
		simulation->createInteraction("normal noise", "output", "field");

		// After creating the application, we can add the windows we want to display.
		app.addWindow<imgui_kit::LogWindow>();
		app.addWindow<user_interface::SimulationWindow>();
		app.addWindow<user_interface::ElementWindow>();

		user_interface::PlotParameters plotParameters;
		plotParameters.annotations = { "Element factory example", "Spatial dimension", "Amplitude" };
		plotParameters.dimensions = { 0, 100, -20, 50, 1.0};
		auto visualization = createVisualization(simulation);
		visualization->addPlottingData("field", "activation");
		visualization->addPlottingData("field", "input");
		visualization->addPlottingData("field", "output");
		app.addWindow<user_interface::PlotWindow>(visualization, plotParameters);

		app.init();

		bool userRequestClose = false;
		while (!userRequestClose)
		{
			app.step();
			userRequestClose = app.hasUIBeenClosed();
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