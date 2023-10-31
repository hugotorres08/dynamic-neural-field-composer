#pragma once

#include <iostream>

#include "mathtools/mathtools.h"
#include "element.h"

struct GaussStimulusParameters
{
	double sigma;
	double amplitude;
	double position;
	bool circular = true;
	bool normalized = false;

	// Overload the == operator
	bool operator==(const GaussStimulusParameters& other) const {
		constexpr double epsilon = 1e-6; // Set an appropriate epsilon value

		// Compare floating-point values with tolerance (epsilon)
		return std::abs(sigma - other.sigma) < epsilon &&
			std::abs(position - other.position) < epsilon &&
			std::abs(amplitude - other.amplitude) < epsilon &&
			std::abs(circular - other.circular) < epsilon &&
			std::abs(normalized - other.normalized) < epsilon;
	}
};

class GaussStimulus : public Element
{
private:
	GaussStimulusParameters parameters;
public:
	GaussStimulus(const std::string& id, const int& size,
		const GaussStimulusParameters& parameters);
	void init() override;
	void step(const double& t, const double& deltaT) override;
	void close() override;
	void setParameters(const GaussStimulusParameters& parameters);
	GaussStimulusParameters getParameters() const;
	~GaussStimulus() override = default;
};