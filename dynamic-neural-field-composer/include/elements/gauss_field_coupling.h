#pragma once

#include "element.h"
#include "tools/math.h"


namespace dnf_composer
{
	namespace element
	{
		struct GaussCoupling
		{
			double x_i, x_j, amplitude, width;

			GaussCoupling(double x_i, double x_j, double amplitude, double width)
				: x_i(x_i), x_j(x_j), amplitude(amplitude), width(width)
			{
				if (x_i <= 0.0 || x_j <= 0.0 || amplitude <= 0.0 || width <= 0.0)
					throw Exception(ErrorCode::ELEM_INVALID_PARAMETER, "GaussCoupling");
			}

			bool operator==(const GaussCoupling& other) const
			{
				constexpr double epsilon = 1e-6;

				return std::abs(x_i - other.x_i) < epsilon &&
					std::abs(x_j - other.x_j) < epsilon &&
					std::abs(amplitude - other.amplitude) < epsilon &&
					std::abs(width - other.width) < epsilon;
			}

			std::string toString() const
			{
				std::string result = "Gauss coupling [";
				result += "x_i: " + std::to_string(x_i) + " ";
				result += "x_j: " + std::to_string(x_j) + " ";
				result += "a: " + std::to_string(amplitude) + " ";
				result += "w: " + std::to_string(width) + "]\n";
				return result;
			}

			void print() const
			{
				tools::logger::log(tools::logger::INFO, toString());
			}
		};


		struct GaussFieldCouplingParameters : ElementSpecificParameters
		{
			bool normalized = true;
			bool circular = false;
			std::vector<GaussCoupling> couplings;

			GaussFieldCouplingParameters(bool normalized = true, bool circular = false,
				const std::vector<GaussCoupling>& couplings = {})
				: normalized(normalized), circular(circular), couplings(couplings)
			{}

			void addCoupling(const GaussCoupling& coupling)
			{
				couplings.emplace_back(coupling);
			}

			std::string toString() const override
			{
				std::string result = "Gauss field coupling parameters:\n";
				result += "Normalized: " + std::to_string(normalized) + "\n";
				result += "Circular: " + std::to_string(circular) + "\n";

				for (const auto& coupling : couplings)
					result += coupling.toString();

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

			void addCoupling(const GaussCoupling& coupling);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			GaussFieldCouplingParameters getParameters() const;
			void setParameters(const GaussFieldCouplingParameters& gfc_parameters);
		private:
			void updateOutput();
		};

	}
}




