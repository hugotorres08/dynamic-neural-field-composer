// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/gauss_stimulus.h"

namespace dnf_composer
{
	namespace element
	{
		GaussStimulus::GaussStimulus(const ElementCommonParameters& elementCommonParameters, const GaussStimulusParameters& parameters)
			: Element(elementCommonParameters), parameters(parameters)
		{
			if (parameters.position < 0 || parameters.position >= elementCommonParameters.dimensionParameters.x_max)
				throw Exception(ErrorCode::GAUSS_STIMULUS_POSITION_OUT_OF_RANGE, elementCommonParameters.identifiers.uniqueName);

			this->commonParameters.identifiers.label = ElementLabel::GAUSS_STIMULUS;
		}

		void GaussStimulus::init()
		{
			std::vector<double> g(commonParameters.dimensionParameters.size);

			if (parameters.circular)
				g = mathtools::circularGauss(commonParameters.dimensionParameters.size, parameters.sigma, parameters.position);
			else
			{
				const std::string message = "Tried to initialize a non-circular Gaussian stimulus '" + this->getUniqueName() + "'. That is not supported yet.";
				log(LogLevel::ERROR, message);
			}

			if (!parameters.normalized)
				for (int i = 0; i < commonParameters.dimensionParameters.size; i++)
					components["output"][i] = parameters.amplitude * g[i];
			else
			{
				const std::string message = "Tried to initialize a normalized Gaussian stimulus '" + this->getUniqueName() + "'. That is not supported yet.";
				log(LogLevel::ERROR, message);
			}

			components["input"] = std::vector<double>(commonParameters.dimensionParameters.size);
			updateInput();
			for (int i = 0; i < commonParameters.dimensionParameters.size; i++)
				components["output"][i] += components["input"][i];
		}

		void GaussStimulus::step(double t, double deltaT)
		{
		}

		void GaussStimulus::close()
		{
		}

		void GaussStimulus::printParameters()
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
				const std::vector<double>& componentValues = pair.second;

				logStream << componentName << " | ";
			}

			logStream << std::endl << "Inputs: ";
			for (const auto& inputPair : inputs)
			{
				const std::shared_ptr<Element>& inputElement = inputPair.first;
				const std::string& inputComponent = inputPair.second;

				logStream << inputElement->getUniqueName() << "->" << inputComponent << " | ";
			}

			logStream << std::endl << "GaussStimulusParameters: ";
			logStream << "Amplitude: " << parameters.amplitude << " | ";
			logStream << "Sigma: " << parameters.sigma << " | ";
			logStream << "Position: " << parameters.position << " | ";
			logStream << "Circular: " << parameters.circular << " | ";
			logStream << "Normalized: " << parameters.normalized << std::endl;

			log(LogLevel::INFO, logStream.str());
		}

		void GaussStimulus::setParameters(const GaussStimulusParameters& gaussStimulusParameters)
		{
			parameters = gaussStimulusParameters;
			init();
		}

		GaussStimulusParameters GaussStimulus::getParameters() const
		{
			return parameters;
		}
	}
}
