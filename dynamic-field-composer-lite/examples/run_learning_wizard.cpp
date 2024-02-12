// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"
#include "user_interface/centroid_monitoring_window.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This .exe creates a simulation object programatically defining its elements.
// This .exe runs a learning wizard to compute the coupling strengths between two fields.

std::shared_ptr<dnf_composer::Simulation> getExperimentSimulation()
{
	// define if you want to train the weights or not
	constexpr bool train = false;

	// create simulation object
	std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>("learning_wizard",1, 0, 0);

	const dnf_composer::element::ElementSpatialDimensionParameters perceptualFieldSpatialDimensionParameters{1000, 0.1};
	const dnf_composer::element::ElementSpatialDimensionParameters outputFieldSpatialDimensionParameters{180, 0.5};

	// create neural fields
	const dnf_composer::element::HeavisideFunction activationFunction{ 0 };
	const dnf_composer::element::NeuralFieldParameters nfp1 = { 25, -10 , activationFunction };
	const std::shared_ptr<dnf_composer::element::NeuralField> perceptual_field (new dnf_composer::element::NeuralField({ "perceptual field", perceptualFieldSpatialDimensionParameters }, nfp1));


	const dnf_composer::element::NeuralFieldParameters nfp2 = { 20, -10 , activationFunction };
	const std::shared_ptr<dnf_composer::element::NeuralField> output_field(new dnf_composer::element::NeuralField({ "output field", outputFieldSpatialDimensionParameters }, nfp2));

	simulation->addElement(perceptual_field);
	simulation->addElement(output_field);

	// create interactions and add them to the simulation
	dnf_composer::element::GaussKernelParameters gkp1;
	gkp1.amplitude = 25;  
	gkp1.sigma = 5;
	gkp1.amplitudeGlobal = -0.1;
	const std::shared_ptr<dnf_composer::element::GaussKernel> k_per_per(new dnf_composer::element::GaussKernel({ "per - per", perceptualFieldSpatialDimensionParameters}, gkp1));
	simulation->addElement(k_per_per);

	dnf_composer::element::GaussKernelParameters gkp2;
	gkp2.amplitude = 25;  
	gkp2.sigma = 3;
	gkp2.amplitudeGlobal = -0.1;
	//const std::shared_ptr<dnf_composer::element::GaussKernel> k_out_out(new dnf_composer::element::GaussKernel({ "out - out", outputFieldSpatialDimensionParameters }, gkp2)); // self-excitation v-v
	//simulation->addElement(k_out_out);

	dnf_composer::element::FieldCouplingParameters fcp;
	fcp.inputFieldSize = perceptualFieldSpatialDimensionParameters.size;
	fcp.learningRate = 0.01;
	fcp.scalar = 0.55;
	fcp.learningRule = dnf_composer::LearningRule::DELTA_WIDROW_HOFF;
	const std::shared_ptr<dnf_composer::element::FieldCoupling> w_per_out
	(new dnf_composer::element::FieldCoupling({ "per - out", outputFieldSpatialDimensionParameters }, fcp));
	simulation->addElement(w_per_out);

	// create noise stimulus and noise kernel
	const std::shared_ptr<dnf_composer::element::NormalNoise> noise_per(new dnf_composer::element::NormalNoise({ "noise per", perceptualFieldSpatialDimensionParameters }, { 0.2 }));
	const std::shared_ptr<dnf_composer::element::NormalNoise> noise_dec(new dnf_composer::element::NormalNoise({ "noise out", outputFieldSpatialDimensionParameters }, { 0.2 }));
	//const std::shared_ptr<dnf_composer::element::GaussKernel> noise_kernel_per(new dnf_composer::element::GaussKernel({ "noise kernel per", perceptualFieldSpatialDimensionParameters }, { 0.25, 0.2 }));
	//const std::shared_ptr<dnf_composer::element::GaussKernel> noise_kernel_dec(new dnf_composer::element::GaussKernel({ "noise kernel out", outputFieldSpatialDimensionParameters }, { 0.25, 0.2 }));

	simulation->addElement(noise_per);
	simulation->addElement(noise_dec);
	//simulation->addElement(noise_kernel_per);
	//simulation->addElement(noise_kernel_dec);

	// define the interactions between the elements
	perceptual_field->addInput(k_per_per); // self-excitation
	//perceptual_field->addInput(noise_kernel_per); // noise

	//output_field->addInput(k_out_out); // self-excitation
	//output_field->addInput(noise_kernel_dec); // noise
	output_field->addInput(w_per_out); // coupling

	k_per_per->addInput(perceptual_field);
	//k_out_out->addInput(output_field);

	w_per_out->addInput(perceptual_field, "activation");

	//noise_kernel_per->addInput(noise_per);
	//noise_kernel_dec->addInput(noise_dec);

	if(train)
	{
		// set up the learning wizard
		dnf_composer::LearningWizard learning_wizard{ simulation, "per - out" };

		std::vector<std::vector<double>> inputTargetPeaksForCoupling =
		{
			{ 90.00 },
			{ 180.00  },
			{ 270.00 },
		};
		std::vector<std::vector<double>> outputTargetPeaksForCoupling =
		{
			{ 15.00 },
			{ 40.00 },
			{ 65.00 , 90.00 },
		};

		learning_wizard.setTargetPeakLocationsForNeuralFieldPre(inputTargetPeaksForCoupling);
		learning_wizard.setTargetPeakLocationsForNeuralFieldPost(outputTargetPeaksForCoupling);

		learning_wizard.simulateAssociation();

		learning_wizard.trainWeights(1000);
		learning_wizard.saveWeights();
	}

	return simulation;
}


int main(int argc, char* argv[])
{
    // After defining the simulation, we can create the application.
    auto simulation = getExperimentSimulation();
    // You can run the application without the user interface by setting the second parameter to false.
    constexpr bool activateUserInterface = false;
    const dnf_composer::Application app{ simulation, activateUserInterface };

    // After creating the application, we can add the windows we want to display.
    app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::SimulationWindow>(simulation));
	app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::LoggerWindow>());
	app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::CentroidMonitoringWindow>(simulation));

	auto visualization = std::make_shared<dnf_composer::Visualization>(simulation);
	dnf_composer::user_interface::PlotParameters plotParameters;
	plotParameters.annotations = { "Perceptual field", "Spatial dimension", "Amplitude" };
	plotParameters.dimensions = { 0, 1000, -20, 50, 0.1};
	visualization->addPlottingData("perceptual field", "activation");
	visualization->addPlottingData("perceptual field", "input");
	visualization->addPlottingData("perceptual field", "output");
	app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::PlotWindow>(visualization, plotParameters, false));

	visualization = std::make_shared<dnf_composer::Visualization>(simulation);
	plotParameters.annotations = { "Output field", "Spatial dimension", "Amplitude" };
	plotParameters.dimensions = { 0, 180, -20, 50, 0.5};
	visualization->addPlottingData("output field", "activation");
	visualization->addPlottingData("output field", "input");
	visualization->addPlottingData("output field", "output");
	app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::PlotWindow>(visualization, plotParameters, false));

	// test the training by adding a stimulus to the perceptual field
	constexpr dnf_composer::element::GaussStimulusParameters gcp_a = { 5, 11, 0};
	const std::shared_ptr<dnf_composer::element::GaussStimulus> gauss_stimulus(new dnf_composer::element::GaussStimulus({ "gauss stimulus",{1000, 0.1} }, gcp_a));
	simulation->addElement(gauss_stimulus);
	simulation->createInteraction("gauss stimulus", "output", "perceptual field");

	try
	{
		app.init();

		for (int i = 0; i < 1000; i++)
			app.step();

		bool userRequestClose = false;
		//while (!userRequestClose)
		//{
		//	app.step();
		//	userRequestClose = app.getCloseUI();
		//}
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