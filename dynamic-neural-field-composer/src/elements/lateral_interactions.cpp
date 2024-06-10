// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "elements/lateral_interactions.h"

namespace dnf_composer
{
	namespace element
	{
		LateralInteractions::LateralInteractions(const ElementCommonParameters& elementCommonParameters, LateralInteractionsParameters li_parameters)
			: Kernel(elementCommonParameters), parameters(std::move(li_parameters))
		{
			commonParameters.identifiers.label = ElementLabel::LATERAL_INTERACTIONS;
		}

		void LateralInteractions::init()
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

			fullSum = 0;
			std::ranges::fill(components["input"], 0.0);
		}

		void LateralInteractions::step(double t, double deltaT)
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

		void LateralInteractions::printParameters()
		{
			printCommonParameters();

			std::ostringstream logStream;

			logStream << "Logging specific element parameters" << std::endl;
			logStream << "AmplitudeExc: " << parameters.amplitudeExc << std::endl;
			logStream << "WidthExc: " << parameters.widthExc << std::endl;
			logStream << "AmplitudeInh: " << parameters.amplitudeInh << std::endl;
			logStream << "WidthInh: " << parameters.widthInh << std::endl;
			logStream << "AmplitudeGlobal: " << parameters.amplitudeGlobal << std::endl;
			logStream << "CutOffFactor: " << cutOfFactor << std::endl;
			logStream << "Normalized: " << parameters.normalized << std::endl;
			logStream << "Circularity: " << parameters.circular;

			log(tools::logger::LogLevel::INFO, logStream.str());
		}

		std::shared_ptr<Element> LateralInteractions::clone() const
		{
			auto cloned = std::make_shared<LateralInteractions>(*this);
			return cloned;
		}

		void LateralInteractions::setParameters(const LateralInteractionsParameters& li_parameters)
		{
			parameters = li_parameters;
			init();
		}

		LateralInteractionsParameters LateralInteractions::getParameters() const
		{
			return parameters;
		}
	}
}