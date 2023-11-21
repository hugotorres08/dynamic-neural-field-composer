#include "dynamic-neural-field-composer.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.

#include "example.h"

int main(int argc, char* argv[])
{
    // After defining the simulation, we can create the application.
    std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>(5, 0, 0);
    // You can run the application without the user interface by setting the second parameter to false.
    constexpr bool activateUserInterface = true;
    Application app{ simulation, activateUserInterface };
    
    // After creating the application, we can add the windows we want to display.
    app.activateUserInterfaceWindow(std::make_shared<SimulationWindow>(simulation));
    PlotDimensions pd = { 0, 100, -30, 40 };
    app.activateUserInterfaceWindow(std::make_shared<PlotWindow>(simulation, pd));

    ArchitectureBuilder architecture{"example-1", simulation};

    architecture.readArchitecture();
 /*   {
    ElementFactory factory;
    ElementParameters ep;
    constexpr int size = 100;

    NeuralFieldParameters nfp;
    nfp.tau = 20;
    nfp.startingRestingLevel = -10;
    nfp.activationFunctionParameters.type = ActivationFunctionType::Heaviside;
    nfp.activationFunctionParameters.xShift = 0;
    ep.nfp = nfp;
    const std::shared_ptr<Element> neuralField = factory.create(ElementLabel::NEURAL_FIELD, "neural field a", size, ep);

    GaussStimulusParameters gsp;
    gsp.amplitude = 20;
    gsp.circular = true;
    gsp.position = 50;
    gsp.sigma = 5;
    ep.gsp = gsp;
    const std::shared_ptr<Element> gaussStimulus = factory.create(ElementLabel::GAUSS_STIMULUS, "gauss stimulus a", size, ep);

    GaussKernelParameters gkp;
    gkp.amplitude = 15;
    gkp.sigma = 5;
    gkp.amplitudeGlobal = -1;
    ep.gkp = gkp;
    const std::shared_ptr<Element> gaussKernel = factory.create(ElementLabel::GAUSS_KERNEL, "gauss kernel a", size, ep);

    MexicanHatKernelParameters mhkp;
    mhkp.amplitudeExc = 45;
    mhkp.sigmaExc = 5;
    mhkp.amplitudeInh = 10;
    mhkp.sigmaInh = 5;
    mhkp.amplitudeGlobal = -1;
    ep.mhkp = mhkp;
    const std::shared_ptr<Element> mexicanHatKernel = factory.create(ElementLabel::MEXICAN_HAT_KERNEL, "mh kernel a", size, ep);

    NormalNoiseParameters nnp;
    nnp.amplitude = 0.01;
    ep.nnp = nnp;


    simulation->addElement(neuralField);
    simulation->addElement(gaussStimulus);
    simulation->addElement(gaussKernel);

    neuralField->addInput(gaussKernel);
    gaussKernel->addInput(neuralField);

}*/
    architecture.saveArchitecture();

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
    catch (const Exception& ex) {
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