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

			static const int rows = commonParameters.dimensionParameters.size;
			static const int cols = commonParameters.dimensionParameters.size;


			for (unsigned int x = 0; x < rows; x++)
			{
				std::vector<double> row;
				for (unsigned int y = 0; y < cols; y++)
				{
					double value = 0.0;
					for (const auto& coupling : parameters.couplings)
					{
						const double amplitude = coupling.amplitude / sqrt(2 * std::numbers::pi * std::pow(coupling.width, 2));
						value += tools::math::gaussian_2d(x, y, coupling.x_i, coupling.x_j, coupling.width, coupling.width, amplitude);
					}
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
			components["output"] = std::vector<double>(commonParameters.dimensionParameters.size, 0.0);

			for (unsigned int x = 0; x < commonParameters.dimensionParameters.size; x++)
			{
				for (unsigned int y = 0; y < commonParameters.dimensionParameters.size; y++)
				{
					components["output"][y] += components["kernel"][x * commonParameters.dimensionParameters.size + y]
																		* components["input"][x];
				}
			}
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