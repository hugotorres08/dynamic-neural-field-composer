// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/element_factory.h"

namespace dnf_composer
{
	namespace element
	{

		ElementFactory::ElementFactory()
		{
			setupElementCreators();
		}

		void ElementFactory::setupElementCreators()
		{
			// Register element creators for each element type
			elementCreators[ElementLabel::NEURAL_FIELD] = [](const ElementCommonParameters& elementCommonParameters, const ElementSpecificParameters& elementSpecificParameters)
				{
					const auto params = dynamic_cast<const NeuralFieldParameters*>(&elementSpecificParameters);
					return std::make_shared<NeuralField>(elementCommonParameters, *params);
				};

			elementCreators[ElementLabel::GAUSS_STIMULUS] = [](const ElementCommonParameters& elementCommonParameters, const ElementSpecificParameters& elementSpecificParameters)
				{
					const auto params = dynamic_cast<const GaussStimulusParameters*>(&elementSpecificParameters);
					return std::make_shared<GaussStimulus>(elementCommonParameters, *params);
				};

			elementCreators[ElementLabel::GAUSS_KERNEL] = [](const ElementCommonParameters& elementCommonParameters, const ElementSpecificParameters& elementSpecificParameters)
				{
					const auto params = dynamic_cast<const GaussKernelParameters*>(&elementSpecificParameters);
					return std::make_shared<GaussKernel>(elementCommonParameters, *params);
				};

			elementCreators[ElementLabel::MEXICAN_HAT_KERNEL] = [](const ElementCommonParameters& elementCommonParameters, const ElementSpecificParameters& elementSpecificParameters)
				{
					const auto params = dynamic_cast<const MexicanHatKernelParameters*>(&elementSpecificParameters);
					return std::make_shared<MexicanHatKernel>(elementCommonParameters, *params);
				};

			elementCreators[ElementLabel::NORMAL_NOISE] = [](const ElementCommonParameters& elementCommonParameters, const ElementSpecificParameters& elementSpecificParameters)
				{
					const auto params = dynamic_cast<const NormalNoiseParameters*>(&elementSpecificParameters);
					return std::make_shared<NormalNoise>(elementCommonParameters, *params);
				};

			elementCreators[ElementLabel::GAUSS_FIELD_COUPLING] = [](const ElementCommonParameters& elementCommonParameters, const ElementSpecificParameters& elementSpecificParameters)
				{
					const auto params = dynamic_cast<const GaussFieldCouplingParameters*>(&elementSpecificParameters);
					return std::make_shared<GaussFieldCoupling>(elementCommonParameters, *params);
				};

			elementCreators[ElementLabel::FIELD_COUPLING] = [](const ElementCommonParameters& elementCommonParameters, const ElementSpecificParameters& elementSpecificParameters)
				{
					const auto params = dynamic_cast<const FieldCouplingParameters*>(&elementSpecificParameters);
					return std::make_shared<FieldCoupling>(elementCommonParameters, *params);
				};
		}

		std::shared_ptr<Element> ElementFactory::createElement(ElementLabel type, const ElementCommonParameters& elementCommonParameters, const ElementSpecificParameters& elementSpecificParameters)
		{
			const auto creator = elementCreators.find(type);

			if (creator != elementCreators.end())
			{
				return creator->second(elementCommonParameters, elementSpecificParameters);
			}
			else
				return nullptr;
		}
	}
}
