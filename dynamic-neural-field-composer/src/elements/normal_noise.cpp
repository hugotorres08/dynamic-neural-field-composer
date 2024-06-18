// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/normal_noise.h"

namespace dnf_composer
{
	namespace element
	{
		NormalNoise::NormalNoise(const ElementCommonParameters& elementCommonParameters, NormalNoiseParameters parameters)
			: Element(elementCommonParameters), parameters(std::move(parameters))
		{
			 commonParameters.identifiers.label = ElementLabel::NORMAL_NOISE;
		}

		void NormalNoise::init()
		{
			std::ranges::fill(components["output"], 0.0);
		}

		void NormalNoise::step(double t, double deltaT)
		{
			const std::vector<double> rand = tools::math::generateNormalVector(commonParameters.dimensionParameters.size);

			for (int i = 0; i < commonParameters.dimensionParameters.size; i++)
				components["output"][i] = parameters.amplitude / sqrt(deltaT) * rand[i];
		}

		void NormalNoise::print()
		{
			printCommonParameters();
			std::ostringstream logStream;
			logStream << "Logging specific element parameters" << std::endl;
			parameters.print();
			log(tools::logger::LogLevel::INFO, logStream.str());
		}

		std::shared_ptr<Element> NormalNoise::clone() const
		{
			auto cloned = std::make_shared<NormalNoise>(*this);
			return cloned;
		}

		std::string NormalNoise::toString() const
		{
			std::string result;
			result += "Normal noise element\n";
			result += "Common parameters\n";
			result += commonParameters.toString();
			result += "Specific parameters\n";
			result += parameters.toString();
			return result;
		}

		void NormalNoise::setParameters(NormalNoiseParameters normalNoiseParameters)
		{
			parameters = std::move(normalNoiseParameters);
		}

		NormalNoiseParameters NormalNoise::getParameters() const
		{
			return parameters;
		}
	}
}