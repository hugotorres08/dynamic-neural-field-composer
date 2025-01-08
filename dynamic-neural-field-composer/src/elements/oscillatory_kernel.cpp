// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/oscillatory_kernel.h"


namespace dnf_composer
{
	namespace element
	{
		OscillatoryKernel::OscillatoryKernel(const ElementCommonParameters& elementCommonParameters, 
			OscillatoryKernelParameters ok_parameters)
			: Kernel(elementCommonParameters), parameters(std::move(ok_parameters))
		{
			commonParameters.identifiers.label = ElementLabel::OSCILLATORY_KERNEL;
		}

		void OscillatoryKernel::init()
		{
			/*kernelRange = tools::math::computeKernelRange(parameters.decay, cutOfFactor,
								commonParameters.dimensionParameters.size, parameters.circular);

			if (parameters.circular)
			{
				extIndex = tools::math::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
				components["input"].resize(extIndex.size());
			}
			else
			{
				extIndex = {};
				components["input"].resize(commonParameters.dimensionParameters.size);
			}

			int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			const int startingValue = static_cast<int>(kernelRange[0]);
			std::iota(rangeX.begin(), rangeX.end(), -startingValue);

			components["kernel"].resize(rangeX.size());
			for (int i = 0; i < components["kernel"].size(); i++)
				components["kernel"][i] = parameters.amplitude * exp(-parameters.decay * abs(rangeX[i])) * (parameters.decay * sin(abs(parameters.zeroCrossings * i)) + cos(parameters.zeroCrossings * i));

			std::ranges::fill(components["input"], 0.0);
			std::ranges::fill(components["output"], 0.0);*/

			// Determine maximum range of the kernel
			//kernelRange = tools::math::computeKernelRange(parameters.decay, cutOfFactor,
			//	commonParameters.dimensionParameters.size, parameters.circular);

			//if (parameters.circular)
			//	extIndex = tools::math::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
			//else
			//	extIndex = {};

			//// Create the range for kernel computation
			//int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			//std::vector<int> rangeX(rangeXsize);
			//const int startingValue = static_cast<int>(kernelRange[0]);
			//std::iota(rangeX.begin(), rangeX.end(), -startingValue);

			//// Compute the oscillatory kernel components
			//components["kernel"].resize(rangeX.size());
			//double alpha = parameters.zeroCrossings * std::numbers::pi; // Calculate alpha based on zero crossings
			//for (int i = 0; i < components["kernel"].size(); i++)
			//{
			//	double distance = rangeX[i];
			//	double decayFactor = parameters.amplitude * exp(-parameters.decay * std::abs(distance));
			//	components["kernel"][i] = decayFactor * (sin(alpha * distance) + cos(alpha * distance));
			//}

			//// Normalize the kernel if required
			//if (parameters.normalized)
			//{
			//	double normFactor = std::accumulate(components["kernel"].begin(), components["kernel"].end(), 0.0);
			//	if (normFactor != 0.0)
			//	{
			//		for (double& value : components["kernel"])
			//			value /= normFactor;
			//	}
			//}

			//// Initialize input and output components
			//std::ranges::fill(components["input"], 0.0);
			//std::ranges::fill(components["output"], 0.0);

			//fullSum = 0.0; // Reset the full sum

				// Determine kernel range
			double effectiveRange = std::max(parameters.decay/100,
				parameters.zeroCrossings);
			kernelRange = tools::math::computeKernelRange(1 / parameters.decay, cutOfFactor,
				commonParameters.dimensionParameters.size, parameters.circular);

			if (parameters.circular)
				extIndex = tools::math::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
			else
				extIndex = {};

			// Create the range for kernel computation
			int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			const int startingValue = static_cast<int>(kernelRange[0]);
			std::iota(rangeX.begin(), rangeX.end(), -startingValue);

			// Compute the oscillatory kernel components
			components["kernel"].resize(rangeX.size());
			double alpha = parameters.zeroCrossings * std::numbers::pi; // Calculate alpha based on zero crossings

			for (int i = 0; i < components["kernel"].size(); i++)
			{
				double distance = rangeX[i];
				double decayFactor = exp(-parameters.decay * std::abs(distance)); // Smooth exponential decay
				double oscillation = sin(alpha * distance) + cos(alpha * distance); // Smooth oscillation
				components["kernel"][i] = parameters.amplitude * decayFactor * oscillation;
			}

			// Smooth the kernel further by applying a Gaussian envelope
			if (parameters.normalized)
			{
				double normFactor = std::accumulate(components["kernel"].begin(), components["kernel"].end(), 0.0);
				if (normFactor != 0.0)
				{
					for (double& value : components["kernel"])
						value /= normFactor;
				}
			}

			// Initialize input and output components
			std::ranges::fill(components["input"], 0.0);
			std::ranges::fill(components["output"], 0.0);

			fullSum = 0.0; // Reset the full sum
		}

		void OscillatoryKernel::step(double t, double deltaT)
		{
			updateInput();

			fullSum = std::accumulate(components["input"].begin(), components["input"].end(),
				(double)0.0);

			std::vector<double> convolution(commonParameters.dimensionParameters.size);
			const std::vector<double> subDataInput = tools::math::obtainCircularVector(extIndex,
				components["input"]);

			if (parameters.circular)
				convolution = tools::math::conv_valid(subDataInput, components["kernel"]);
			else
				convolution = tools::math::conv_same(components["input"], components["kernel"]);

			for (int i = 0; i < components["output"].size(); i++)
				components["output"][i] = convolution[i] + parameters.amplitudeGlobal * fullSum;
		}

		std::string OscillatoryKernel::toString() const
		{
			std::string result = "Oscillatory kernel element\n";
			result += commonParameters.toString() + '\n';
			result += parameters.toString();
			return result;
		}

		std::shared_ptr<Element> OscillatoryKernel::clone() const
		{
			auto cloned = std::make_shared<OscillatoryKernel>(*this);
			return cloned;
		}

		void OscillatoryKernel::setParameters(const OscillatoryKernelParameters& ok_parameters)
		{
			parameters = ok_parameters;
			// correct zero crossings if necessary
			if (parameters.zeroCrossings < 0.0)
				parameters.zeroCrossings = 0.0;
			else if (parameters.zeroCrossings > 1.0)
				parameters.zeroCrossings = 1.0;
			init();
		}

		OscillatoryKernelParameters OscillatoryKernel::getParameters() const
		{
			return parameters;
		}
	}
}
