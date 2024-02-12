// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/gauss_kernel.h"

namespace dnf_composer
{
	namespace element
	{
		GaussKernel::GaussKernel(const ElementCommonParameters& elementCommonParameters, const GaussKernelParameters& gk_parameters)
			: Kernel(elementCommonParameters), parameters(gk_parameters)
		{
			commonParameters.identifiers.label = ElementLabel::GAUSS_KERNEL;
			components["kernel"] = std::vector<double>(commonParameters.dimensionParameters.size);
		}

		void GaussKernel::init()
		{
			kernelRange = mathtools::computeKernelRange(parameters.sigma, cutOfFactor, commonParameters.dimensionParameters.size, circular);

			if (circular)
				extIndex = mathtools::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
			else
			{
				const std::string message = "Tried to initialize a non-circular Mexican hat kernel '" + this->getUniqueName() + "'. That is not supported yet.\n";
				log(LogLevel::ERROR, message);
			}

			int rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			const int startingValue = static_cast<int>(kernelRange[0]);
			std::iota(rangeX.begin(), rangeX.end(), -startingValue);
			std::vector<double> gauss(commonParameters.dimensionParameters.size);
			if (!normalized)
				gauss = mathtools::gaussNorm(rangeX, 0.0, parameters.sigma);
			else
			{
				const std::string message = "Tried to initialize a normalized Mexican hat kernel '" + this->getUniqueName() + "'. That is not supported yet.\n";
				log(LogLevel::ERROR, message);
			}

			components["kernel"].resize(rangeX.size());
			for (int i = 0; i < components["kernel"].size(); i++)
				components["kernel"][i] = parameters.amplitude * gauss[i];

			components["input"].resize(extIndex.size());
			fullSum = 0;
			std::ranges::fill(components["input"], 0.0);
		}

		void GaussKernel::step(double t, double deltaT)
		{

			updateInput();

			fullSum = std::accumulate(components["input"].begin(), components["input"].end(), (double)0.0);

			//std::vector<double> convolution(commonParameters.dimensionParameters.size);
			std::vector<double> subDataInput = mathtools::obtainCircularVector(extIndex, components["input"]);

			const int h_src = components["input"].size();
			constexpr int w_src = 1; // For 1-dimensional vectors
			const int h_kernel = components["kernel"].size();
			constexpr int w_kernel = 1; // For 1-dimensional kernels

			//STD_Convolution::Workspace workspace;
			//STD_Convolution::init_workspace(workspace, STD_Convolution::LINEAR_VALID, h_src, w_src, h_kernel, w_kernel);

			FFTW_Convolution::Workspace workspace;
			FFTW_Convolution::init_workspace(workspace, FFTW_Convolution::LINEAR_VALID, h_src, w_src, h_kernel, w_kernel);

			//STD_Convolution::convolve(workspace, subDataInput.data(), components["kernel"].data());
			FFTW_Convolution::convolve(workspace, subDataInput.data(), components["kernel"].data());

			std::vector<double> convolution(workspace.dst, workspace.dst + workspace.h_dst * workspace.w_dst);

			/*if (circular)
				convolution = mathtools::conv_valid(subDataInput, components["kernel"]);
			else
				convolution = mathtools::conv(subDataInput, components["kernel"]);*/

			for (int i = 0; i < components["output"].size(); i++)
				components["output"][i] = convolution[i] + parameters.amplitudeGlobal * fullSum;
				//commonParameters.dimensionParameters.d_x;

			//STD_Convolution::clear_workspace(workspace);
			FFTW_Convolution::clear_workspace(workspace);


		}

		void GaussKernel::close()
		{
		}

		void GaussKernel::printParameters()
		{
			printCommonParameters();

			std::ostringstream logStream;

			logStream << "Logging specific element parameters" << std::endl;
			logStream << "Amplitude: " << parameters.amplitude << std::endl;
			logStream << "Amplitude Global: " << parameters.amplitudeGlobal << std::endl;
			logStream << "Sigma: " << parameters.sigma << std::endl;
			logStream << "Cut-Off Factor: " << cutOfFactor << std::endl;
			logStream << "Circular: " << circular << std::endl;
			logStream << "Normalized: " << normalized << std::endl;

			log(LogLevel::INFO, logStream.str());
		}

		void GaussKernel::setParameters(const GaussKernelParameters& gk_parameters)
		{
			parameters = gk_parameters;
			init();
		}

		GaussKernelParameters GaussKernel::getParameters() const
		{
			return parameters;
		}
	}
}
