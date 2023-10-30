#pragma once

#include "../mathtools/mathtools.h"
#include "element.h"
#include <unordered_set>


enum class ActivationFunctionType
{
	Sigmoid,
	Heaviside,
};

struct ActivationFunctionParameters
{
	ActivationFunctionType type;
	double steepness;
	double xShift;

	bool operator==(const ActivationFunctionParameters& other) const
	{
		return type == other.type &&
			steepness == other.steepness &&
			xShift == other.xShift;
	}
};

struct NeuralFieldParameters
{
	double tau;
	double startingRestingLevel;
	ActivationFunctionParameters activationFunctionParameters;

	// Overload the == operator
	bool operator==(const NeuralFieldParameters& other) const
	{
		return tau == other.tau &&
			startingRestingLevel == other.startingRestingLevel;
	}
};

class NeuralField : public Element
{
protected:
	NeuralFieldParameters parameters;
public:
	NeuralField(const std::string& id, const int& size,
		const NeuralFieldParameters& parameters);
	void init() override;
	void step(const double& t, const double& deltaT) override;
	void close() override;

	double calculateCentroid();

	void setParameters(const NeuralFieldParameters& parameters);
	NeuralFieldParameters getParameters() const;

	//~NeuralField() = default;

protected:
	void calculateActivation(const double& t, const double& deltaT);
	void calculateOutput();
};