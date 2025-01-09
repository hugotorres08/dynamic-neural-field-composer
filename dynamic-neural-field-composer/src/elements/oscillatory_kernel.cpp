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
			// Determine kernel range
			double effectiveRange = std::max(1.0 / parameters.decay,
				parameters.zeroCrossings * cutOfFactor);
			kernelRange = tools::math::computeKernelRange(effectiveRange, cutOfFactor,
				commonParameters.dimensionParameters.size, parameters.circular);

			if (parameters.circular)
				extIndex = tools::math::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
			else
				extIndex = {};

			// Create the range for kernel computation
			int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			std::iota(rangeX.begin(), rangeX.end(), -kernelRange[0]);

			// Compute the oscillatory kernel components
			components["kernel"].resize(rangeX.size());
			for (int i = 0; i < components["kernel"].size(); i++)
			{
				double distance = rangeX[i];
				double decayFactor = exp(-parameters.decay * std::abs(distance));
				double oscillation = sin(parameters.decay * std::abs(parameters.zeroCrossings * distance)) + cos(parameters.zeroCrossings * distance);
				components["kernel"][i] = parameters.amplitude * decayFactor * oscillation;
			}

			if (parameters.normalized)
			{
				double normFactor = std::accumulate(components["kernel"].begin(), components["kernel"].end(), 0.0);
				if (normFactor != 0.0)
				{
					for (double& value : components["kernel"])
						value /= normFactor;
				}
			}

			std::ranges::fill(components["input"], 0.0);
			std::ranges::fill(components["output"], 0.0);
			fullSum = 0.0;
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
			// correct decay if necessary
			if (parameters.decay <= 0.0)
				parameters.decay = 0.01;
			init();
		}

		OscillatoryKernelParameters OscillatoryKernel::getParameters() const
		{
			return parameters;
		}
	}
}
