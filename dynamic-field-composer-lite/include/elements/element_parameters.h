#pragma once

#include <map>
#include <string>
#include <format>

namespace dnf_composer
{
	namespace element
	{
		enum ElementLabel : int
		{
			UNINITIALIZED,
			NEURAL_FIELD,
			GAUSS_STIMULUS,
			GAUSS_KERNEL,
			MEXICAN_HAT_KERNEL,
			NORMAL_NOISE,
			FIELD_COUPLING,
			GAUSS_FIELD_COUPLING
		};

		inline const std::map<ElementLabel, std::string> ElementLabelToString = {
			{NEURAL_FIELD, "neural field" },
			{GAUSS_STIMULUS, "gauss stimulus" },
			{GAUSS_FIELD_COUPLING, "gauss field coupling" },
			{FIELD_COUPLING, "field coupling" },
			{GAUSS_KERNEL, "gauss kernel" },
			{MEXICAN_HAT_KERNEL, "mexican hat kernel" },
			{NORMAL_NOISE, "normal noise" },
		};

		struct ElementSpatialDimensionParameters
		{
			int x_max, size;
			double d_x;
			ElementSpatialDimensionParameters(int x_max, double d_x);
		};

		struct ElementIdentifiers
		{
			static inline int uniqueIdentifierCounter = 0;
			int uniqueIdentifier;
			std::string uniqueName;
			ElementLabel label;

			ElementIdentifiers();
		};

		struct ElementCommonParameters
		{
			ElementIdentifiers identifiers;
			ElementSpatialDimensionParameters dimensionParameters;

			ElementCommonParameters(const ElementIdentifiers& identifiers, const ElementSpatialDimensionParameters& dimensionParameters);
		};
	}
}
