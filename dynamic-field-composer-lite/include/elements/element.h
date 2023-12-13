#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <ranges>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <cassert>
#include <cmath>

#include "../exceptions/exception.h"
#include "../user_interface/logger_window.h"

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

			ElementSpatialDimensionParameters(int x_max, double d_x) : x_max(x_max), size(static_cast<int>(std::round(x_max / d_x)) + 1), d_x(d_x) {}
		};

		struct ElementIdentifiers
		{
			static inline int uniqueIdentifierCounter = 0;
			int uniqueIdentifier;
			std::string uniqueName;
			ElementLabel label;

			ElementIdentifiers() : uniqueIdentifier(uniqueIdentifierCounter++), uniqueName(""), label(ElementLabel::UNINITIALIZED) {}
		};

		struct ElementCommonParameters
		{
			ElementIdentifiers identifiers;
			ElementSpatialDimensionParameters dimensionParameters;
		};

		class Element
		{
		protected:
			ElementCommonParameters commonParameters;
			std::unordered_map<std::string, std::vector<double>> components;
			std::unordered_map<std::shared_ptr<Element>, std::string> inputs;
		public:
			Element(const ElementCommonParameters& parameters);

			Element(const Element&) = delete;
			Element& operator=(const Element&) = delete;
			Element(Element&&) = delete;
			Element& operator=(Element&&) = delete;

			virtual void init() = 0;
			virtual void step(double t, double deltaT) = 0;
			virtual void close() = 0;
			virtual void printParameters() = 0;

			void addInput(const std::shared_ptr<Element>& inputElement, const std::string& inputComponent = "output");
			void removeInput(const std::string& inputElementId);
			void removeInput(int uniqueId);
			bool hasInput(const std::string& inputElementName, const std::string& inputComponent);
			bool hasInput(int inputElementId, const std::string& inputComponent);
			void updateInput();

			int getMaxSpatialDimension() const;
			int getSize() const;
			double getStepSize() const;

			int getUniqueIdentifier() const;
			std::string getUniqueName() const;
			ElementLabel getLabel() const;

			std::vector<double> getComponent(const std::string& componentName);
			std::vector<double>* getComponentPtr(const std::string& componentName);
			std::vector<std::string> getComponentList() const;

			std::vector < std::shared_ptr<Element>> getInputs();

			virtual ~Element() = default;
		};
	}
}