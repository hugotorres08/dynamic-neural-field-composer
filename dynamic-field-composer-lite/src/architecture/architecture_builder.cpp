#include "architecture/architecture-builder.h"

ArchitectureBuilder::ArchitectureBuilder(std::string identifier, const std::shared_ptr<Simulation>& simulation)
    : identifier(std::move(identifier)), simulation(simulation)
{
	unparsedDynamicNeuralFieldParameters = {};
	architectureParameters = ElementParameters{};
	dynamicNeuralField = DynamicNeuralField{};

    files.architectureFileLocation = std::string(OUTPUT_DIRECTORY)
        + "/architectures/architecture-" + this->identifier + "/architecture-" + this->identifier + "-dnfs.txt";
    files.fieldCouplingFileLocation = std::string(OUTPUT_DIRECTORY)
        + "/architectures/architecture-" + this->identifier + "/architecture-" + this->identifier + "-fcs.txt";
}

void ArchitectureBuilder::readArchitecture()
{
	readDynamicNeuralFieldParameters();

    const int numberOfFields = static_cast<int>(unparsedDynamicNeuralFieldParameters.size());
    
    for(int i = 0; i < numberOfFields; i++)
    {
		parseDynamicNeuralFieldParameters();
		createDynamicNeuralFieldElements();
		addElementsToSimulation();
		setupInteractionsBetweenElements();
        clearDynamicNeuralFieldParametersAndDynamicNeuralField();
    }

    readDynamicNeuralFieldCouplingsParameters();

    const int numberOfCouplings = static_cast<int>(unparsedDynamicNeuralFieldCouplingsParameters.size());

    for (int i = 0; i < numberOfCouplings; i++)
    {
        parseDynamicNeuralFieldCouplingsParameters();
        createDynamicNeuralFieldCouplings();
        addDynamicNeuralFieldCouplingsToSimulation();
        setupInteractionBetweenFields();
        clearDynamicNeuralFieldCouplingsParameters();
    }
}

void ArchitectureBuilder::saveArchitecture()
{
    // Construct the output file locations
    const std::string architectureFileLocation = std::string(OUTPUT_DIRECTORY)
        + "/architectures/architecture-" + identifier + "/architecture-" + identifier + "-dnfs-saved.txt";
    const std::string fieldCouplingFileLocation = std::string(OUTPUT_DIRECTORY)
        + "/architectures/architecture-" + identifier + "/architecture-" + identifier + "-fcs-saved.txt";

    std::ofstream architectureFile(architectureFileLocation);
    std::ofstream fieldCouplingFile(fieldCouplingFileLocation);

    if (architectureFile.is_open())
    {
        for (int i = 0; i < simulation->getNumberOfElements(); i++)
        {
            const auto element = simulation->getElement(i);

            //std::cout << "Element id: " << element->getUniqueIdentifier() << " Element name: " << element->getUniqueName() << std::endl;

            const auto elementLabel = element->getLabel();

            switch(elementLabel)
            {
            case ElementLabel::NEURAL_FIELD:
                dynamicNeuralField.neuralField = std::dynamic_pointer_cast<NeuralField>(element);
                architectureParameters.nfp = dynamicNeuralField.neuralField->getParameters();
                architectureFile << dynamicNeuralField.neuralField->getSize() << " "
                    << 1 << " " // dx
                    << architectureParameters.nfp.tau << " "
                    << -architectureParameters.nfp.startingRestingLevel << " "
                    << static_cast<int>(architectureParameters.nfp.activationFunctionParameters.type) << " ";
				if (architectureParameters.nfp.activationFunctionParameters.type == ActivationFunctionType::Sigmoid)
					architectureFile << architectureParameters.nfp.activationFunctionParameters.steepness << " ";
            	architectureFile << architectureParameters.nfp.activationFunctionParameters.xShift << " ";
                break;
            case ElementLabel::GAUSS_KERNEL:
                if (element->getUniqueName().find("noise kernel") == std::string::npos)
                {
	                dynamicNeuralField.kernel = std::dynamic_pointer_cast<GaussKernel>(element);
                    architectureParameters.gkp = std::dynamic_pointer_cast<GaussKernel>(dynamicNeuralField.kernel)->getParameters();

                    architectureFile << ElementLabel::GAUSS_KERNEL << " "
                        << architectureParameters.gkp.amplitude << " "
                        << architectureParameters.gkp.sigma << " "
                        << -architectureParameters.gkp.amplitudeGlobal << " ";
                }
                break;
            case ElementLabel::MEXICAN_HAT_KERNEL:
                dynamicNeuralField.kernel = std::dynamic_pointer_cast<MexicanHatKernel>(element);
                architectureParameters.mhkp = std::dynamic_pointer_cast<MexicanHatKernel>(dynamicNeuralField.kernel)->getParameters();

                architectureFile << ElementLabel::MEXICAN_HAT_KERNEL << " "
                    << architectureParameters.mhkp.amplitudeExc << " "
                    << architectureParameters.mhkp.sigmaExc << " "
					<< architectureParameters.mhkp.amplitudeInh << " "
					<< architectureParameters.mhkp.sigmaInh << " "
                    << -architectureParameters.mhkp.amplitudeGlobal << " ";
                break;
            case ElementLabel::NORMAL_NOISE:
                dynamicNeuralField.normalNoise = std::dynamic_pointer_cast<NormalNoise>(element);
                architectureParameters.nnp = dynamicNeuralField.normalNoise->getParameters();
                architectureFile << architectureParameters.nnp.amplitude << '\n';
                break;
            case ElementLabel::FIELD_COUPLING:
                {
                    dynamicNeuralFieldCoupling.gaussianFieldCoupling = std::dynamic_pointer_cast<GaussFieldCoupling>(element);
                    architectureParameters.gfcp = dynamicNeuralFieldCoupling.gaussianFieldCoupling->getParameters();
                    
                    const auto inputField = dynamicNeuralFieldCoupling.gaussianFieldCoupling->getInputs()[0];
                    const auto outputField = simulation->getElementsThatHaveSpecifiedElementAsInput(element->getUniqueName())[0];

                    fieldCouplingFile << inputField->getUniqueIdentifier() << " "
                        << outputField->getUniqueIdentifier() << " "
                        << architectureParameters.gfcp.sigma << " ";

                    const int numberOfCouplings = dynamicNeuralFieldCoupling.gaussianFieldCoupling->getParameters().couplings.size();

                    fieldCouplingFile << numberOfCouplings << " ";

                    for (const auto coupling : dynamicNeuralFieldCoupling.gaussianFieldCoupling->getParameters().couplings)
                        fieldCouplingFile << coupling.x_i << " "
                        << coupling.x_j << " "
                        << coupling.w_i_j << " ";

                    fieldCouplingFile << '\n';
                }
                break;
            case ElementLabel::GAUSS_STIMULUS:
            case ElementLabel::SUM_DIMENSION:
            case ElementLabel::UNINITIALIZED:
                // do nothing
                break;
            }

        }

        architectureFile.close();
        fieldCouplingFile.close();
    }
    else
        std::cout << "Error: Unable to open file for saving " << architectureFileLocation << std::endl;

}

void ArchitectureBuilder::readDynamicNeuralFieldParameters()
{
    std::ifstream architectureFile(files.architectureFileLocation);
    if (architectureFile.is_open())
    {
        std::string line;
        while (std::getline(architectureFile, line))
            unparsedDynamicNeuralFieldParameters.push_back(line);

        architectureFile.close();
    }
    else
    {
        std::cout << "Error: Unable to open file " << files.architectureFileLocation << std::endl;
    }
}

void ArchitectureBuilder::parseDynamicNeuralFieldParameters()
{
    // Get the first line from the unparsed parameters
    // size dx tau h g[1-2] kernel[3-5] q
    const std::string line = unparsedDynamicNeuralFieldParameters.front();
    unparsedDynamicNeuralFieldParameters.erase(unparsedDynamicNeuralFieldParameters.begin());
    std::istringstream iss(line);

    iss >> dynamicNeuralField.size;

    std::vector<double> parsedParameters;
    double value;
    // Parse the first 3 values as neural field parameters
    for (int i = 0; i < 3 && iss >> value; ++i)
        parsedParameters.push_back(value);

    //architectureParameters.nfp.dx = parsedParameters[0];
    architectureParameters.nfp.tau = parsedParameters[1];
    architectureParameters.nfp.startingRestingLevel = - parsedParameters[2];

    // Parse the activation function type
    int tempAfpType;
	iss >> tempAfpType;
    architectureParameters.nfp.activationFunctionParameters.type = static_cast<ActivationFunctionType>(tempAfpType);
    // Depending on the activation function, parse additional parameters
	if(architectureParameters.nfp.activationFunctionParameters.type == ActivationFunctionType::Sigmoid)
    {
        iss >> architectureParameters.nfp.activationFunctionParameters.steepness;
    }
    iss >> architectureParameters.nfp.activationFunctionParameters.xShift;

    // Parse the kernel type
    iss >> dynamicNeuralField.kernelType;
    // Depending on the kernel type, parse additional parameters
    if (dynamicNeuralField.kernelType == ElementLabel::GAUSS_KERNEL) // Gaussian kernel
    {
        iss >> architectureParameters.gkp.amplitude
    		>> architectureParameters.gkp.sigma
    		>> architectureParameters.gkp.amplitudeGlobal;
        architectureParameters.gkp.amplitudeGlobal = - architectureParameters.gkp.amplitudeGlobal;
        architectureParameters.gkp.cutOfFactor = 5;
    }
    else if (dynamicNeuralField.kernelType == ElementLabel::MEXICAN_HAT_KERNEL) // Mexican hat kernel
    {
        iss >> architectureParameters.mhkp.amplitudeExc
            >> architectureParameters.mhkp.sigmaExc
            >> architectureParameters.mhkp.amplitudeInh
            >> architectureParameters.mhkp.sigmaInh
            >> architectureParameters.mhkp.amplitudeGlobal;
    	architectureParameters.mhkp.amplitudeGlobal = -architectureParameters.mhkp.amplitudeGlobal;
    	architectureParameters.mhkp.cutOfFactor = 5;

    }

    // Parse the last value as the amplitude of the normal noise
    iss >> architectureParameters.nnp.amplitude;
}

void ArchitectureBuilder::createDynamicNeuralFieldElements()
{
    static int elementCount = 0;
    dynamicNeuralField.neuralField = std::make_shared<NeuralField>("neural field " + std::to_string(elementCount++), 
        dynamicNeuralField.size, architectureParameters.nfp);

    if (dynamicNeuralField.kernelType == ElementLabel::GAUSS_KERNEL)
        dynamicNeuralField.kernel = std::make_shared<GaussKernel>("gauss kernel " + std::to_string(elementCount++),
            dynamicNeuralField.size, architectureParameters.gkp);
    if (dynamicNeuralField.kernelType == ElementLabel::MEXICAN_HAT_KERNEL)
        dynamicNeuralField.kernel = std::make_shared<MexicanHatKernel>("mex-hat kernel " + std::to_string(elementCount++),
            dynamicNeuralField.size, architectureParameters.mhkp);

    dynamicNeuralField.normalNoise = std::make_shared<NormalNoise>("normal noise " + std::to_string(elementCount++),
        dynamicNeuralField.size, architectureParameters.nnp);
    GaussKernelParameters gkp;
    gkp.amplitude = 0.2;
    gkp.sigma = 0.25;
    gkp.cutOfFactor = 5;
    gkp.amplitudeGlobal = 0.0;
    dynamicNeuralField.normalNoiseKernel = std::make_shared<GaussKernel>("noise kernel " + std::to_string(elementCount++),
        dynamicNeuralField.size, gkp);
}

void ArchitectureBuilder::addElementsToSimulation() const
{
    simulation->addElement(dynamicNeuralField.neuralField);
    simulation->addElement(dynamicNeuralField.kernel);
    simulation->addElement(dynamicNeuralField.normalNoise);
    simulation->addElement(dynamicNeuralField.normalNoiseKernel);
}

void ArchitectureBuilder::setupInteractionsBetweenElements() const
{
	dynamicNeuralField.neuralField->addInput(dynamicNeuralField.kernel);
    dynamicNeuralField.kernel->addInput(dynamicNeuralField.neuralField);

	dynamicNeuralField.neuralField->addInput(dynamicNeuralField.normalNoiseKernel);
	dynamicNeuralField.normalNoiseKernel->addInput(dynamicNeuralField.normalNoise);
}

void ArchitectureBuilder::clearDynamicNeuralFieldParametersAndDynamicNeuralField()
{
    dynamicNeuralField = DynamicNeuralField{};
}

void ArchitectureBuilder::readDynamicNeuralFieldCouplingsParameters()
{
    std::ifstream fieldCouplingsFile(files.fieldCouplingFileLocation);
    if (fieldCouplingsFile.is_open())
    {
        std::string line;
        while (std::getline(fieldCouplingsFile, line))
            unparsedDynamicNeuralFieldCouplingsParameters.push_back(line);

        fieldCouplingsFile.close();
    }
    else
    {
        std::cout << "Error: Unable to open file " << files.fieldCouplingFileLocation << std::endl;
    }
}

void ArchitectureBuilder::parseDynamicNeuralFieldCouplingsParameters()
{
    const std::string line = unparsedDynamicNeuralFieldCouplingsParameters.front();
    unparsedDynamicNeuralFieldCouplingsParameters.erase(unparsedDynamicNeuralFieldCouplingsParameters.begin());
    std::istringstream iss(line);

    iss >> dynamicNeuralFieldCoupling.idsOfCoupledDynamicNeuralFields[0]
        >> dynamicNeuralFieldCoupling.idsOfCoupledDynamicNeuralFields[1]
        >> architectureParameters.gfcp.sigma;

    int numberOfCouplings;
    iss >> numberOfCouplings;

    for (int i = 0; i < numberOfCouplings; i++)
    {
        WeightedCoupling coupling;
        iss >> coupling.x_i
            >> coupling.x_j
            >> coupling.w_i_j;
        dynamicNeuralFieldCoupling.couplings.push_back(coupling);
    }


}

void ArchitectureBuilder::createDynamicNeuralFieldCouplings()
{
    static int elementCount = 0;

    architectureParameters.gfcp.inputFieldSize = simulation->getElement(dynamicNeuralFieldCoupling.idsOfCoupledDynamicNeuralFields[0])->getSize();
    const int size = simulation->getElement(dynamicNeuralFieldCoupling.idsOfCoupledDynamicNeuralFields[1])->getSize();

    dynamicNeuralFieldCoupling.gaussianFieldCoupling = std::make_shared<GaussFieldCoupling>("coupling " + std::to_string(elementCount++), size, architectureParameters.gfcp);
    
    for(const auto coupling : dynamicNeuralFieldCoupling.couplings)
        dynamicNeuralFieldCoupling.gaussianFieldCoupling->addCoupling(coupling);

}

void ArchitectureBuilder::addDynamicNeuralFieldCouplingsToSimulation() const
{
    simulation->addElement(dynamicNeuralFieldCoupling.gaussianFieldCoupling);
}

void ArchitectureBuilder::setupInteractionBetweenFields() const
{
    const auto inputField = simulation->getElement(dynamicNeuralFieldCoupling.idsOfCoupledDynamicNeuralFields[0]);
    const auto outputField = simulation->getElement(dynamicNeuralFieldCoupling.idsOfCoupledDynamicNeuralFields[1]);

    dynamicNeuralFieldCoupling.gaussianFieldCoupling->addInput(inputField);
    outputField->addInput(dynamicNeuralFieldCoupling.gaussianFieldCoupling);
}

void ArchitectureBuilder::clearDynamicNeuralFieldCouplingsParameters()
{
    dynamicNeuralFieldCoupling = DynamicNeuralFieldCoupling{};
}
