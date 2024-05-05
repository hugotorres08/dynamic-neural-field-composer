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
			double sigma;
			double amplitude;
			bool circular;
			bool normalized;

			GaussKernelParameters(double sigma = 5.0, double amp = 10.0, bool circular = true, bool normalized = true)
				: sigma(sigma), amplitude(amp), circular(circular), normalized(normalized)
			{}

			bool operator==(const GaussKernelParameters& other) const {
				constexpr double epsilon = 1e-6; 

				return std::abs(sigma - other.sigma) < epsilon &&
					std::abs(amplitude - other.amplitude) < epsilon &&
					circular == other.circular &&
					normalized == other.normalized;
			}
		};

		class GaussKernel : public Kernel
		{
		private:
			GaussKernelParameters parameters;
		public:
			GaussKernel(const ElementCommonParameters& elementCommonParameters, GaussKernelParameters parameters);

			void init() override;
			void step( double t,  double deltaT) override;
			void close() override;
			void printParameters() override;
			std::shared_ptr<Element> clone() const override;


			void setParameters(const GaussKernelParameters& gk_parameters);
			GaussKernelParameters getParameters() const;
		};
	}
}
