#pragma once

#include <string>

#include "../elements/element_factory.h"
#include "simulation/simulation.h"

struct DynamicNeuralField
{
	std::shared_ptr<NeuralField> nf;
	std::shared_ptr<NormalNoise> nn;
	std::shared_ptr<Kernel> k;
	int size;
	int kernelType;
};

class ArchitectureBuilder
{
private:
	std::string identifier;
	std::shared_ptr<Simulation> simulation;
	std::vector<std::string> unparsedDynamicNeuralFieldParameters;
	ElementParameters dynamicNeuralFieldParameters;
	DynamicNeuralField dynamicNeuralField;
public:
	ArchitectureBuilder(std::string identifier, const std::shared_ptr<Simulation>& simulation);
	ArchitectureBuilder(const ArchitectureBuilder& other);
	ArchitectureBuilder(ArchitectureBuilder&& other) noexcept;
	ArchitectureBuilder& operator=(const ArchitectureBuilder& other);
	ArchitectureBuilder& operator=(ArchitectureBuilder&& other) noexcept;
	~ArchitectureBuilder() = default;

	void generate();
private:
	void readDynamicNeuralFieldParameters();
	void parseDynamicNeuralFieldParameters();
	void createDynamicNeuralFieldElements();
	void addElementsToSimulation() const;
	void setupInteractionsBetweenElements() const;
	void clearDynamicNeuralFieldParametersAndDynamicNeuralField();
};