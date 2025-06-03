#pragma once

#include "kernel.h"
#include "tools/math.h"

namespace dnf_composer
{
	namespace element
	{
		struct AsymmetricGaussKernelParameters : ElementSpecificParameters
		{
			double width;
			double amplitude;
			double amplitudeGlobal;
			double timeShift;
			bool circular;
			bool normalized;

			AsymmetricGaussKernelParameters(double width = 3.0, double amp = 3.0, double ampGlobal = 0.00, double timeShift = 0.00,
				bool circular = true, bool normalized = true)
				: width(width), amplitude(amp), amplitudeGlobal(ampGlobal), timeShift(timeShift),
				circular(circular), normalized(normalized)
			{}

			bool operator==(const AsymmetricGaussKernelParameters& other) const {
				constexpr double epsilon = 1e-6;

				return std::abs(width - other.width) < epsilon &&
					std::abs(amplitude - other.amplitude) < epsilon &&
					std::abs(amplitudeGlobal - other.amplitudeGlobal) < epsilon &&
					std::abs(timeShift - other.timeShift) < epsilon &&
					circular == other.circular &&
					normalized == other.normalized;
			}

			std::string toString() const override
			{
				std::ostringstream result;
				result << std::fixed << std::setprecision(2); 
				result << "Parameters: ["
					<< "Width: " << width << ", "
					<< "Amplitude: " << amplitude << ", "
					<< "Amplitude global: " << amplitudeGlobal << ", "
					<< "Time shift: " << timeShift << ", "
					<< "Circular: " << (circular ? "true" : "false") << ", "
					<< "Normalized: " << (normalized ? "true" : "false")
					<< "]";
				return result.str();
			}
		};

		class AsymmetricGaussKernel : public Kernel
		{
		private:
			AsymmetricGaussKernelParameters parameters;
			std::vector<double> gauss;
			std::vector<double> gaussDerivative;
		public:
			AsymmetricGaussKernel(const ElementCommonParameters& elementCommonParameters, AsymmetricGaussKernelParameters agk_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			void setParameters(const AsymmetricGaussKernelParameters& gk_parameters);
			AsymmetricGaussKernelParameters getParameters() const;
		};
	}
}