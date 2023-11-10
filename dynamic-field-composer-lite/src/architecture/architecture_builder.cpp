#include "architecture/architecture-builder.h"

ArchitectureBuilder::ArchitectureBuilder(std::string identifier, const std::shared_ptr<Simulation>& simulation)
    : identifier(std::move(identifier)), simulation(simulation)
{
	unparsedDynamicNeuralFieldParameters = {};
	dynamicNeuralFieldParameters = ElementParameters{};
	dynamicNeuralField = DynamicNeuralField{};
}

void ArchitectureBuilder::generate()
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
}

void ArchitectureBuilder::readDynamicNeuralFieldParameters()
{
	const std::string architectureFileLocation = std::string(OUTPUT_DIRECTORY)
		+ "/architectures/architecture-" + identifier + ".txt";

    std::ifstream architectureFile(architectureFileLocation);
    if (architectureFile.is_open())
    {
        std::string line;
        // Read each line from the file
        while (std::getline(architectureFile, line))
        {
            // Store the parsed parameters into unparsedDynamicNeuralFieldParameters
            unparsedDynamicNeuralFieldParameters.push_back(line);
        }

        // Close the file after reading
        architectureFile.close();
    }
    else
    {
        // Handle file open error
        std::cout << "Error: Unable to open file " << architectureFileLocation << std::endl;
        // You might want to throw an exception or handle the error in an appropriate way
    }
}

void ArchitectureBuilder::parseDynamicNeuralFieldParameters()
{
    // Get the first line from the unparsed parameters
    // size dx tau h g[1-2] k[3-5] q
    const std::string line = unparsedDynamicNeuralFieldParameters.front();
    unparsedDynamicNeuralFieldParameters.erase(unparsedDynamicNeuralFieldParameters.begin());
    std::istringstream iss(line);

    iss >> dynamicNeuralField.size;

    std::vector<double> parsedParameters;
    double value;
    // Parse the first 3 values as neural field parameters
    for (int i = 0; i < 3 && iss >> value; ++i)
        parsedParameters.push_back(value);

    //dynamicNeuralFieldParameters.nfp.dx = parsedParameters[0];
    dynamicNeuralFieldParameters.nfp.tau = parsedParameters[1];
    dynamicNeuralFieldParameters.nfp.startingRestingLevel = - parsedParameters[2];

    // Parse the activation function type
    int tempAfpType;
	iss >> tempAfpType;
    dynamicNeuralFieldParameters.nfp.activationFunctionParameters.type = static_cast<ActivationFunctionType>(tempAfpType);
    // Depending on the activation function, parse additional parameters
	if(dynamicNeuralFieldParameters.nfp.activationFunctionParameters.type == ActivationFunctionType::Sigmoid)
    {
        iss >> dynamicNeuralFieldParameters.nfp.activationFunctionParameters.steepness;
    }
    iss >> dynamicNeuralFieldParameters.nfp.activationFunctionParameters.xShift;

    // Parse the kernel type
    iss >> dynamicNeuralField.kernelType;
    // Depending on the kernel type, parse additional parameters
    if (dynamicNeuralField.kernelType == ElementLabel::GAUSS_KERNEL) // Gaussian kernel
    {
        iss >> dynamicNeuralFieldParameters.gkp.amplitude
    		>> dynamicNeuralFieldParameters.gkp.sigma
    		>> dynamicNeuralFieldParameters.gkp.amplitudeGlobal;
        dynamicNeuralFieldParameters.gkp.amplitudeGlobal = - dynamicNeuralFieldParameters.gkp.amplitudeGlobal;
    }
    else if (dynamicNeuralField.kernelType == ElementLabel::MEXICAN_HAT_KERNEL) // Mexican hat kernel
    {
        iss >> dynamicNeuralFieldParameters.mhkp.amplitudeExc
            >> dynamicNeuralFieldParameters.mhkp.sigmaExc
            >> dynamicNeuralFieldParameters.mhkp.amplitudeInh
            >> dynamicNeuralFieldParameters.mhkp.sigmaInh
            >> dynamicNeuralFieldParameters.mhkp.amplitudeGlobal;
    	dynamicNeuralFieldParameters.mhkp.amplitudeGlobal = -dynamicNeuralFieldParameters.mhkp.amplitudeGlobal;
    	dynamicNeuralFieldParameters.mhkp.cutOfFactor = 5;

    }

    // Parse the last value as the amplitude of the normal noise
    iss >> dynamicNeuralFieldParameters.nnp.amplitude;
}

void ArchitectureBuilder::createDynamicNeuralFieldElements()
{
    static int elementCount = 0;
    dynamicNeuralField.nf = std::make_shared<NeuralField>("neural field " + std::to_string(elementCount++), 
        dynamicNeuralField.size, dynamicNeuralFieldParameters.nfp);

    if (dynamicNeuralField.kernelType == ElementLabel::GAUSS_KERNEL)
        dynamicNeuralField.k = std::make_shared<GaussKernel>("gauss kernel " + std::to_string(elementCount++),
            dynamicNeuralField.size, dynamicNeuralFieldParameters.gkp);
    if (dynamicNeuralField.kernelType == ElementLabel::MEXICAN_HAT_KERNEL)
        dynamicNeuralField.k = std::make_shared<MexicanHatKernel>("mex-hat kernel " + std::to_string(elementCount++),
            dynamicNeuralField.size, dynamicNeuralFieldParameters.mhkp);

    dynamicNeuralField.nn = std::make_shared<NormalNoise>("normal noise " + std::to_string(elementCount++),
        dynamicNeuralField.size, dynamicNeuralFieldParameters.nnp);
    GaussKernelParameters gkp;
    gkp.amplitude = 0.25;
    gkp.sigma = 0.2;
    dynamicNeuralField.nk = std::make_shared<GaussKernel>("noise kernel " + std::to_string(elementCount++),
        dynamicNeuralField.size, gkp);
}

void ArchitectureBuilder::addElementsToSimulation() const
{
    simulation->addElement(dynamicNeuralField.nf);
    simulation->addElement(dynamicNeuralField.k);
    simulation->addElement(dynamicNeuralField.nn);
    simulation->addElement(dynamicNeuralField.nk);
}

void ArchitectureBuilder::setupInteractionsBetweenElements() const
{
    dynamicNeuralField.nf->addInput(dynamicNeuralField.k);
    dynamicNeuralField.nf->addInput(dynamicNeuralField.nk);

    dynamicNeuralField.k->addInput(dynamicNeuralField.nf);

    dynamicNeuralField.nk->addInput(dynamicNeuralField.nn);
}

void ArchitectureBuilder::clearDynamicNeuralFieldParametersAndDynamicNeuralField()
{
    dynamicNeuralField = DynamicNeuralField{};
}
