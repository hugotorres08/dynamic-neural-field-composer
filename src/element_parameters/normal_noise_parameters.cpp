// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "element_parameters/normal_noise_parameters.h"

namespace dnf_composer
{
	namespace element
	{
		NormalNoiseParameters::NormalNoiseParameters(double amp = 1.0) : amplitude(amp) {}

		bool NormalNoiseParameters::operator==(const NormalNoiseParameters& other) const
		{
			constexpr double epsilon = 1e-6;
			return std::abs(amplitude - other.amplitude) < epsilon;
		}

		std::string NormalNoiseParameters::toString() const
		{
			std::string result;
			result += "Normal noise parameters\n";
			result += "Amplitude: ";
			return result;
		}
	}
}