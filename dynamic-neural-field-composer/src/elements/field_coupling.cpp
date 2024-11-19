// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "elements/field_coupling.h"

#include <complex>


namespace dnf_composer
{
	namespace element
	{

		FieldCoupling::FieldCoupling(const ElementCommonParameters& elementCommonParameters, 
			const FieldCouplingParameters& parameters)
			: Element(elementCommonParameters), parameters(parameters)
		{
			this->parameters.learning = false;
			commonParameters.identifiers.label = ElementLabel::FIELD_COUPLING;
			weightsFilePath = std::string(OUTPUT_DIRECTORY) + "/inter-field-synaptic-connections/" + 
				commonParameters.identifiers.uniqueName + "_weights.txt";

			updateInputFieldDimensions();
			components["kernel"] = std::vector<double>(commonParameters.dimensionParameters.size *
				parameters.inputFieldDimensions.size);
			components["input"] = std::vector<double>(parameters.inputFieldDimensions.size, 0);
			components["output"] = std::vector<double>(commonParameters.dimensionParameters.size, 0);
			weights = std::vector<std::vector<double>>(components["input"].size(), std::vector<double>(components["output"].size(), 0));
		}

		void FieldCoupling::init()
		{
			parameters.learning = false;
			updateInputFieldDimensions();
			std::ranges::fill(components["input"], 0);
			std::ranges::fill(components["output"], 0);

			if (!readWeights())
				fillWeightsRandomly();

			// new add
			components["kernel"] = tools::math::flattenMatrix(weights);
		}

		void FieldCoupling::step(double t, double deltaT)
		{
			if (parameters.learning)
				updateWeights();
			updateInput();
			updateOutput();
		}

		std::string FieldCoupling::toString() const
		{
			std::string result = "Field coupling element\n";
			result += commonParameters.toString();
			result += parameters.toString();
			return result;
		}

		std::shared_ptr<Element> FieldCoupling::clone() const
		{
			auto cloned = std::make_shared<FieldCoupling>(*this);
			return cloned;
		}

		void FieldCoupling::setParameters(const FieldCouplingParameters& fcp)
		{
			parameters = fcp;
		}

		void FieldCoupling::setLearningRate(double learningRate)
		{
			parameters.learningRate = learningRate;
		}

		const std::vector<std::vector<double>>& FieldCoupling::getWeights() const
		{
			return weights;
		}

		FieldCouplingParameters FieldCoupling::getParameters() const
		{
			return parameters;
		}

		void FieldCoupling::updateOutput()
		{
			components["output"] = std::vector<double>(components["output"].size(), 0);

			for (int i = 0; i < components["output"].size(); i++)
				for (int j = 0; j < components["input"].size(); j++)
					components["output"][i] += weights[j][i] * components["input"][j];
		}

		void FieldCoupling::updateInputFieldDimensions()
		{
			if (inputs.empty())
			{
				const std::string logMessage = "No input element is connected to '" + commonParameters.identifiers.uniqueName + "'.";
				log(tools::logger::LogLevel::ERROR, logMessage);
				return;
			}
			const std::shared_ptr<Element> input = inputs.begin()->first;
			tools::logger::log(tools::logger::INFO, "Currently not checking if field couplings have more than one input - enforce this.");
			parameters.inputFieldDimensions = input->getElementCommonParameters().dimensionParameters;
		}

		void FieldCoupling::updateWeights()
		{
			std::vector<double> input = parameters.inputField->getComponents()->at("output");
			std::vector<double> output = parameters.outputField->getComponents()->at("output");

			static auto copy_weights = weights;

			// learning rule delta_wij = learning_rate * (input_i * output_j - output_j^2 * wij)
			/*for (int i = 0; i < input.size(); i++)
				for (int j = 0; j < output.size(); j++)
					copy_weights[i][j] += parameters.learningRate * (input[i] * output[j] - output[j] * output[j] * copy_weights[i][j]);*/

			//tools::math::deltaLearningRuleKroghHertz(copy_weights, input, output, parameters.learningRate);
			//tools::math::deltaLearningRuleWidrowHoff(copy_weights, input, output, parameters.learningRate);
			//tools::math::hebbLearningRule(copy_weights, input, output, parameters.learningRate);
			tools::math::ojaLearningRule(copy_weights, input, output, parameters.learningRate);


			components["kernel"] = tools::math::flattenMatrix(copy_weights);
		}

		bool FieldCoupling::readWeights()
		{
			std::ifstream file(weightsFilePath);

			const std::tuple<int, int> initialWeightsSize = std::make_tuple( static_cast<int>(weights.size()), 
				static_cast<int>(weights[0].size()) );
			std::tuple<int, int> readWeightsSize = std::make_tuple(0, 0);

			if (file.is_open()) {
				tools::utils::resizeMatrix(weights, 0, 0);
				double element;
				std::vector<double> row;
				while (file >> element) 
				{  
					row.push_back(element);  
					if (row.size() == components["output"].size())
					{
						weights.push_back(row);  
						row.clear(); 
					}
				}
				file.close();
				const std::string message = "Weights '" + this->getUniqueName() + "' read successfully from: " +
					weightsFilePath + ".";
				log(tools::logger::LogLevel::INFO, message);

				readWeightsSize = std::make_tuple(static_cast<int>(weights.size()), 
					static_cast<int>(weights[0].size()));
				if(initialWeightsSize != readWeightsSize)
				{
					log(tools::logger::LogLevel::ERROR, "Weight matrix read from file has a different "
										 "dimensionality compared to the actual matrix size! ");
					return false;
				}
				return true;	
			}

			const std::string message = "Failed to read weights '" + this->getUniqueName() + "' from: " +
				weightsFilePath + ". ";
			log(tools::logger::LogLevel::ERROR, message);
			
			return false;
		}

		void FieldCoupling::writeWeights() const
		{
			std::ofstream file(weightsFilePath);

			if (file.is_open()) {
				for (const auto& row : weights) {
					for (const auto& element : row) {
						file << element << " ";  
					}
					file << '\n'; 
				}
				file.close();
				const std::string message = "Saved weights '" + this->getUniqueName() +"' to: " + weightsFilePath + ".";
				log(tools::logger::LogLevel::INFO, message);
			}
			else
			{
				const std::string message = "Failed to saved weights '" + this->getUniqueName() + "' to: " + 
					weightsFilePath + ". ";
				log(tools::logger::LogLevel::ERROR, message);
			}
		}

		void FieldCoupling::fillWeightsRandomly()
		{
			tools::utils::resizeMatrix(weights, static_cast<int>(components["input"].size()),
				static_cast<int>(components["output"].size()));
			tools::utils::fillMatrixWithRandomValues(weights, 0.0, 0.1);
			log(tools::logger::LogLevel::INFO, "Filling the weight matrix with random values.");
			writeWeights();
		}

		void FieldCoupling::setWeightsFilePath(const std::string& filePath)
		{
			weightsFilePath = filePath + "/" + commonParameters.identifiers.uniqueName + "_weights.txt";
		}
	}
}
