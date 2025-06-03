// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/mexican_hat_kernel.h"

namespace dnf_composer
{
	namespace element
	{
		MexicanHatKernel::MexicanHatKernel(const ElementCommonParameters& elementCommonParameters, MexicanHatKernelParameters mhk_parameters)
			: Kernel(elementCommonParameters), parameters(std::move(mhk_parameters))
		{
			commonParameters.identifiers.label = ElementLabel::MEXICAN_HAT_KERNEL;
		}

		void MexicanHatKernel::init()
		{
			const double maxWidth = std::max((parameters.amplitudeExc != 0.0) ? parameters.widthExc : 0,
				(parameters.amplitudeInh != 0.0) ? parameters.widthInh : 0);
			kernelRange = tools::math::computeKernelRange(maxWidth, cutOfFactor,
				commonParameters.dimensionParameters.size, parameters.circular);

			if (parameters.circular)
				extIndex = tools::math::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
			else
				extIndex = {};


			int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			const int startingValue = static_cast<int>(kernelRange[0]);
			std::iota(rangeX.begin(), rangeX.end(), -startingValue);
			std::vector<double> gaussExc(commonParameters.dimensionParameters.size);
			std::vector<double> gaussInh(commonParameters.dimensionParameters.size);
			if (parameters.normalized)
			{
				gaussExc = tools::math::gaussNorm(rangeX, 0.0, parameters.widthExc);
				gaussInh = tools::math::gaussNorm(rangeX, 0.0, parameters.widthInh);
			}
			else
			{

				gaussExc = tools::math::gauss(rangeX, 0.0, parameters.widthExc);
				gaussInh = tools::math::gauss(rangeX, 0.0, parameters.widthInh);
			}

			components["kernel"].resize(rangeX.size());
			for (int i = 0; i < components["kernel"].size(); i++)
				components["kernel"][i] = parameters.amplitudeExc * gaussExc[i] -
				parameters.amplitudeInh * gaussInh[i];

			fullSum = 0.0;
			std::ranges::fill(components["input"], 0.0);
			std::ranges::fill(components["output"], 0.0);  
		}

		void MexicanHatKernel::step(double t, double deltaT)
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

		std::string MexicanHatKernel::toString() const
		{
			std::string result = "Mexican hat kernel element\n";
			result += commonParameters.toString() + '\n';
			result += parameters.toString();
			return result;
		}

		std::shared_ptr<Element> MexicanHatKernel::clone() const
		{
			auto cloned = std::make_shared<MexicanHatKernel>(*this);
			return cloned;
		}

		void MexicanHatKernel::setParameters(const MexicanHatKernelParameters& mhk_parameters)
		{
			parameters = mhk_parameters;
			init();
		}

		MexicanHatKernelParameters MexicanHatKernel::getParameters() const
		{
			return parameters;
		}
	}
}