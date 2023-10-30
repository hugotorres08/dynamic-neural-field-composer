#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <cassert>
#include <cmath>

#include "../exceptions/exception.h"

enum ElementLabel
{
	UNINITIALIZED = 0,
	NEURAL_FIELD,
	GAUSS_STIMULUS,
	GAUSS_KERNEL,
	MEXICAN_HAT_KERNEL,
	NORMAL_NOISE,
	SUM_DIMENSION,
	FIELD_COUPLING
};

const std::map<ElementLabel, std::string> ElementLabelToString = {
	{ NEURAL_FIELD, "neural field" },
	{ GAUSS_STIMULUS, "gauss stimulus" },
	{ FIELD_COUPLING, "field coupling" },
	{ GAUSS_KERNEL, "gauss kernel" },
	{ MEXICAN_HAT_KERNEL, "mexican hat kernel" },
	{ NORMAL_NOISE, "normal noise" },
	{ SUM_DIMENSION, "sum dimension" },
};


class Element
{
protected:
	std::string uniqueIdentifier;
	ElementLabel label;
	int size;
	std::unordered_map<std::string, std::vector<double>> components;
	std::unordered_map<std::shared_ptr<Element>, std::string> inputs;
public:
	Element();
	virtual void init() = 0;
	virtual void step(const double& t, const double& deltaT) = 0;
	virtual void close() = 0;

	void addInput(const std::shared_ptr<Element>& inputElement, const std::string& inputComponent = "output");
	void removeInput(const std::string& inputElementId);
	bool hasInput(const std::string& inputElementId, const std::string& inputComponent);
	void updateInput();

	void setUniqueIdentifier(const std::string& uniqueIdentifier);
	void setSize(uint8_t size);

	int getSize() const;
	std::string getUniqueIdentifier() const;
	ElementLabel getLabel() const;
	std::vector<double> getComponent(const std::string& componentName);
	std::vector<double>* getComponentPtr(const std::string& componentName);
	std::vector < std::shared_ptr<Element>> getInputs();

	virtual ~Element() = default;
};
