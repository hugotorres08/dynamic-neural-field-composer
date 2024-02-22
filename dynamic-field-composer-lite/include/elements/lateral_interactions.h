#pragma once


#include "kernel.h"
#include "mexican_hat_kernel.h"

namespace dnf_composer
{
	namespace element
	{
		struct LateralInteractionsParameters : ElementSpecificParameters
		{
			double sigmaExc;
			double amplitudeExc;
			double sigmaInh;
			double amplitudeInh;
			double amplitudeGlobal;

			LateralInteractionsParameters(double sigmaExc, double amplitudeExc, double sigmaInh, double amplitudeInh, double amplitudeGlobal)
				: sigmaExc(sigmaExc), amplitudeExc(amplitudeExc), sigmaInh(sigmaInh), amplitudeInh(amplitudeInh), amplitudeGlobal(amplitudeGlobal)
			{}

			bool operator==(const LateralInteractionsParameters& other) const
			{
				constexpr double epsilon = 1e-6;

				return std::abs(sigmaExc - other.sigmaExc) < epsilon &&
					std::abs(amplitudeExc - other.amplitudeExc) < epsilon &&
					std::abs(sigmaInh - other.sigmaInh) < epsilon &&
					std::abs(amplitudeInh - other.amplitudeInh) < epsilon;
			}
		};

		class LateralInteractions : public Kernel
		{
		private:
			LateralInteractionsParameters parameters;
		public:
			LateralInteractions(const ElementCommonParameters& elementCommonParameters, const LateralInteractionsParameters& li_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			void close() override;
			void printParameters() override;
			std::shared_ptr<Element> clone() const override;

			void setParameters(const LateralInteractionsParameters& li_parameters);
			LateralInteractionsParameters getParameters() const;

			~LateralInteractions() override = default;
		private:
			void updateParameters();
		};
	}
}
