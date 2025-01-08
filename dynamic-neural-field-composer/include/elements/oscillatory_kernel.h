#pragma once

#include "kernel.h"
#include "tools/math.h"

namespace dnf_composer
{
	namespace element
	{
		struct OscillatoryKernelParameters : ElementSpecificParameters
		{
			double amplitude;
			double decay;
			double zeroCrossings;
			double amplitudeGlobal;
			bool circular;
			bool normalized;

			OscillatoryKernelParameters(double amplitude = 3.0, double decay = 0.8,
				double zeroCrossings = 1.0, double amplitudeGlobal = -0.01,
				bool circular = true, bool normalized = true)
				: amplitude(amplitude), decay(decay),
				zeroCrossings(zeroCrossings), amplitudeGlobal(amplitudeGlobal),
				circular(circular), normalized(normalized)
			{
				// zero crossings must be in the range [0, 1]
				if (zeroCrossings < 0.0)
					this->zeroCrossings = 0.0;
				else if (zeroCrossings > 1.0)
					this->zeroCrossings = 1.0;
			}

			bool operator==(const OscillatoryKernelParameters& other) const
			{
				constexpr double epsilon = 1e-6;

				return std::abs(amplitude - other.amplitude) < epsilon &&
					std::abs(decay - other.decay) < epsilon &&
					std::abs(zeroCrossings - other.zeroCrossings) < epsilon &&
					std::abs(amplitudeGlobal - other.amplitudeGlobal) < epsilon &&
					circular == other.circular &&
					normalized == other.normalized;
			}

			std::string toString() const override
			{
				std::ostringstream result;
				result << std::fixed << std::setprecision(2);
				result << "Parameters: ["
					<< "Amplitude: " << amplitude << ", "
					<< "Decay: " << decay << ", "
					<< "Zero crossings: " << zeroCrossings << ", "
					<< "Amplitude global: " << amplitudeGlobal << ", "
					<< "Circular: " << (circular ? "true" : "false") << ", "
					<< "Normalized: " << (normalized ? "true" : "false")
					<< "]";
				return result.str();
			}
		};

		class OscillatoryKernel : public Kernel
		{
		private:
			OscillatoryKernelParameters parameters;
		public:
			OscillatoryKernel(const ElementCommonParameters& elementCommonParameters,
				OscillatoryKernelParameters ok_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			void setParameters(const OscillatoryKernelParameters& ok_parameters);
			OscillatoryKernelParameters getParameters() const;
		};
	}
}