#pragma once

#include "element.h"
#include "mathtools/mathtools.h"



struct WeightedCoupling
{
	double x_i, x_j, w_i_j;
};

struct GaussFieldCouplingParameters
{
	int inputFieldSize;
	double sigma;
	std::vector<WeightedCoupling> couplings;

	void addCoupling(const WeightedCoupling& coupling)
	{
		couplings.emplace_back(coupling);
	}
};

class GaussFieldCoupling : public Element
{
private:
	GaussFieldCouplingParameters gfcp;
public:
	GaussFieldCoupling(const std::string& id, const int& size, const GaussFieldCouplingParameters& gfcp);

	void addCoupling(const WeightedCoupling& coupling);
	void init() override;
	void step(const double& t, const double& deltaT) override;
	void close() override;

	~GaussFieldCoupling() override = default;
private:
	void updateOutput();
};