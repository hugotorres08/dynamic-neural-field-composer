// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "elements/element_parameters.h"

namespace dnf_composer
{
	namespace element
	{
		ElementSpatialDimensionParameters::ElementSpatialDimensionParameters(int x_max, double d_x)
		: x_max(x_max), size(static_cast<int>(std::round(x_max / d_x)) + 1), d_x(d_x)
		{}

		ElementIdentifiers::ElementIdentifiers()
		: uniqueIdentifier(uniqueIdentifierCounter++), uniqueName(""), label(ElementLabel::UNINITIALIZED)
		{}

		ElementCommonParameters::ElementCommonParameters(const ElementIdentifiers& identifiers, const ElementSpatialDimensionParameters& dimensionParameters)
			: identifiers(identifiers), dimensionParameters(dimensionParameters)
		{}

	}
}