// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/gauss_stimulus.h"

namespace dnf_composer
{
	namespace element
	{
		GaussStimulus::GaussStimulus(const ElementCommonParameters& elementCommonParameters, 
			const GaussStimulusParameters& parameters)
			: Element(elementCommonParameters), parameters(parameters)
		{
			if (parameters.position < 0 || parameters.position >= elementCommonParameters.dimensionParameters.x_max)
				throw Exception(ErrorCode::GAUSS_STIMULUS_POSITION_OUT_OF_RANGE, 
					elementCommonParameters.identifiers.uniqueName);

			this->commonParameters.identifiers.label = ElementLabel::GAUSS_STIMULUS;
		}

		void GaussStimulus::init()
		{
			std::vector<double> g(commonParameters.dimensionParameters.size);

			if (parameters.circular)
				g = tools::math::circularGauss(commonParameters.dimensionParameters.size, 
												parameters.width, 
												parameters.position / commonParameters.dimensionParameters.d_x);
			else
				g = tools::math::gauss(commonParameters.dimensionParameters.size, 
					parameters.width, parameters.position / commonParameters.dimensionParameters.d_x);

			if (!parameters.normalized)
				for (int i = 0; i < commonParameters.dimensionParameters.size; i++)
					components["output"][i] = parameters.amplitude * g[i];
			else
			{
				const double sum = tools::math::calculateVectorSum(g);
				if(sum != 0.0)
					for (int i = 0; i < commonParameters.dimensionParameters.size; i++)
						components["output"][i] = parameters.amplitude * g[i] / sum;
				else
				{
					const std::string message = "Tried to initialize a normalized Gaussian stimulus '"
						+ this->getUniqueName() + "'. With the sum of the output vector equal "
								"to zero that is impossible! ";
					log(tools::logger::LogLevel::ERROR, message);
				}
			}

			components["input"] = std::vector<double>(commonParameters.dimensionParameters.size);
			updateInput();
			for (int i = 0; i < commonParameters.dimensionParameters.size; i++)
				components["output"][i] += components["input"][i];
		}

		void GaussStimulus::step(double t, double deltaT)
		{
		}

		void GaussStimulus::printParameters()
		{
			printCommonParameters();

			std::ostringstream logStream;

			logStream << "Logging specific element parameters" << std::endl;
			logStream << "Amplitude: " << parameters.amplitude << std::endl;
			logStream << "Width: " << parameters.width << std::endl;
			logStream << "Position: " << parameters.position << std::endl;
			logStream << "Circular: " << parameters.circular << std::endl;
			logStream << "Normalized: " << parameters.normalized;

			log(tools::logger::LogLevel::INFO, logStream.str());
		}

		std::shared_ptr<Element> GaussStimulus::clone() const
		{
			auto cloned = std::make_shared<GaussStimulus>(*this);
			return cloned;
		}

		void GaussStimulus::setParameters(const GaussStimulusParameters& gaussStimulusParameters)
		{
			parameters = gaussStimulusParameters;
			updateParameters();
		}

		GaussStimulusParameters GaussStimulus::getParameters() const
		{
			return parameters;
		}
		void GaussStimulus::updateParameters()
		{
			init();
		}
	}
}
