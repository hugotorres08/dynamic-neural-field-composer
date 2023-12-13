#pragma once

#include "../mathtools/mathtools.h"

namespace dnf_composer
{
	namespace element
	{
		struct ActivationFunction
		{
			ActivationFunction() = default;                                 
			virtual std::vector<double> operator()(const std::vector<double>& input) = 0;
			virtual std::unique_ptr<ActivationFunction> clone() const = 0; 
		};

		struct SigmoidFunction : public ActivationFunction
		{
			double x_shift, steepness;

			SigmoidFunction(double x_shift, double steepness) : x_shift(x_shift), steepness(steepness) {}
			std::vector<double> operator()(const std::vector<double>& input) override
			{
				return mathtools::sigmoid(input, steepness, x_shift);
			}

			std::unique_ptr<ActivationFunction> clone() const override
			{
				return std::make_unique<SigmoidFunction>(*this);
			}
		};

		struct HeavisideFunction : public ActivationFunction
		{
			double x_shift;

			HeavisideFunction(double x_shift) : x_shift(x_shift) {}
			std::vector<double> operator()(const std::vector<double>& input) override
			{
				return mathtools::heaviside(input, x_shift);
			}

			std::unique_ptr<ActivationFunction> clone() const override
			{
				return std::make_unique<HeavisideFunction>(*this);
			}
		};
	}
}