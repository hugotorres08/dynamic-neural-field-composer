#pragma once

#include "../mathtools/mathtools.h"
#include "element.h"
#include <unordered_set>


enum class ActivationFunctionType
{
	Sigmoid,
	Heaviside,
};

struct ActivationFunctionParameters {
    ActivationFunctionType type;
    double steepness;
    double xShift;

    bool operator==(const ActivationFunctionParameters& other) const {
        constexpr double epsilon = 1e-6; // Set an appropriate epsilon value

        // Compare floating-point values with tolerance (epsilon)
        return type == other.type &&
            std::abs(steepness - other.steepness) < epsilon &&
            std::abs(xShift - other.xShift) < epsilon;
    }
};

struct NeuralFieldParameters {
    double tau;
    double startingRestingLevel;
    ActivationFunctionParameters activationFunctionParameters;

    // Overload the == operator
    bool operator==(const NeuralFieldParameters& other) const {
        constexpr double epsilon = 1e-6; // Set an appropriate epsilon value

        // Compare floating-point values with tolerance (epsilon)
        return std::abs(tau - other.tau) < epsilon &&
            std::abs(startingRestingLevel - other.startingRestingLevel) < epsilon &&
            activationFunctionParameters == other.activationFunctionParameters;
    }
};

class NeuralField : public Element
{
protected:
	NeuralFieldParameters parameters;
    double centroid;
public:
	NeuralField(const std::string& id, const int& size,
		const NeuralFieldParameters& parameters);
	void init() override;
	void step(const double& t, const double& deltaT) override;
	void close() override;


	void setParameters(const NeuralFieldParameters& parameters);
	NeuralFieldParameters getParameters() const;
    double getCentroid() const;

	~NeuralField() override = default;

protected:
	void calculateActivation(const double& t, const double& deltaT);
	void calculateOutput();
	void calculateCentroid();
};