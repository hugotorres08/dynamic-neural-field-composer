// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "elements/element_parameters.h"

namespace dnf_composer
{
	namespace element
	{
		ElementSpatialDimensionParameters::ElementSpatialDimensionParameters(int x_max, double d_x)
			: x_max(x_max), size(static_cast<int>(std::round(x_max / d_x))), d_x(d_x)
		{}

		void ElementSpatialDimensionParameters::print() const
		{
			std::ostringstream logStream;
			logStream << std::left;
			logStream << "Element spatial dimension parameters" << std::endl;
			logStream << "Maximum spatial dimension size: " << x_max << std::endl;
			logStream << "Spatial dimension step size: " << d_x << std::endl;
			logStream << "Number of samples in spatial dimension: " << size;
			log(tools::logger::LogLevel::INFO, logStream.str());
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
		{}

		ElementIdentifiers::ElementIdentifiers(std::string elementName)
			: uniqueIdentifier(uniqueIdentifierCounter++), uniqueName(std::move(elementName)),
				label(ElementLabel::UNINITIALIZED)
		{}

		void ElementIdentifiers::print() const
		{
			std::ostringstream logStream;
			logStream << std::left;
			logStream << "Element identifiers" << std::endl;
			logStream << "Unique identifier: " << uniqueIdentifier << std::endl;
			logStream << "Unique name: " << uniqueName << std::endl;
			logStream << "Label: " << ElementLabelToString.at(label);
			log(tools::logger::LogLevel::INFO, logStream.str());
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

		void ElementCommonParameters::print() const
		{
			std::ostringstream logStream;
			logStream << std::left;
			logStream << "Element common parameters" << std::endl;
			log(tools::logger::LogLevel::INFO, logStream.str());
			identifiers.print();
			dimensionParameters.print();
		}
		std::string ElementCommonParameters::toString() const
		{
			std::string result;
			result += "Element common parameters\n";
			result += identifiers.toString();
			result += dimensionParameters.toString();
			return result;
		}
	}
}