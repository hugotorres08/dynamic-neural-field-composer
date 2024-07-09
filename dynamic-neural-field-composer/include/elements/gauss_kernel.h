#pragma once

#include <array>

#include "tools/math.h"
#include "kernel.h"


namespace dnf_composer
{
	namespace element
	{
		struct GaussKernelParameters : ElementSpecificParameters
		{
			double width;
			double amplitude;
			double amplitudeGlobal;
			bool circular;
			bool normalized;

			GaussKernelParameters(double width = 3.0, double amp = 3.0, double ampGlobal = -0.01,
				bool circular = true, bool normalized = true)
				: width(width), amplitude(amp), amplitudeGlobal(ampGlobal),
					circular(circular), normalized(normalized)
			{}

			bool operator==(const GaussKernelParameters& other) const {
				constexpr double epsilon = 1e-6;

				return std::abs(width - other.width) < epsilon &&
					std::abs(amplitude - other.amplitude) < epsilon &&
					std::abs(amplitudeGlobal - other.amplitudeGlobal) < epsilon &&
					circular == other.circular &&
					normalized == other.normalized;
			}

			std::string toString() const override
			{
				std::string result = "Gauss kernel parameters\n";
				result += "Width: " + std::to_string(width) + "\n";
				result += "Amplitude: " + std::to_string(amplitude) + "\n";
				result += "Amplitude global: " + std::to_string(amplitudeGlobal) + "\n";
				result += "Circular: " + std::to_string(circular) + "\n";
				result += "Normalized: " + std::to_string(normalized) + "\n";
				return result;
			}
		};

		class GaussKernel : public Kernel
		{
		private:
			GaussKernelParameters parameters;
		public:
			GaussKernel(const ElementCommonParameters& elementCommonParameters, GaussKernelParameters parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			void setParameters(const GaussKernelParameters& gk_parameters);
			GaussKernelParameters getParameters() const;
		};
	}
}