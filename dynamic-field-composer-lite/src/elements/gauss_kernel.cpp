// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/gauss_kernel.h"

#include "tools/profiling.h"

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
			kernelRange = tools::computeKernelRange(parameters.sigma, cutOfFactor, commonParameters.dimensionParameters.size, circular);

			if (circular)
				extIndex = tools::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
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
				gauss = tools::gaussNorm(rangeX, 0.0, parameters.sigma);
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

			std::vector<double> subDataInput = tools::obtainCircularVector(extIndex, components["input"]);

			{
				std::vector<double> convolution(commonParameters.dimensionParameters.size);

				//convolution = STD_Convolution::conv_linear_full(subDataInput, components["kernel"]);
				//convolution = STD_Convolution::conv_linear_same(subDataInput, components["kernel"]);
				//convolution = STD_Convolution::conv_linear_valid(subDataInput, components["kernel"]);
				//convolution = STD_Convolution::conv_circular_same(subDataInput, components["kernel"]);
				convolution = STD_Convolution::conv_circular_full(subDataInput, components["kernel"]);


				for (int i = 0; i < components["output"].size(); i++)
					components["output"][i] = convolution[i] + parameters.amplitudeGlobal * fullSum;
			}



			// testing three types of convolutions
			// testing linear valid mode
			// testing different kernel sizes 10, 100, 1000, 10000
			// 1 - nested for loops - stack overflow
			// 2 - nested for loops - jeremyfix on GitHub
			// 3 - fft using fftw lib - jeremyfix on GitHub

			const std::string testCase = "nested for loops - jeremyfix on GitHub";

			//if(testCase == "nested for loops - stack overflow")
			//{
			//	std::vector<double> convolution(commonParameters.dimensionParameters.size);
			//	if (circular)
			//		convolution = tools::conv_valid(subDataInput, components["kernel"]);
			//	else
			//		convolution = tools::conv(subDataInput, components["kernel"]);

			//	for (int i = 0; i < components["output"].size(); i++)
			//		components["output"][i] = convolution[i] + parameters.amplitudeGlobal * fullSum;
			//}

			//if (testCase == "fft using fftw lib - jeremyfix on GitHub")
			//{
			//	const int h_src = static_cast<int>(components["input"].size());
			//	constexpr int w_src = 1; // For 1-dimensional vectors
			//	const int h_kernel = static_cast<int>(components["kernel"].size());
			//	constexpr int w_kernel = 1; // For 1-dimensional kernels

			//	FFTW_Convolution::Workspace workspace;
			//	FFTW_Convolution::init_workspace(workspace, FFTW_Convolution::LINEAR_VALID, h_src, w_src, h_kernel, w_kernel);

			//	FFTW_Convolution::convolve(workspace, subDataInput.data(), components["kernel"].data());

			//	std::vector<double> convolution(workspace.dst, workspace.dst + workspace.h_dst * workspace.w_dst);

			//	for (int i = 0; i < components["output"].size(); i++)
			//		components["output"][i] = convolution[i] + parameters.amplitudeGlobal * fullSum;
			//	//commonParameters.dimensionParameters.d_x;

			//	FFTW_Convolution::clear_workspace(workspace);
			//}
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
