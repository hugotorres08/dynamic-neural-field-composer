// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/normal_noise.h"

namespace dnf_composer
{
	namespace element
	{
		NormalNoise::NormalNoise(const ElementCommonParameters& elementCommonParameters, NormalNoiseParameters parameters)
			: Element(elementCommonParameters), parameters(parameters)
		{
			 commonParameters.identifiers.label = ElementLabel::NORMAL_NOISE;
		}

		void NormalNoise::init()
		{
			std::ranges::fill(components["output"], 0.0);
		}

		void NormalNoise::step(double t, double deltaT)
		{
			const std::vector<double> rand = mathtools::generateNormalVector(commonParameters.dimensionParameters.size);

			for (int i = 0; i < commonParameters.dimensionParameters.size; i++)
				components["output"][i] = parameters.amplitude / sqrt(deltaT) * rand[i];
		}

		void NormalNoise::printParameters()
		{
			std::ostringstream logStream;

			logStream << std::left;

			logStream << "Logging element parameters" << std::endl;
			logStream << "Unique Identifier: " << commonParameters.identifiers.uniqueIdentifier << std::endl;
			logStream << "Unique Name: " << commonParameters.identifiers.uniqueName << std::endl;
			logStream << "Label: " << ElementLabelToString.at(commonParameters.identifiers.label) << std::endl;
			logStream << "Maximum spatial dimension size: " << commonParameters.dimensionParameters.x_max << std::endl;
			logStream << "Spatial dimension step size: " << commonParameters.dimensionParameters.d_x << std::endl;
			logStream << "Number of samples in spatial dimension: " << commonParameters.dimensionParameters.size << std::endl;


			logStream << "Components: ";
			for (const auto& pair : components)
			{
				const std::string& componentName = pair.first;
				logStream << componentName << " | ";
			}

			logStream << std::endl << "Inputs: ";
			for (const auto& inputPair : inputs)
			{
				const std::shared_ptr<Element>& inputElement = inputPair.first;
				const std::string& inputComponent = inputPair.second;

				logStream << inputElement->getUniqueName() << "->" << inputComponent << " | ";
			}

			logStream << std::endl << "NormalNoiseParameters: ";
			logStream << "Amplitude: " << parameters.amplitude << std::endl;

			user_interface::LoggerWindow::addLog(user_interface::LogLevel::_INFO, logStream.str().c_str());
		}

		void NormalNoise::setParameters(NormalNoiseParameters normalNoiseParameters)
		{
			parameters = normalNoiseParameters;
		}

		NormalNoiseParameters NormalNoise::getParameters() const
		{
			return parameters;
		}
	}
}