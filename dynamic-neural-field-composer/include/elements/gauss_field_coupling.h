#pragma once

#include "element.h"
#include "tools/math.h"


namespace dnf_composer
{
	struct WeightedCoupling
	{
		double x_i, x_j, w_i_j;
	};

	namespace element
	{
		struct GaussFieldCouplingParameters : ElementSpecificParameters
		{
			int inputFieldSize = 0;
			double width = 0.0;
			std::vector<WeightedCoupling> couplings;

			void addCoupling(const WeightedCoupling& coupling)
			{
				couplings.emplace_back(coupling);
			}

			std::string toString() const override
			{
				std::string result = "Gauss field coupling parameters:\n";
				result += "inputFieldSize: " + std::to_string(inputFieldSize) + "\n";
				result += "width: " + std::to_string(width) + "\n";
				for (const auto& coupling : couplings)
				{
					result += "Coupling: x_i: " + std::to_string(coupling.x_i) + 
						" x_j: " + std::to_string(coupling.x_j) + " w_i_j: " + std::to_string(coupling.w_i_j) + "\n";
				}
				return result;
			}
		};

		class GaussFieldCoupling : public Element
		{
		private:
			GaussFieldCouplingParameters parameters;
		public:
			GaussFieldCoupling(const ElementCommonParameters& elementCommonParameters, 
				const GaussFieldCouplingParameters& gfc_parameters);

			void addCoupling(const WeightedCoupling& coupling);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			GaussFieldCouplingParameters getParameters() const;
		private:
			void updateOutput();
		};

	}
}




