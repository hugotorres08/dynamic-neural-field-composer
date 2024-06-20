#pragma once

#include "element_parameters.h"

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
				bool circular = true, bool normalized = true);
			bool operator==(const LateralInteractionsParameters& other) const;
			std::string toString() const override;

		};
	}
}
		