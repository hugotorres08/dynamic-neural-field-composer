#pragma once

#include <array>

#include "tools/math.h"
#include "kernel.h"
#include "tools/convolution_std.h"
#include "tools/convolution_fftw.h"


namespace dnf_composer
{
	namespace element
	{
		struct GaussKernelParameters
		{
			double sigma = 5.0;
			double amplitude = 10.0;
			
			bool operator==(const GaussKernelParameters& other) const {
				constexpr double epsilon = 1e-6; // Set an appropriate epsilon value

				// Compare floating-point values with tolerance (epsilon)
				return std::abs(sigma - other.sigma) < epsilon &&
					std::abs(amplitude - other.amplitude) < epsilon;
			}
		};

		class GaussKernel : public Kernel
		{
		private:
			GaussKernelParameters parameters;
		public:
			GaussKernel(const ElementCommonParameters& elementCommonParameters, const GaussKernelParameters& parameters);

			void init() override;
			void step( double t,  double deltaT) override;
			void close() override;

			void printParameters() override;

			void setParameters(const GaussKernelParameters& gk_parameters);
			GaussKernelParameters getParameters() const;

			~GaussKernel() override = default;
		};
	}
}
