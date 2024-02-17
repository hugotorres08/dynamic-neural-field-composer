#include "./simulation/simulation_file_manager.h"


namespace dnf_composer
{
    using json = nlohmann::json;

	SimulationFileManager::SimulationFileManager(const std::shared_ptr<Simulation>& simulation)
		: simulation(simulation)
	{
        filePath = std::string(OUTPUT_DIRECTORY) + "/simulations/simulation_" + simulation->getUniqueIdentifier() + ".json";
        log(INFO, "Simulation file manager will read and write data regarding this simulation to: " + filePath + ".\n");
	}

	void SimulationFileManager::saveElementsToJson() const
	{
        json elementsJson;

        const int numberOfElementsInSimulation = simulation->getNumberOfElements();

        for (int i = 0; i < numberOfElementsInSimulation; i++)
        {
            const auto simulationElement = simulation->getElement(i);
            json elementJson;
            elementJson = elementToJson(simulationElement);
            elementsJson.push_back(elementJson);
        }

        // Write the JSON to a file
        std::ofstream file(filePath);
        if (file.is_open()) {
            file << elementsJson.dump(4); // Add indentation for readability
            log(INFO, "Elements saved to: " + filePath + ".\n");
        }
        else {
            log(ERROR, "Unable to open file to save elements: " + filePath + ".\n");
        }
	}

    void SimulationFileManager::loadElementsFromJson() const
    {
        // Open the JSON file for reading
        std::ifstream file(filePath);
        if (!file.is_open()) {
            log(ERROR, "Unable to open file to load elements: " + filePath + ".\n");
            return;
        }

        // Read the JSON content
        json elementsJson;
        try {
            file >> elementsJson;
        }
        catch (const std::exception& e) {
            log(ERROR, "Error reading JSON file: " + std::string(e.what()) + "\n");
            return;
        }

        log(INFO, "Elements loaded from: " + filePath + ".\n");

        jsonToElements(elementsJson);

    }

    json SimulationFileManager::elementToJson(const std::shared_ptr<element::Element>& element)
    {
        json elementJson;

        // Get common parameters from the Element
        const element::ElementCommonParameters& commonParams = element->getElementCommonParameters();

        // Add common parameters to the JSON object
        elementJson["uniqueName"] = commonParams.identifiers.uniqueName;
        elementJson["label"] = { commonParams.identifiers.label, element::ElementLabelToString.at(commonParams.identifiers.label) };
        elementJson["x_max"] = commonParams.dimensionParameters.x_max;
        elementJson["d_x"] = commonParams.dimensionParameters.d_x;

        // Add interactions to the JSON object
        const std::unordered_map<std::shared_ptr<element::Element>, std::string> inputs = element->getInputsAndComponents();
        if(!(inputs.empty()))
        {
            for (const auto& [key, component] : inputs)
            {
                const auto inputUniqueName = key->getUniqueName();
                const auto inputComponent = component;
                elementJson["inputs"] += {inputUniqueName, inputComponent};
            }
        }
        else
        {
            elementJson["inputs"] = {};
        }

        // Add element specific parameters to JSON object
        switch (commonParams.identifiers.label)
        {
        case element::NEURAL_FIELD:
        {
            const auto neuralField = std::dynamic_pointer_cast<element::NeuralField>(element);
            const auto neuralFieldParameters = neuralField->getParameters();
            const auto activationFunctionType = neuralFieldParameters.activationFunction->type;
            elementJson["tau"] = neuralFieldParameters.tau;
            elementJson["restingLevel"] = neuralFieldParameters.startingRestingLevel;

            switch (activationFunctionType) {
            case element::ActivationFunctionType::HEAVISIDE:
            {
                const auto heavisideActivationFunction = dynamic_cast<const element::HeavisideFunction*>(neuralFieldParameters.activationFunction.get());
                if (heavisideActivationFunction) {
                    elementJson["activationFunction"] = {
                        {"type", "heaviside"},
                        {"x_shift", heavisideActivationFunction->getXShift()}
                    };
                }
            }
            break;
            case element::ActivationFunctionType::SIGMOID:
            {
                const auto sigmoidActivationFunction = dynamic_cast<const element::SigmoidFunction*>(neuralFieldParameters.activationFunction.get());
                if (sigmoidActivationFunction) {
                    elementJson["activationFunction"] = {
                        {"type", "sigmoid"},
                        {"x_shift", sigmoidActivationFunction->getXShift()},
                        {"steepness", sigmoidActivationFunction->getSteepness()},
                    };
                }
            }
            break;
            }
        }
        break;
        case element::GAUSS_KERNEL:
        {
            const auto kernel = std::dynamic_pointer_cast<element::GaussKernel>(element);
            const auto kernelParameters = kernel->getParameters();
            elementJson["sigma"] = kernelParameters.sigma;
            elementJson["amplitude"] = kernelParameters.amplitude;
        }
        break;
        case element::MEXICAN_HAT_KERNEL:
        {
            const auto kernel = std::dynamic_pointer_cast<element::MexicanHatKernel>(element);
            const auto kernelParameters = kernel->getParameters();
            elementJson["sigmaExc"] = kernelParameters.sigmaExc;
            elementJson["amplitudeExc"] = kernelParameters.amplitudeExc;
            elementJson["sigmaInh"] = kernelParameters.sigmaInh;
            elementJson["amplitudeInh"] = kernelParameters.amplitudeInh;
        }
        break;
        case element::LATERAL_INTERACTIONS:
        {
            const auto kernel = std::dynamic_pointer_cast<element::LateralInteractions>(element);
            const auto kernelParameters = kernel->getParameters();
            elementJson["sigmaExc"] = kernelParameters.sigmaExc;
            elementJson["amplitudeExc"] = kernelParameters.amplitudeExc;
            elementJson["sigmaInh"] = kernelParameters.sigmaInh;
            elementJson["amplitudeInh"] = kernelParameters.amplitudeInh;
            elementJson["amplitudeGlobal"] = kernelParameters.amplitudeGlobal;
        }
        break;
        case element::NORMAL_NOISE:
        {
            const auto normalNoise = std::dynamic_pointer_cast<element::NormalNoise>(element);
            const auto normalNoiseParameters = normalNoise->getParameters();
            elementJson["amplitude"] = normalNoiseParameters.amplitude;
        }
        break;
        case element::GAUSS_STIMULUS:
        {
            const auto gaussStimulus = std::dynamic_pointer_cast<element::GaussStimulus>(element);
            const auto gaussStimulusParameters = gaussStimulus->getParameters();
            elementJson["amplitude"] = gaussStimulusParameters.amplitude;
            elementJson["circular"] = gaussStimulusParameters.circular;
            elementJson["normalized"] = gaussStimulusParameters.normalized;
            elementJson["position"] = gaussStimulusParameters.position;
            elementJson["sigma"] = gaussStimulusParameters.sigma;
        }
        break;
        case element::FIELD_COUPLING:
        {
            const auto fieldCoupling = std::dynamic_pointer_cast<element::FieldCoupling>(element);
            const auto fieldCouplingParameters = fieldCoupling->getParameters();
            elementJson["learningRate"] = fieldCouplingParameters.learningRate;
            elementJson["inputFieldSize"] = fieldCouplingParameters.inputFieldSize;
            elementJson["learningRule"] = fieldCouplingParameters.learningRule;
            elementJson["scalar"] = fieldCouplingParameters.scalar;
        }
        case element::GAUSS_FIELD_COUPLING:
        {
            // to do
        }
        break;
        case element::UNINITIALIZED:
            break;
        }

        return elementJson;
    }

    void SimulationFileManager::jsonToElements(const json& jsonElements) const
    {
         //Iterate over elements in the JSON and reconstruct them
	    for (const auto& elementJson : jsonElements) 
        {
	        // Parse common parameters
	        const std::string uniqueName = elementJson["uniqueName"];
	        const std::tuple<element::ElementLabel, std::string> label = elementJson["label"];
	        const int x_max = elementJson["x_max"];
	        const double d_x = elementJson["d_x"];

	        switch (get<0>(label))
	    	{
	        case element::NEURAL_FIELD: 
	            {
		            // Parse specific parameters for neural field
		            const double tau = elementJson["tau"];
		            const double restingLevel = elementJson["restingLevel"];

		            // Check activation function type and parameters
		            auto activationFunctionJson = elementJson["activationFunction"];
		            std::unique_ptr<element::ActivationFunction> activationFunction;
		            if (!activationFunctionJson.is_null()) {
		                std::string activationFunctionType = activationFunctionJson["type"];
		                if (activationFunctionType == "heaviside") {
		                    double x_shift = activationFunctionJson["x_shift"];
		                    activationFunction = std::make_unique<element::HeavisideFunction>(x_shift);
		                }
		                else if (activationFunctionType == "sigmoid") {
		                    double x_shift = activationFunctionJson["x_shift"];
		                    double steepness = activationFunctionJson["steepness"];
		                    activationFunction = std::make_unique<element::SigmoidFunction>(x_shift, steepness);
		                }
		            }
		            // Reconstruct neural field element
		            auto neuralField = std::make_shared<element::NeuralField>(
		                element::ElementCommonParameters(uniqueName, element::ElementSpatialDimensionParameters(x_max, d_x)),
		                element::NeuralFieldParameters(tau, restingLevel, *activationFunction)
		            );
		            // Add the reconstructed element to the simulation
		            simulation->addElement(neuralField);
		        }
	        	break;
            case element::NORMAL_NOISE:
            {
                const double amplitude = elementJson["amplitude"];

                auto normalNoise = std::make_shared<element::NormalNoise>(
                    element::ElementCommonParameters(uniqueName, element::ElementSpatialDimensionParameters(x_max, d_x)),
                    element::NormalNoiseParameters(amplitude)
                );
                simulation->addElement(normalNoise);
            }
            break;
	        case element::GAUSS_KERNEL:
            {
                const double amplitude = elementJson["amplitude"];
                const double sigma = elementJson["sigma"];

                auto kernel = std::make_shared<element::GaussKernel>(
                    element::ElementCommonParameters(uniqueName, element::ElementSpatialDimensionParameters(x_max, d_x)),
                    element::GaussKernelParameters(sigma, amplitude)
                );
                simulation->addElement(kernel);
            }
            break;
	        case element::MEXICAN_HAT_KERNEL:
            {
                const double amplitudeExc = elementJson["amplitudeExc"];
                const double sigmaExc = elementJson["sigmaExc"];
                const double amplitudeInh = elementJson["amplitudeInh"];
                const double sigmaInh = elementJson["sigmaInh"];

                auto kernel = std::make_shared<element::MexicanHatKernel>(
                    element::ElementCommonParameters(uniqueName, element::ElementSpatialDimensionParameters(x_max, d_x)),
                    element::MexicanHatKernelParameters(sigmaExc, amplitudeExc, sigmaInh, amplitudeInh)
                );
                simulation->addElement(kernel);
            }
            break;
            case element::LATERAL_INTERACTIONS:
            {
                const double amplitudeExc = elementJson["amplitudeExc"];
                const double sigmaExc = elementJson["sigmaExc"];
                const double amplitudeInh = elementJson["amplitudeInh"];
                const double sigmaInh = elementJson["sigmaInh"];
                const double amplitudeGlobal = elementJson["amplitudeGlobal"];

                auto kernel = std::make_shared<element::LateralInteractions>(
                    element::ElementCommonParameters(uniqueName, element::ElementSpatialDimensionParameters(x_max, d_x)),
                    element::LateralInteractionsParameters(sigmaExc, amplitudeExc, sigmaInh, amplitudeInh, amplitudeGlobal)
                );
                simulation->addElement(kernel);
            }
            break;
	        case element::GAUSS_STIMULUS:
            {
                const double amplitude = elementJson["amplitude"];
                const double sigma = elementJson["sigma"];
                const double position = elementJson["position"];
                const bool circular = elementJson["circular"];
                const bool normalized = elementJson["normalized"];

                auto stimulus = std::make_shared<element::GaussStimulus>(
                    element::ElementCommonParameters(uniqueName, element::ElementSpatialDimensionParameters(x_max, d_x)),
                    element::GaussStimulusParameters(sigma, amplitude, position, circular, normalized)
                );
                simulation->addElement(stimulus);
            }
            break;
	        case element::FIELD_COUPLING:
            {
                const int inputFieldSize = elementJson["inputFieldSize"];
                const double scalar = elementJson["scalar"];
                const double learningRate = elementJson["learningRate"];
                const LearningRule learningRule = elementJson["learningRule"];

                auto coupling = std::make_shared<element::FieldCoupling>(
                    element::ElementCommonParameters(uniqueName, element::ElementSpatialDimensionParameters(x_max, d_x)),
                    element::FieldCouplingParameters(inputFieldSize, scalar, learningRate, learningRule)
                );
                simulation->addElement(coupling);
            }
            break;
	        case element::GAUSS_FIELD_COUPLING:
            {
                // to do
            }
            break;
	        default:
                break;
	        }
    }

	    // Iterate to create interactions
	    for (const auto& elementJson : jsonElements)
	    {
	        const std::string uniqueName = elementJson["uniqueName"];
	        const auto& inputsJson = elementJson["inputs"];

            if(!inputsJson.empty())
            {
                // Iterate over each inner array
                for (const auto& input : inputsJson)
                {
                    // Extract component and keyUniqueName
                    const std::string& keyUniqueName = input[0];
                    const std::string& component = input[1];

                    simulation->createInteraction(keyUniqueName, component, uniqueName);
                }
            }

	    }

    }

}