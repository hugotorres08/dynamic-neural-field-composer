#pragma once

#include "element.h"

#include "element.h"
#include "gauss_kernel.h"
#include "gauss_stimulus.h"
#include "mexican_hat_kernel.h"
#include "neural_field.h" 
#include "normal_noise.h"


struct ElementParameters
{
	NeuralFieldParameters nfp;
    GaussStimulusParameters gsp;
    GaussKernelParameters gkp;
    MexicanHatKernelParameters mhkp;
    NormalNoiseParameters nnp;
};

class ElementFactory
{
private:
    std::unordered_map<ElementLabel, std::function<std::shared_ptr<Element>(const std::string&, int, const ElementParameters&)>> elementCreators;

public:
    ElementFactory()
	{
        // Register element creators for each element type
        elementCreators[ElementLabel::NEURAL_FIELD] = [](const std::string& id, int size, const ElementParameters& params)
            {
                return std::make_shared<NeuralField>(id, size, params.nfp);
            };

        elementCreators[ElementLabel::GAUSS_STIMULUS] = [](const std::string& id, int size, const ElementParameters& params)
            {
                return std::make_shared<GaussStimulus>(id, size, params.gsp);
            };

        elementCreators[ElementLabel::GAUSS_KERNEL] = [](const std::string& id, int size, const ElementParameters& params)
            {
                return std::make_shared<GaussKernel>(id, size, params.gkp);
            };

        elementCreators[ElementLabel::MEXICAN_HAT_KERNEL] = [](const std::string& id, int size, const ElementParameters& params)
            {
                return std::make_shared<MexicanHatKernel>(id, size, params.mhkp);
            };

        elementCreators[ElementLabel::NORMAL_NOISE] = [](const std::string& id, int size, const ElementParameters& params)
            {
                return std::make_shared<NormalNoise>(id, size, params.nnp);
            };
    }

    std::shared_ptr<Element> create(ElementLabel type, const std::string& id, int size, const ElementParameters& params)
	{
        const auto creator = elementCreators.find(type);
        if (creator != elementCreators.end()) 
        {
            return creator->second(id, size, params);
        }
        // Handle unsupported element types or return a default element
        return nullptr;
    }
};
