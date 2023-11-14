#pragma once

#include <string>

#include "../elements/element_factory.h"
#include "simulation/simulation.h"

struct DynamicNeuralField
{
	std::shared_ptr<NeuralField> neuralField;
	std::shared_ptr<Kernel> kernel;
	std::shared_ptr<NormalNoise> normalNoise;
	std::shared_ptr<GaussKernel> normalNoiseKernel;
	int size;
	int kernelType;
};

struct DynamicNeuralFieldCoupling
{
	std::shared_ptr<GaussFieldCoupling> gaussianFieldCoupling;
	std::vector<WeightedCoupling> couplings;
	std::array<int, 2> idsOfCoupledDynamicNeuralFields;
};

struct ArchitectureFileLocations
{
	std::string architectureFileLocation;
	std::string fieldCouplingFileLocation;
};

class ArchitectureBuilder
{
private:
	std::string identifier;
	std::shared_ptr<Simulation> simulation;
	std::vector<std::string> unparsedDynamicNeuralFieldParameters;
	std::vector<std::string> unparsedDynamicNeuralFieldCouplingsParameters;
	ElementParameters architectureParameters;
	DynamicNeuralField dynamicNeuralField;
	DynamicNeuralFieldCoupling dynamicNeuralFieldCoupling;
	ArchitectureFileLocations files;
public:
	ArchitectureBuilder(std::string identifier, const std::shared_ptr<Simulation>& simulation);
	ArchitectureBuilder(const ArchitectureBuilder& other);
	ArchitectureBuilder(ArchitectureBuilder&& other) noexcept;
	ArchitectureBuilder& operator=(const ArchitectureBuilder& other);
	ArchitectureBuilder& operator=(ArchitectureBuilder&& other) noexcept;
	~ArchitectureBuilder() = default;

	void readArchitecture();
	void saveArchitecture();
private:
	void readDynamicNeuralFieldParameters();
	void parseDynamicNeuralFieldParameters();
	void createDynamicNeuralFieldElements();
	void addElementsToSimulation() const;
	void setupInteractionsBetweenElements() const;
	void clearDynamicNeuralFieldParametersAndDynamicNeuralField();

	void readDynamicNeuralFieldCouplingsParameters();
	void parseDynamicNeuralFieldCouplingsParameters();
	void createDynamicNeuralFieldCouplings();
	void addDynamicNeuralFieldCouplingsToSimulation() const;
	void setupInteractionBetweenFields() const;
	void clearDynamicNeuralFieldCouplingsParameters();

	//void saveNeuralFieldParameters() const;
	//void saveGaussKernelParameters() const;
	//void saveMexicanHatKernelParameters() const;
	//void saveNormalNoiseParameters() const;
	//void saveFieldCouplingParameters() const;
};