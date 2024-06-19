#pragma once

#include <vector>
#include <string>
#include <array>

#include "kernel.h"
#include "tools/math.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


namespace dnf_composer
{
	namespace element
	{
		struct MexicanHatKernelParameters : ElementSpecificParameters
		{
			double widthExc;
			double amplitudeExc;
			double widthInh;
			double amplitudeInh;
			bool circular;
			bool normalized;

			MexicanHatKernelParameters(double widthExc, double amplitudeExc, double widthInh, 
				double amplitudeInh, bool circular = true, bool normalized = true)
				: widthExc(widthExc), amplitudeExc(amplitudeExc), widthInh(widthInh),
					amplitudeInh(amplitudeInh), circular(circular), normalized(normalized)
			{}

			bool operator==(const MexicanHatKernelParameters& other) const
			{
				constexpr double epsilon = 1e-6;

				return std::abs(widthExc - other.widthExc) < epsilon &&
					std::abs(amplitudeExc - other.amplitudeExc) < epsilon &&
					std::abs(widthInh - other.widthInh) < epsilon &&
					std::abs(amplitudeInh - other.amplitudeInh) < epsilon &&
					circular == other.circular &&
					normalized == other.normalized;
			}

			std::string toString() const override
			{
				std::string result = "Mexican-hat kernel parameters\n";
				result += "Width exc: " + std::to_string(widthExc) + "\n";
				result += "Amplitude exc: " + std::to_string(amplitudeExc) + "\n";
				result += "Width inh: " + std::to_string(widthInh) + "\n";
				result += "Amplitude inh: " + std::to_string(amplitudeInh) + "\n";
				result += "Circular: " + std::to_string(circular) + "\n";
				result += "Normalized: " + std::to_string(normalized) + "\n";
				return result;
			}
		};

		class MexicanHatKernel : public Kernel
		{
		private:
			MexicanHatKernelParameters parameters;
		public:
			MexicanHatKernel(const ElementCommonParameters& elementCommonParameters, 
				MexicanHatKernelParameters mhk_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			void setParameters(const MexicanHatKernelParameters& mhk_parameters);
			MexicanHatKernelParameters getParameters() const;
		};
	}
}
