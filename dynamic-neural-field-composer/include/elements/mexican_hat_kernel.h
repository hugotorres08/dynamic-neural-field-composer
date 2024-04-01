#pragma once

#include "tools/math.h"
#include <vector>
#include <string>
#include <array>

#include "kernel.h"

namespace dnf_composer
{
	namespace element
	{
		struct MexicanHatKernelParameters : ElementSpecificParameters
		{
			double sigmaExc;
			double amplitudeExc;
			double sigmaInh;
			double amplitudeInh;

			MexicanHatKernelParameters(double sigmaExc, double amplitudeExc, double sigmaInh, double amplitudeInh)
				: sigmaExc(sigmaExc), amplitudeExc(amplitudeExc), sigmaInh(sigmaInh), amplitudeInh(amplitudeInh)
			{}

			bool operator==(const MexicanHatKernelParameters& other) const
			{
				constexpr double epsilon = 1e-6;

				return std::abs(sigmaExc - other.sigmaExc) < epsilon &&
					std::abs(amplitudeExc - other.amplitudeExc) < epsilon &&
					std::abs(sigmaInh - other.sigmaInh) < epsilon &&
					std::abs(amplitudeInh - other.amplitudeInh) < epsilon;
			}
		};

		class MexicanHatKernel : public Kernel
		{
		private:
			MexicanHatKernelParameters parameters;
		public:
			MexicanHatKernel(const ElementCommonParameters& elementCommonParameters, const MexicanHatKernelParameters& mhk_parameters);
			MexicanHatKernel(const ElementCommonParameters& elementCommonParameters, const MexicanHatKernelParameters& mhk_parameters, const bool circular, const bool normalized);


			void init() override;
			void step(double t, double deltaT) override;
			void close() override;
			void printParameters() override;
			std::shared_ptr<Element> clone() const override;

			void setParameters(const MexicanHatKernelParameters& mhk_parameters);
			MexicanHatKernelParameters getParameters() const;

			~MexicanHatKernel() override = default;
		private:
			void updateParameters();
		};
	}
}
