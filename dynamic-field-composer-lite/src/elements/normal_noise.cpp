// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/normal_noise.h"

namespace dnf_composer
{
	namespace element
	{
		NormalNoise::NormalNoise(const std::string& id, int size, NormalNoiseParameters parameters)
			: parameters(parameters)
		{
			// Assert that the size is positive
			assert(size > 0);

			this->label = ElementLabel::NORMAL_NOISE;
			this->uniqueName = id;
			this->size = size;
			components["output"] = std::vector<double>(size);
		}

		void NormalNoise::init()
		{
			std::ranges::fill(components["output"], 0.0);
		}

		void NormalNoise::step(double t, double deltaT)
		{
			const std::vector<double> rand = mathtools::generateNormalVector(size);

			for (int i = 0; i < size; i++)
				components["output"][i] = parameters.amplitude / sqrt(deltaT) * rand[i];
		}

		void NormalNoise::setParameters(NormalNoiseParameters parameters)
		{
			this->parameters = parameters;
		}

		NormalNoiseParameters NormalNoise::getParameters() const
		{
			return parameters;
		}
	}
}