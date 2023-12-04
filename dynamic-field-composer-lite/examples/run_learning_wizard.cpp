// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"
#include "elements/field_coupling.h"
#include "wizards/learning_wizard.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This .exe creates a simulation object programatically defining its elements.
// This .exe runs a learning wizard to compute the coupling strengths between two fields.

std::shared_ptr<dnf_composer::Simulation> getExperimentSimulation()
{
	// create simulation object
	std::shared_ptr<dnf_composer::Simulation> simulation = std::make_shared<dnf_composer::Simulation>(5, 0, 0);

	constexpr int perceptualFieldSize = 360;
	constexpr int decisionFieldSize = 180;

	// create neural fields
	constexpr dnf_composer::element::ActivationFunctionParameters afp = {dnf_composer::element::ActivationFunctionType::Heaviside, 0.0, 0.2 };
	constexpr dnf_composer::element::NeuralFieldParameters nfp1 = { 25, -10 , afp};
	constexpr dnf_composer::element::NeuralFieldParameters nfp2 = { 20, -10 , afp};
	const std::shared_ptr < dnf_composer::element::NeuralField > perceptual_field(new dnf_composer::element::NeuralField("perceptual field", perceptualFieldSize, nfp1));
	const std::shared_ptr<dnf_composer::element::NeuralField> decision_field(new dnf_composer::element::NeuralField("decision field", decisionFieldSize, nfp2));

	simulation->addElement(perceptual_field);
	simulation->addElement(decision_field);

	// create interactions and add them to the simulation
	dnf_composer::element::GaussKernelParameters gkp1;
	gkp1.amplitude = 35;  // self-sustained (without input)
	gkp1.sigma = 3;
	gkp1.amplitudeGlobal = -1;
	const std::shared_ptr<dnf_composer::element::GaussKernel> k_per_per(new dnf_composer::element::GaussKernel("per - per", perceptualFieldSize, gkp1)); // self-excitation u-u
	simulation->addElement(k_per_per);

	dnf_composer::element::GaussKernelParameters gkp2;
	gkp2.amplitude = 15;  // self-stabilized (with input)
	gkp2.sigma = 2;
	gkp1.amplitudeGlobal = -0.5;
	const std::shared_ptr<dnf_composer::element::GaussKernel> k_dec_dec(new dnf_composer::element::GaussKernel("dec - dec", decisionFieldSize, gkp2)); // self-excitation v-v
	simulation->addElement(k_dec_dec);

	const std::shared_ptr<dnf_composer::element::FieldCoupling> w_per_dec
		(new dnf_composer::element::FieldCoupling("per - dec", decisionFieldSize, perceptualFieldSize, { 0.5, 0.01 }, dnf_composer::LearningRule::DELTA_KROGH_HERTZ));
	simulation->addElement(w_per_dec);

	// create noise stimulus and noise kernel
	const std::shared_ptr<dnf_composer::element::NormalNoise> noise_per(new dnf_composer::element::NormalNoise("noise per", perceptualFieldSize, { 1 }));
	const std::shared_ptr<dnf_composer::element::NormalNoise> noise_dec(new dnf_composer::element::NormalNoise("noise dec", decisionFieldSize, { 1 }));
	const std::shared_ptr<dnf_composer::element::GaussKernel> noise_kernel_per(new dnf_composer::element::GaussKernel("noise kernel per", perceptualFieldSize, { 0.25, 0.2 }));
	const std::shared_ptr<dnf_composer::element::GaussKernel> noise_kernel_dec(new dnf_composer::element::GaussKernel("noise kernel dec", decisionFieldSize, { 0.25, 0.2 }));

	simulation->addElement(noise_per);
	simulation->addElement(noise_dec);
	simulation->addElement(noise_kernel_per);
	simulation->addElement(noise_kernel_dec);

	// define the interactions between the elements
	perceptual_field->addInput(k_per_per); // self-excitation
	perceptual_field->addInput(noise_kernel_per); // noise

	decision_field->addInput(k_dec_dec); // self-excitation
	decision_field->addInput(noise_kernel_dec); // noise
	decision_field->addInput(w_per_dec); // coupling

	k_per_per->addInput(perceptual_field);
	k_dec_dec->addInput(decision_field);

	w_per_dec->addInput(perceptual_field, "activation");

	noise_kernel_per->addInput(noise_per);
	noise_kernel_dec->addInput(noise_dec);

	// set up the learning wizard
	dnf_composer::LearningWizard learning_wizard{ simulation, "per - dec" };

	// add gaussian inputs
	// these inputs will be used to create the association between bumps of activation
	double offset = 1.0;
	dnf_composer::element::GaussStimulusParameters gsp = { 3, 15, 20 };

	std::vector<std::vector<double>> inputTargetPeaksForCoupling =
	{
		{ 00.00 + offset }, // red
		{ 40.60 + offset }, // orange
		{ 60.00 + offset }, // yellow
		{ 120.00 + offset }, // green
		{ 240.00 + offset }, // blue
		{ 274.15 + offset }, // indigo
		{ 281.79 + offset } // violet
	};
	std::vector<std::vector<double>> outputTargetPeaksForCoupling =
	{
		{ 15.00 + offset },
		{ 40.00 + offset },
		{ 65.00 + offset },
		{ 90.00 + offset },
		{ 115.00 + offset },
		{ 140.00 + offset },
		{ 165.00 + offset }
	};

	learning_wizard.setTargetPeakLocationsForNeuralFieldPre(inputTargetPeaksForCoupling);
	learning_wizard.setTargetPeakLocationsForNeuralFieldPost(outputTargetPeaksForCoupling);

	gsp.amplitude = 15;
	gsp.sigma = 3;

	learning_wizard.setGaussStimulusParameters(gsp);

	learning_wizard.simulateAssociation();

	learning_wizard.trainWeights(100);

	return simulation;

}


int main(int argc, char* argv[])
{
    // After defining the simulation, we can create the application.
    auto simulation = getExperimentSimulation();
    // You can run the application without the user interface by setting the second parameter to false.
    constexpr bool activateUserInterface = true;
    const dnf_composer::Application app{ simulation, activateUserInterface };

    // After creating the application, we can add the windows we want to display.
    app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::SimulationWindow>(simulation));
    dnf_composer::user_interface::PlotDimensions pd = { 0, 100, -30, 40 };
    app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::PlotWindow>(simulation, pd));


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