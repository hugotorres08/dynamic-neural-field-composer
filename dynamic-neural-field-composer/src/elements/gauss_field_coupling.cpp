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
			components["kernel"] = std::vector<double>(commonParameters.dimensionParameters.size * 
																commonParameters.dimensionParameters.size);
		}

		void GaussFieldCoupling::init()
		{
			std::ranges::fill(components["output"], 0.0);

			static constexpr int rows = 100;
			static constexpr int cols = 100;

			for (unsigned int x = 0; x < rows; x++)
			{
				std::vector<double> row;
				for (unsigned int y = 0; y < cols; y++)
				{
					double value = 0.0;
					for (const auto& coupling : parameters.couplings)
						value += tools::math::gaussian_2d(x, y, coupling.x_i, coupling.x_j, coupling.width, coupling.width, coupling.amplitude);
					components["kernel"][x * cols + y] = value;
				}
			}
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

			for (const auto& coupling : parameters.couplings)
			{
				const double x_i = static_cast<int>(coupling.x_i/commonParameters.dimensionParameters.d_x);
				const double x_j = static_cast<int>(coupling.x_j / commonParameters.dimensionParameters.d_x);
				const auto activationAtx_i = components["input"][static_cast<int>(x_i)];
				if (activationAtx_i > 0.0)
				{
					double amplitude = coupling.amplitude;
					if (parameters.normalized)
						amplitude /= sqrt(2 * std::numbers::pi * std::pow(coupling.width, 2));
					if (parameters.circular)
						gauss = tools::math::circularGauss(commonParameters.dimensionParameters.size,
														coupling.width, x_j);
					else
						gauss = tools::math::nonCircularGauss(commonParameters.dimensionParameters.size,
													coupling.width, x_j);
					for (auto& element : gauss)
						element *= amplitude * element;
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
			init();
		}
	}
}