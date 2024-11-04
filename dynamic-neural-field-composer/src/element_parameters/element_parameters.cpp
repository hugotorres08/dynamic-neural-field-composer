// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "element_parameters/element_parameters.h"

namespace dnf_composer
{
	namespace element
	{
		ElementSpatialDimensionParameters::ElementSpatialDimensionParameters(int x_max, double d_x)
			: x_max(x_max), size(static_cast<int>(std::round(x_max / d_x))), d_x(d_x)
		{}

		bool ElementSpatialDimensionParameters::operator==(const ElementSpatialDimensionParameters& other) const
		{
			constexpr double epsilon = 1e-6;
			return x_max == other.x_max && std::abs(d_x - other.d_x) < epsilon;
		}

		void ElementSpatialDimensionParameters::print() const
		{
			tools::logger::log(tools::logger::LogLevel::INFO, toString());
		}

		std::string ElementSpatialDimensionParameters::toString() const
		{
			std::string result;
			result += "Element spatial dimension parameters\n";
			result += std::format("Maximum spatial dimension size: {}\n", x_max);
			result += std::format("Spatial dimension step size: {}\n", d_x);
			result += std::format("Number of samples in spatial dimension: {}\n", size);
			return result;
		}

		ElementIdentifiers::ElementIdentifiers()
			: uniqueIdentifier(uniqueIdentifierCounter++), label(ElementLabel::UNINITIALIZED)
		{
			uniqueName = "Element " + std::to_string(uniqueIdentifier);
		}

		ElementIdentifiers::ElementIdentifiers(std::string elementName)
			: uniqueIdentifier(uniqueIdentifierCounter++), uniqueName(std::move(elementName)),
				label(ElementLabel::UNINITIALIZED)
		{}

		bool ElementIdentifiers::operator==(const ElementIdentifiers& other) const
		{
			return uniqueIdentifier == other.uniqueIdentifier && uniqueName == other.uniqueName &&
				label == other.label;
		}

		void ElementIdentifiers::print() const
		{
			tools::logger::log(tools::logger::LogLevel::INFO, toString());
		}

		std::string ElementIdentifiers::toString() const
		{
			std::string result;
			result += "Element identifiers\n";
			result += std::format("Unique identifier: {}\n", uniqueIdentifier);
			result += std::format("Unique name: {}\n", uniqueName);
			result += std::format("Label: {}\n", ElementLabelToString.at(label));
			return result;
		}

		ElementCommonParameters::ElementCommonParameters()
			: identifiers(), dimensionParameters()
		{}

		ElementCommonParameters::ElementCommonParameters(const std::string& elementName)
			: identifiers(elementName), dimensionParameters()
		{}

		ElementCommonParameters::ElementCommonParameters(const std::string& elementName, int x_max)
			: identifiers(elementName), dimensionParameters(x_max)
		{}

		ElementCommonParameters::ElementCommonParameters(const std::string& elementName, 
			const ElementSpatialDimensionParameters& dimensionParameters)
			: identifiers(elementName), dimensionParameters(dimensionParameters)
		{}

		ElementCommonParameters::ElementCommonParameters(ElementIdentifiers identifiers, 
			const ElementSpatialDimensionParameters& dimensionParameters)
			: identifiers(std::move(identifiers)), dimensionParameters(dimensionParameters)
		{}

		bool ElementCommonParameters::operator==(const ElementCommonParameters& other) const
		{
			return identifiers == other.identifiers && dimensionParameters == other.dimensionParameters;
		}

		void ElementCommonParameters::print() const
		{
			tools::logger::log(tools::logger::LogLevel::INFO, toString());
		}

		std::string ElementCommonParameters::toString() const
		{
			std::string result;
			result += "Element common parameters\n";
			result += identifiers.toString();
			result += dimensionParameters.toString();
			return result;
		}

		void ElementSpecificParameters::print() const
		{
			log(tools::logger::LogLevel::INFO, toString());
		}

	}
}