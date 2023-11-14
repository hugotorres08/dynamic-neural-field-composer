#include "elements/gauss_field_coupling.h"



GaussFieldCoupling::GaussFieldCoupling(const std::string& id, const int& size, const GaussFieldCouplingParameters& gfcp)
	: gfcp(gfcp)
{
	assert(size > 0);
	assert(gfcp.inputFieldSize > 0);
	assert(gfcp.sigma > 0.0);

	this->label = ElementLabel::FIELD_COUPLING;
	this->uniqueName = id;
	this->size = size;

	components["input"] = std::vector<double>(gfcp.inputFieldSize);
	components["output"] = std::vector<double>(size);
}

void GaussFieldCoupling::init()
{
	std::ranges::fill(components["output"], 0.0);
}

void GaussFieldCoupling::step(const double& t, const double& deltaT)
{
	updateInput();
	updateOutput();
}

void GaussFieldCoupling::close()
{
}

void GaussFieldCoupling::updateOutput()
{
	std::vector<double> summedGaussians = std::vector<double>(size);
	std::ranges::fill(summedGaussians, 0.0);

	for (const auto& coupling : gfcp.couplings)
	{
		const auto activationAtx_i = components["input"][static_cast<int>(coupling.x_i)];
		if (activationAtx_i > 0.0)
		{
			std::vector<double> gauss = mathtools::circularGauss(size, gfcp.sigma, coupling.x_j);
			for (auto& element : gauss)
				element *= coupling.w_i_j * activationAtx_i * element;

			for (int i = 0; i < size; i++)
				summedGaussians[i] += gauss[i];
			
		}
	}
	components["output"] = summedGaussians;

}

void GaussFieldCoupling::addCoupling(const WeightedCoupling& coupling)
{
	gfcp.couplings.emplace_back(coupling);
}

GaussFieldCouplingParameters GaussFieldCoupling::getParameters() const
{
	return gfcp;
}