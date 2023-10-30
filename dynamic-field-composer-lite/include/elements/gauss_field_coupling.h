#pragma once

#include "element.h"
#include "mathtools/mathtools.h"


struct WeightedCoupling
{
	double x_i, x_j, w_i_j;
};

class GaussFieldCoupling : public Element
{
private:
	std::vector<WeightedCoupling> couplings;

public:
	GaussFieldCoupling(const std::string& id, const int& size);

	void addCoupling(const WeightedCoupling& coupling);
	void init() override;
	void step(const double& t, const double& deltaT) override;
	void close() override;

	~GaussFieldCoupling() override = default;
private:
	void updateOutput();
};