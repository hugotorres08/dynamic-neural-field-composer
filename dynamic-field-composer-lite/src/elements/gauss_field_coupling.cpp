#include "elements/gauss_field_coupling.h"



GaussFieldCoupling::GaussFieldCoupling(const std::string& id, const int& size, const GaussFieldCouplingParameters& gfcp)
	: gfcp(gfcp)
{
	assert(size > 0);

	this->label = ElementLabel::FIELD_COUPLING;
	this->uniqueIdentifier = id;
	this->size = size;

	components["input"] = std::vector<double>(size); // not right!!
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
	for (const auto& coupling : couplings)
	{
		if (components["input"][static_cast<int>(coupling.x_i)] > 0)
		{
			std::vector<double> gauss = mathtools::circularGauss(size, 5.0, coupling.x_j);
			for (auto& element : gauss)
				element *= coupling.w_i_j * element;

			components["output"] = gauss;
		}
	}
}

void GaussFieldCoupling::addCoupling(const WeightedCoupling& coupling)
{
	couplings.emplace_back(coupling);
}