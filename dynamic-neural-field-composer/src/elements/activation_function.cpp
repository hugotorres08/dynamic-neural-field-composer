
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/activation_function.h"



namespace dnf_composer
{
	namespace element
	{
		SigmoidFunction::SigmoidFunction(double x_shift, double steepness)
		: x_shift(x_shift), steepness(steepness)
		{
			type = ActivationFunctionType::SIGMOID;
		}

		std::vector<double> SigmoidFunction::operator()(const std::vector<double>& input)
		{
			return tools::math::sigmoid(input, steepness, x_shift);
		}

		bool SigmoidFunction::operator==(const ActivationFunction& other) const
		{
			if (type == other.type)
			{
				auto& other_casted = dynamic_cast<const SigmoidFunction&>(other);
				return x_shift == other_casted.getXShift() && steepness == other_casted.getSteepness();
			}
			return false;
		}

		std::unique_ptr<ActivationFunction> SigmoidFunction::clone() const 
		{
			return std::make_unique<SigmoidFunction>(*this);
		}

		std::string SigmoidFunction::toString() const
		{
			std::string result = "SigmoidFunction(";
			std::ostringstream stream_x_shift;
			stream_x_shift << std::fixed << std::setprecision(2) << x_shift;
			result += "x_shift = " + stream_x_shift.str() + ", ";
			std::ostringstream stream_steepness;
			stream_steepness << std::fixed << std::setprecision(2) << steepness;
			result += "steepness = " + stream_steepness.str() + ")";
			return result;
		}

		void SigmoidFunction::print() const
		{
			const std::string result = toString();
			tools::logger::log(tools::logger::LogLevel::INFO, result);
		}

		double SigmoidFunction::getSteepness() const
		{
			return steepness;
		}

		double SigmoidFunction::getXShift() const
		{
			return x_shift;
		}

		HeavisideFunction::HeavisideFunction(double x_shift)
		: x_shift(x_shift)
		{
			type = ActivationFunctionType::HEAVISIDE;
		}

		std::vector<double> HeavisideFunction::operator()(const std::vector<double>& input)
		{
			return tools::math::heaviside(input, x_shift);
		}

		bool HeavisideFunction::operator==(const ActivationFunction& other) const
		{
			if (type == other.type)
			{
				auto& other_casted = dynamic_cast<const HeavisideFunction&>(other);
				return x_shift == other_casted.getXShift();
			}
			return false;
		}

		std::unique_ptr<ActivationFunction> HeavisideFunction::clone() const 
		{
			return std::make_unique<HeavisideFunction>(*this);
		}

		std::string HeavisideFunction::toString() const
		{
			std::string result = "HeavisideFunction(";
			std::ostringstream stream;
			stream << std::fixed << std::setprecision(2) << x_shift;
			result += "x_shift = " + stream.str() + ")";
			return result;
		}

		void HeavisideFunction::print() const
		{
			const std::string result = toString();
			tools::logger::log(tools::logger::LogLevel::INFO, result);
		}

		double HeavisideFunction::getXShift() const
		{
			return x_shift;
		}
	}
}
