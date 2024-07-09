// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/gauss_field_coupling.h"

namespace dnf_composer
{
	namespace element
	{
		GaussFieldCoupling::GaussFieldCoupling(const ElementCommonParameters& elementCommonParameters, 
			const GaussFieldCouplingParameters& gfc_parameters)
			: Element(elementCommonParameters), parameters(gfc_parameters)
		{
			commonParameters.identifiers.label = ElementLabel::GAUSS_FIELD_COUPLING;
		}

		void GaussFieldCoupling::init()
		{
			std::ranges::fill(components["output"], 0.0);
		}

		void GaussFieldCoupling::step(double t, double deltaT)
		{
			updateInput();
			updateOutput();
		}

		std::string GaussFieldCoupling::toString() const
		{
			std::string result = "Gauss field coupling element\n";
			result += commonParameters.toString();
			result += parameters.toString();
			return result;
		}

		std::shared_ptr<Element> GaussFieldCoupling::clone() const
		{
			auto cloned = std::make_shared<GaussFieldCoupling>(*this);
			return cloned;
		}

		void GaussFieldCoupling::updateOutput()
		{
			std::vector<double> summedGaussians(commonParameters.dimensionParameters.size);
			std::vector<double> gauss(commonParameters.dimensionParameters.size);
			std::ranges::fill(summedGaussians, 0.0);

			for (const auto& coupling : parameters.couplings)
			{
				const auto activationAtx_i = components["input"][static_cast<int>(coupling.x_i)];
				if (activationAtx_i > 0.0)
				{
					double amplitude = coupling.amplitude;
					if (parameters.normalized)
						amplitude /= sqrt(2 * std::numbers::pi * std::pow(coupling.width, 2));
					if (parameters.circular)
						gauss = tools::math::circularGauss(commonParameters.dimensionParameters.size,
														coupling.width, coupling.x_j);
					else
						gauss = tools::math::nonCircularGauss(commonParameters.dimensionParameters.size,
													coupling.width, coupling.x_j);
					for (auto& element : gauss)
						element *= amplitude * activationAtx_i * element;
					for (int i = 0; i < commonParameters.dimensionParameters.size; i++)
						summedGaussians[i] += gauss[i];
				}
			}

			components["output"] = std::move(summedGaussians);
		}

		void GaussFieldCoupling::addCoupling(const GaussCoupling& coupling)
		{
			parameters.couplings.emplace_back(coupling);
		}

		GaussFieldCouplingParameters GaussFieldCoupling::getParameters() const
		{
			return parameters;
		}

		void GaussFieldCoupling::setParameters(const GaussFieldCouplingParameters& gfc_parameters)
		{
			parameters = gfc_parameters;
		}
	}
}