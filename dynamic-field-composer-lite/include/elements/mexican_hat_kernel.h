#pragma once

#include "mathtools/mathtools.h"
#include <vector>
#include <string>
#include <array>

#include "kernel.h"

struct MexicanHatKernelParameters
{
	double sigmaExc;
	double amplitudeExc;
	double sigmaInh;
	double amplitudeInh;
	double amplitudeGlobal;
	double fullSum;
	int cutOfFactor;

	bool operator==(const MexicanHatKernelParameters& other) const
	{
		return sigmaExc == other.sigmaExc &&
			amplitudeExc == other.amplitudeExc &&
			sigmaInh == other.sigmaInh && 
			amplitudeInh == other.amplitudeInh &&
			amplitudeGlobal == other.amplitudeGlobal &&
			fullSum == other.fullSum &&
			cutOfFactor == other.cutOfFactor;
	}
};

class MexicanHatKernel : public Kernel
{
private:
	MexicanHatKernelParameters parameters;
public:
	MexicanHatKernel(const std::string& id, const int& size,
		const MexicanHatKernelParameters& parameters);

	void init() override;
	void step(const double& t, const double& deltaT) override;
	void close() override;

	void setParameters(const MexicanHatKernelParameters& parameters);
	MexicanHatKernelParameters getParameters() const;

	~MexicanHatKernel() override = default;
};