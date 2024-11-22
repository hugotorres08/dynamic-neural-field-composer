
#pragma once

#include "tools/math.h"

#include "element.h"
//#include "element_parameters/normal_noise_parameters.h"


namespace dnf_composer
{
	namespace element
	{
		struct NormalNoiseParameters : ElementSpecificParameters
		{
			NormalNoiseParameters(double amp = 0.01) : amplitude(amp) {}

			double amplitude;
			bool operator==(const NormalNoiseParameters& other) const
			{
				constexpr double epsilon = 1e-6; // Set an appropriate epsilon value
				return std::abs(amplitude - other.amplitude) < epsilon;
			}
			std::string toString() const override
			{
				std::string result;
				result += "Normal noise parameters\n";
				result += "Amplitude: ";
				return result;
			}
		};

		class NormalNoise : public Element
		{
		private:
			NormalNoiseParameters parameters;
		public:
			NormalNoise(const ElementCommonParameters& elementCommonParameters,
				NormalNoiseParameters parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::shared_ptr<Element> clone() const override;
			std::string toString() const override;

			void setParameters(NormalNoiseParameters parameters);
			NormalNoiseParameters getParameters() const;
		};
	}
}