#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class ArchitectureFileHandler
{
private:
	std::string architectureFileLocation;
	std::string fieldCouplingFileLocation;

	std::vector<std::string> unparsedDynamicNeuralFieldParameters;
	std::vector<std::string> unparsedDynamicNeuralFieldCouplingsParameters;

	std::string dynamicNeuralFieldParametersToSave;
	std::string dynamicNeuralFieldCouplingsParametersToSave;

public:
	ArchitectureFileHandler(const std::string& identifier);
	ArchitectureFileHandler(const ArchitectureFileHandler& other);
	ArchitectureFileHandler(ArchitectureFileHandler&& other) noexcept;
	ArchitectureFileHandler& operator=(const ArchitectureFileHandler& other);
	ArchitectureFileHandler& operator=(ArchitectureFileHandler&& other) noexcept;
	~ArchitectureFileHandler() = default;

	void readArchitectureFile();
	void saveArchitectureFile();

	std::vector<std::string> getUnparsedDynamicNeuralFieldParameters();
	std::vector<std::string> getUnparsedDynamicNeuralFieldCouplingsParameters();

	void setDynamicNeuralFieldParametersToSave(const std::string& parameters);
	void setDynamicNeuralFieldCouplingsParametersToSave(const std::string& parameters);

private:
	void readDynamicNeuralFieldParameters();
	void readDynamicNeuralFieldCouplingsParameters();

	void saveDynamicNeuralFieldParameters() const;
	void saveDynamicNeuralFieldCouplingsParameters() const;
};