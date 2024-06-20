#pragma once


#include "kernel.h"
#include "mexican_hat_kernel.h"

namespace dnf_composer
{
	namespace element
	{
		struct LateralInteractionsParameters : ElementSpecificParameters
		{
			double widthExc;
			double amplitudeExc;
			double widthInh;
			double amplitudeInh;
			double amplitudeGlobal;
			bool circular;
			bool normalized;

			LateralInteractionsParameters(double widthExc, double amplitudeExc,
				double widthInh, double amplitudeInh,
				double amplitudeGlobal,
				bool circular = true, bool normalized = true)
				: widthExc(widthExc), amplitudeExc(amplitudeExc),
				widthInh(widthInh), amplitudeInh(amplitudeInh),
				amplitudeGlobal(amplitudeGlobal),
				circular(circular), normalized(normalized)
			{}

			bool operator==(const LateralInteractionsParameters& other) const
			{
				constexpr double epsilon = 1e-6;

				return std::abs(widthExc - other.widthExc) < epsilon &&
					std::abs(amplitudeExc - other.amplitudeExc) < epsilon &&
					std::abs(widthInh - other.widthInh) < epsilon &&
					std::abs(amplitudeInh - other.amplitudeInh) < epsilon &&
					std::abs(amplitudeGlobal - other.amplitudeGlobal) < epsilon &&
					circular == other.circular &&
					normalized == other.normalized;
			}

			std::string toString() const override
			{
				std::string result = "Lateral interactions kernel parameters\n";
				result += "Width excitation: " + std::to_string(widthExc) + "\n";
				result += "Amplitude excitation: " + std::to_string(amplitudeExc) + "\n";
				result += "Width inhibition: " + std::to_string(widthInh) + "\n";
				result += "Amplitude inhibition: " + std::to_string(amplitudeInh) + "\n";
				result += "Amplitude global: " + std::to_string(amplitudeGlobal) + "\n";
				result += "Circular: " + std::to_string(circular) + "\n";
				result += "Normalized: " + std::to_string(normalized) + "\n";
				return result;
			}

		};

		class LateralInteractions : public Kernel
		{
		private:
			LateralInteractionsParameters parameters;
		public:
			LateralInteractions(const ElementCommonParameters& elementCommonParameters,
				LateralInteractionsParameters li_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			void setParameters(const LateralInteractionsParameters& li_parameters);
			LateralInteractionsParameters getParameters() const;
		};
	}
}