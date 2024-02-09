#include "./simulation/simulation_file_manager.h"

#include "elements/field_coupling.h"
#include "elements/gauss_stimulus.h"


namespace dnf_composer
{
    using json = nlohmann::json;

	SimulationFileManager::SimulationFileManager(const std::shared_ptr<Simulation>& simulation)
		: simulation(simulation)
	{
        filePath = std::string(OUTPUT_DIRECTORY) + "/simulation.json";
        log(INFO, "Simulation file manager will read and write data regarding this simulation to: " + filePath + ".\n");
	}

    json SimulationFileManager::elementToJson(const std::shared_ptr<element::Element>& element) 
    {
        json elementJson;
        
        // Get common parameters from the Element
        const element::ElementCommonParameters& commonParams = element->getElementCommonParameters();

        // Add common parameters to the JSON object
        elementJson["uniqueName"] = commonParams.identifiers.uniqueName;
        elementJson["label"] = element::ElementLabelToString.at(commonParams.identifiers.label);
        elementJson["x_max"] = commonParams.dimensionParameters.x_max;
        elementJson["d_x"] = commonParams.dimensionParameters.d_x;

        const std::vector<std::shared_ptr<element::Element>> inputs = element->getInputs();

        for (const std::shared_ptr<element::Element>& input : inputs)
        {
            const auto inputUniqueName = input->getUniqueName();
            elementJson["inputs"] += inputUniqueName;
        }

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
	                elementJson["amplitudeGlobal"] = kernelParameters.amplitudeGlobal;
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

}


