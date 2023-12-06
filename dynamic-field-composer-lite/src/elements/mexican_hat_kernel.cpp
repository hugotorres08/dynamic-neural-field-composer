// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/mexican_hat_kernel.h"

namespace dnf_composer
{
	namespace element
	{
		MexicanHatKernel::MexicanHatKernel(const std::string& id, int size,
			const MexicanHatKernelParameters& parameters)
			: parameters(parameters)
		{
			if (size <= 0)
				throw Exception(ErrorCode::ELEM_INVALID_SIZE, id);

			this->label = ElementLabel::MEXICAN_HAT_KERNEL;
			this->uniqueName = id;
			this->size = size;

			components["kernel"] = std::vector<double>(size);
			components["output"] = std::vector<double>(size);
			components["input"] = std::vector<double>(size);
		}

		void MexicanHatKernel::init()
		{
			const double maxSigma = std::max(parameters.amplitudeExc * parameters.sigmaExc, parameters.amplitudeInh * parameters.sigmaInh);
			kernelRange = mathtools::computeKernelRange(maxSigma, parameters.cutOfFactor, size, circular);

			if (circular)
				extIndex = mathtools::createExtendedIndex(size, kernelRange);
			else
			{
				const std::string message = "Tried to initialize a non-circular Mexican hat kernel '" + this->getUniqueName() + "'. That is not supported yet.";
				user_interface::LoggerWindow::addLog(user_interface::LogLevel::_ERROR, message.c_str());
			}

			uint32_t rangeXsize = kernelRange[0] + kernelRange[1] + 1;
			std::vector<int> rangeX(rangeXsize);
			const int startingValue = static_cast<int>(kernelRange[0]);
			std::iota(rangeX.begin(), rangeX.end(), -startingValue);
			std::vector<double> gaussExc(size);
			std::vector<double> gaussInh(size);
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

			/*for (int i = 0; i < components["kernel"].size(); i++)
				components["kernel"][i] += parameters.amplitudeGlobal;*/

			parameters.fullSum = 0;
			std::ranges::fill(components["input"], 0.0);
		}

		void MexicanHatKernel::step(double t, double deltaT)
		{
			updateInput();

			parameters.fullSum = std::accumulate(components["input"].begin(), components["input"].end(), (double)0.0);

			std::vector<double> convolution(size);
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

		void MexicanHatKernel::setParameters(const MexicanHatKernelParameters& parameters)
		{
			this->parameters = parameters;
		}

		MexicanHatKernelParameters MexicanHatKernel::getParameters() const
		{
			return parameters;
		}
	}
}