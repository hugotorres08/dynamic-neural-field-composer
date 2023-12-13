// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/mexican_hat_kernel.h"

namespace dnf_composer
{
	namespace element
	{
		MexicanHatKernel::MexicanHatKernel(const ElementCommonParameters& elementCommonParameters, const MexicanHatKernelParameters& mhk_parameters)
			: Kernel(elementCommonParameters), parameters(mhk_parameters)
		{
			commonParameters.identifiers.label = ElementLabel::MEXICAN_HAT_KERNEL;
		}

		void MexicanHatKernel::init()
		{
			const double maxSigma = std::max(parameters.amplitudeExc * parameters.sigmaExc, parameters.amplitudeInh * parameters.sigmaInh);
			kernelRange = mathtools::computeKernelRange(maxSigma, parameters.cutOfFactor, commonParameters.dimensionParameters.size, circular);

			if (circular)
				extIndex = mathtools::createExtendedIndex(commonParameters.dimensionParameters.size, kernelRange);
			else
			{
				const std::string message = "Tried to initialize a non-circular Mexican hat kernel '" + this->getUniqueName() + "'. That is not supported yet.";
				user_interface::LoggerWindow::addLog(user_interface::LogLevel::_ERROR, message.c_str());
			}

			uint32_t rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			const int startingValue = static_cast<int>(kernelRange[0]);
			std::iota(rangeX.begin(), rangeX.end(), -startingValue);
			std::vector<double> gaussExc(commonParameters.dimensionParameters.size);
			std::vector<double> gaussInh(commonParameters.dimensionParameters.size);
			if (!normalized)
			{
				gaussExc = mathtools::gaussNorm(rangeX, 0.0, parameters.sigmaExc);
				gaussInh = mathtools::gaussNorm(rangeX, 0.0, parameters.sigmaInh);
			}
			else
			{
				const std::string message = "Tried to initialize a normalized Mexican hat kernel '" + this->getUniqueName() + "'. That is not supported yet.";
				user_interface::LoggerWindow::addLog(user_interface::LogLevel::_ERROR, message.c_str());
			}

			for (int i = 0; i < components["kernel"].size(); i++)
				components["kernel"][i] = parameters.amplitudeExc * gaussExc[i] - parameters.amplitudeInh * gaussInh[i];

			parameters.fullSum = 0;
			std::ranges::fill(components["input"], 0.0);
		}

		void MexicanHatKernel::step(double t, double deltaT)
		{
			updateInput();

			parameters.fullSum = std::accumulate(components["input"].begin(), components["input"].end(), (double)0.0);

			std::vector<double> convolution(commonParameters.dimensionParameters.size);
			const std::vector<double> subDataInput = mathtools::obtainCircularVector(extIndex, components["input"]);

			if (circular)
				convolution = mathtools::conv_valid(subDataInput, components["kernel"]);
			else
				convolution = mathtools::conv(subDataInput, components["kernel"]);

			for (int i = 0; i < components["output"].size(); i++)
				components["output"][i] = convolution[i] + parameters.amplitudeGlobal;
		}

		void MexicanHatKernel::close()
		{
		}

		void MexicanHatKernel::printParameters()
		{
			std::ostringstream logStream;

			logStream << std::left;

			logStream << "Logging element parameters" << std::endl;
			logStream << "Unique Identifier: " << commonParameters.identifiers.uniqueIdentifier << std::endl;
			logStream << "Unique Name: " << commonParameters.identifiers.uniqueName << std::endl;
			logStream << "Label: " << ElementLabelToString.at(commonParameters.identifiers.label) << std::endl;
			logStream << "Maximum spatial dimension size: " << commonParameters.dimensionParameters.x_max << std::endl;
			logStream << "Spatial dimension step size: " << commonParameters.dimensionParameters.d_x << std::endl;
			logStream << "Number of samples in spatial dimension: " << commonParameters.dimensionParameters.size << std::endl;


			logStream << "Components: ";
			for (const auto& pair : components)
			{
				const std::string& componentName = pair.first;
				const std::vector<double>& componentValues = pair.second;

				logStream << componentName << " | ";
			}

			logStream << std::endl << "Inputs: ";
			for (const auto& inputPair : inputs)
			{
				const std::shared_ptr<Element>& inputElement = inputPair.first;
				const std::string& inputComponent = inputPair.second;

				logStream << inputElement->getUniqueName() << "->" << inputComponent << " | ";
			}

			logStream << std::endl << "MexicanHatKernelParameters: ";
			logStream << "AmplitudeExc: " << parameters.amplitudeExc << " | ";
			logStream << "SigmaExc: " << parameters.sigmaExc << " | ";
			logStream << "AmplitudeInh: " << parameters.amplitudeInh << " | ";
			logStream << "SigmaInh: " << parameters.sigmaInh << " | ";
			logStream << "AmplitudeGlobal: " << parameters.amplitudeGlobal << " | ";
			logStream << "CutOffFactor: " << parameters.cutOfFactor << " | ";
			logStream << "Normalized: " << normalized << std::endl;

			user_interface::LoggerWindow::addLog(user_interface::LogLevel::_INFO, logStream.str().c_str());
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