
#pragma once

#include "mathtools/mathtools.h"

#include "element.h"
 
struct NormalNoiseParameters
{
	double amplitude;
	bool operator==(const NormalNoiseParameters& other) const
	{
		//comparing floating-point numbers using the equality
		//(==) operator can be problematic due to precision issues
		constexpr double epsilon = 1e-6; // Set an appropriate epsilon value
		return std::abs(amplitude - other.amplitude) < epsilon;
	}
};

class NormalNoise : public Element
{
private:
	NormalNoiseParameters parameters;
public:
	NormalNoise(const std::string& id, const int& size, const NormalNoiseParameters& parameters);

	void init() override;
	void step(const double& t, const double& deltaT) override;
	void close() override {}

	void setParameters(const NormalNoiseParameters& parameters);
	NormalNoiseParameters getParameters() const;

	~NormalNoise() override = default;
};