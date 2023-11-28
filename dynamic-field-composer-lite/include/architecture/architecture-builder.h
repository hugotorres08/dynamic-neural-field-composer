#pragma once

#include <string>
#include <deque>

#include "../elements/element_factory.h"
#include "simulation/simulation.h"

#include "architecture-file-handler.h"

namespace dnf_composer
{
	struct DynamicNeuralField
{
	std::shared_ptr<element::NeuralField> neuralField;
	std::shared_ptr<element::Kernel> kernel;
	std::shared_ptr<element::NormalNoise> normalNoise;
	std::shared_ptr<element::GaussKernel> normalNoiseKernel;
	int size;
	int kernelType;
};

	struct DynamicNeuralFieldCoupling
	{
		std::shared_ptr<element::GaussFieldCoupling> gaussianFieldCoupling;
		std::vector<WeightedCoupling> couplings;
		std::array<int, 2> idsOfCoupledDynamicNeuralFields;
	};


	class ArchitectureBuilder
	{
	private:
		std::string identifier;
		std::shared_ptr<Simulation> simulation;

		ArchitectureFileHandler fileHandler;

		element::ElementParameters architectureParameters;
		DynamicNeuralField dynamicNeuralField;
		DynamicNeuralFieldCoupling dynamicNeuralFieldCoupling;
	public:
		ArchitectureBuilder(std::string identifier, const std::shared_ptr<Simulation>& simulation);
		ArchitectureBuilder(const ArchitectureBuilder& other);
		ArchitectureBuilder(ArchitectureBuilder&& other) noexcept;
		ArchitectureBuilder& operator=(const ArchitectureBuilder& other);
		ArchitectureBuilder& operator=(ArchitectureBuilder&& other) noexcept;
		~ArchitectureBuilder() = default;

		void readArchitecture();
		void saveArchitecture();

		void createArchitecture();

		void setArchitectureParameters(const element::ElementParameters& elementParameters);
		void setDynamicNeuralField(const DynamicNeuralField& dynamicNeuralField);
		void setDynamicNeuralFieldCoupling(const DynamicNeuralFieldCoupling& coupling);

		element::ElementParameters getArchitectureParameters() const;
		DynamicNeuralField getDynamicNeuralField() const;
		DynamicNeuralFieldCoupling getDynamicNeuralFieldCoupling() const;
		std::string getIdentifier() const;

	private:
		void parseDynamicNeuralFieldParameters(const std::string& line);
		void createDynamicNeuralFieldElements();
		void addElementsToSimulation() const;
		void setupInteractionsBetweenElements() const;
		void clearDynamicNeuralFieldParametersAndDynamicNeuralField();

		void parseDynamicNeuralFieldCouplingsParameters(const std::string& line);
		void createDynamicNeuralFieldCouplings();
		void addDynamicNeuralFieldCouplingsToSimulation() const;
		void setupInteractionBetweenFields() const;
		void clearDynamicNeuralFieldCouplingsParameters();
	};
}
