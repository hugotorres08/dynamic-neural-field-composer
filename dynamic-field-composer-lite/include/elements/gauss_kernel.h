#pragma once

#include <array>

#include "mathtools/mathtools.h"
#include "element.h"

struct GaussKernelParameters
{
	double sigma;
	double amplitude;
	double amplitudeGlobal;
	double fullSum;
	int cutOfFactor = 5;
	
	bool operator==(const GaussKernelParameters& other) const {
		constexpr double epsilon = 1e-6; // Set an appropriate epsilon value

		// Compare floating-point values with tolerance (epsilon)
		return std::abs(sigma - other.sigma) < epsilon &&
			std::abs(amplitude - other.amplitude) < epsilon &&
			std::abs(amplitudeGlobal - other.amplitudeGlobal) < epsilon &&
			std::abs(fullSum - other.fullSum) < epsilon &&
			std::abs(cutOfFactor - other.cutOfFactor) < epsilon;
	}
};

class GaussKernel : public Element
{
private:
	GaussKernelParameters parameters;
	bool circular;
	bool normalized;
	std::array<uint32_t, 2> kernelRange;
	std::vector<uint32_t> extIndex;
public:
	GaussKernel(const std::string& id, const int& size,
		const GaussKernelParameters& parameters,
		bool circular = true, bool normalized = true);

	void init() override;
	void step(const double& t, const double& deltaT) override;
	void close() override;

	void setParameters(const GaussKernelParameters& parameters);
	GaussKernelParameters getParameters() const;

	~GaussKernel() override = default;
};