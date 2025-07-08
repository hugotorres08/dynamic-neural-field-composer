#pragma once

#include <string>

#include "tools/logger.h"
#include "tools/math.h"

namespace dnf_composer
{
	namespace element
	{
		enum ActivationFunctionType : int
		{
			SIGMOID,
			HEAVISIDE,
		};

		struct ActivationFunction
		{
			ActivationFunctionType type;
			ActivationFunction() = default;                                 
			ActivationFunction(const ActivationFunction&) = default;
			ActivationFunction& operator=(const ActivationFunction&) = delete;
			virtual std::vector<double> operator()(const std::vector<double>& input) = 0;
			virtual bool operator==(const ActivationFunction& other) const = 0;
			virtual std::unique_ptr<ActivationFunction> clone() const = 0;
			virtual std::string toString() const = 0;
			virtual void print() const = 0;
			virtual ~ActivationFunction() = default;

		};

		struct SigmoidFunction : public ActivationFunction
		{
			double x_shift, steepness;

			SigmoidFunction(const SigmoidFunction&) = default;
			SigmoidFunction(double x_shift, double steepness);

			std::vector<double> operator()(const std::vector<double>& input) override;
			bool operator==(const ActivationFunction& other) const override;
			std::unique_ptr<ActivationFunction> clone() const override;
			std::string toString() const override;
			void print() const override;

			double getSteepness() const;
			double getXShift() const;

			~SigmoidFunction() override = default;

		};

		struct HeavisideFunction : public ActivationFunction
		{
			double x_shift;

			HeavisideFunction(const HeavisideFunction&) = default;
			HeavisideFunction(double x_shift);

			std::vector<double> operator()(const std::vector<double>& input) override;
			bool operator==(const ActivationFunction& other) const override;
			std::unique_ptr<ActivationFunction> clone() const override;
			std::string toString() const override;
			void print() const override;

			double getXShift() const;

			~HeavisideFunction() override = default;
		};
	}
}