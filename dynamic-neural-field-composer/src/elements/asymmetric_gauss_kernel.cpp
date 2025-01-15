#include "elements/asymmetric_gauss_kernel.h"


namespace dnf_composer
{
	namespace element
	{
		AsymmetricGaussKernel::AsymmetricGaussKernel(const ElementCommonParameters& elementCommonParameters, AsymmetricGaussKernelParameters agk_parameters)
			: Kernel(elementCommonParameters), parameters(std::move(agk_parameters))
		{
			commonParameters.identifiers.label = ElementLabel::ASYMMETRIC_GAUSS_KERNEL;
			components["kernel"] = std::vector<double>(commonParameters.dimensionParameters.size);
		}

		void AsymmetricGaussKernel::init()
		{
            // Compute kernel range
            kernelRange = tools::math::computeKernelRange(parameters.width, cutOfFactor, commonParameters.dimensionParameters.size, parameters.circular);


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

            // Generate the Gaussian kernel
            int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
            std::vector<int> rangeX(rangeXsize);
            const int startingValue = static_cast<int>(kernelRange[0]);
            std::iota(rangeX.begin(), rangeX.end(), -startingValue);

            // Generate Gaussian and derivative
            std::vector<double> gauss(commonParameters.dimensionParameters.size);
            std::vector<double> gaussDerivative(commonParameters.dimensionParameters.size);

            if (parameters.normalized)
            {
                gauss = tools::math::gaussNorm(rangeX, 0.0, parameters.width);
                gaussDerivative = tools::math::gaussDerivativeNorm(rangeX, 0.0, parameters.width, parameters.amplitude); // Assume this exists in math tools
            }
            else
            {
                gauss = tools::math::gauss(rangeX, 0.0, parameters.width);
                gaussDerivative = tools::math::gaussDerivative(rangeX, 0.0, parameters.width, parameters.amplitude); // Assume this exists in math tools
            }

            // Simulated eta(t) using exponential decay
            static const double eta0 = parameters.timeShift; // Initial amplitude of the bump movement
            static const double lambda = 5; // Decay rate of eta(t)
            double eta_t = eta0 * exp(-lambda); // Exponential decay

            // Alternatively, use linear decay (comment out the above and uncomment below)
            //double eta_t = eta0 * std::max(0.0, 1.0 - lambda * t);


            // Combine Gaussian with its derivative for asymmetry
            components["kernel"].resize(rangeX.size());
            for (int i = 0; i < components["kernel"].size(); i++)
            {
                components["kernel"][i] = parameters.amplitude * gauss[i] + parameters.timeShift * gaussDerivative[i];
            }

            fullSum = 0.0;
            std::ranges::fill(components["input"], 0.0);
            std::ranges::fill(components["output"], 0.0);
		}

		void AsymmetricGaussKernel::step(double t, double deltaT)
		{
            updateInput();

            fullSum = std::accumulate(components["input"].begin(), components["input"].end(),
                (double)0.0);

            // Compute convolution
            std::vector<double> convolution(commonParameters.dimensionParameters.size);
            const std::vector<double> subDataInput = tools::math::obtainCircularVector(extIndex, components["input"]);

            if (parameters.circular)
            {
                convolution = tools::math::conv_valid(subDataInput, components["kernel"]);
            }
            else
            {
                convolution = tools::math::conv_same(components["input"], components["kernel"]);
            }

            for (int i = 0; i < components["output"].size(); i++)
            {
                components["output"][i] = convolution[i] + parameters.amplitudeGlobal * fullSum;
            }
		}

		std::string AsymmetricGaussKernel::toString() const
		{
			std::string result = "Asymmetric gauss kernel element\n";
			result += commonParameters.toString() + '\n';
			result += parameters.toString();
			return result;
		}

		std::shared_ptr<Element> AsymmetricGaussKernel::clone() const
		{
			auto cloned = std::make_shared<AsymmetricGaussKernel>(*this);
			return cloned;
		}

		void AsymmetricGaussKernel::setParameters(const AsymmetricGaussKernelParameters& agk_parameters)
		{
			parameters = agk_parameters;
			init();
		}

		AsymmetricGaussKernelParameters AsymmetricGaussKernel::getParameters() const
		{
			return parameters;
		}
	}
}
