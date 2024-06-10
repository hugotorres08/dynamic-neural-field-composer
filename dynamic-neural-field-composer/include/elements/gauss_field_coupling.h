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
			void printParameters() override;
			std::shared_ptr<Element> clone() const override;

			GaussFieldCouplingParameters getParameters() const;
		private:
			void updateOutput();
		};

	}
}




