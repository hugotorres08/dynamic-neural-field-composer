#pragma once

#include <set>

#include "tools/math.h"
#include "element.h"
#include "neural_field.h"
#include "tools/utils.h"


namespace dnf_composer
{
	enum class LearningRule : int
	{
		HEBB,
		OJA,
		DELTA
	};

	inline const std::map<LearningRule, std::string> LearningRuleToString = {
		{LearningRule::HEBB, "Hebb"},
		{LearningRule::OJA, "Oja"},
		{LearningRule::DELTA, "Delta"}

	};

	namespace element
	{
		struct FieldCouplingParameters : ElementSpecificParameters
		{
			std::shared_ptr<Element> inputField;
			std::shared_ptr<Element> outputField;
			LearningRule learningRule;
			double scalar;
			double learningRate;
			ElementSpatialDimensionParameters inputFieldDimensions;
			bool learning;


			FieldCouplingParameters(const std::shared_ptr<Element>& input, const std::shared_ptr<Element>& output,
				LearningRule learningRule = LearningRule::HEBB, 
				double scalar = 1.0, double learningRate = 0.01)
				: inputField(input), outputField(output),
			learningRule(learningRule), scalar(scalar),
				learningRate(learningRate), inputFieldDimensions(), learning(false)
			{}

			std::string toString() const override
			{
				std::string result = "Field coupling parameters\n";
				result += "Learning rule: " + LearningRuleToString.at(learningRule) + "\n";
				result += "Learning rate: " + std::to_string(learningRate) + "\n";
				result += "Scalar: " + std::to_string(scalar) + "\n";
				return result;
			}
		};

		class FieldCoupling : public Element
		{
		protected:
			FieldCouplingParameters parameters;
			std::vector<std::vector<double>> weights;
			std::string weightsFilePath;
		public:
			FieldCoupling(const ElementCommonParameters& elementCommonParameters, 
				const FieldCouplingParameters& fc_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			void setWeightsFilePath(const std::string& filePath);
			void setLearningRate(double learningRate);
			void setLearning(bool learning) { parameters.learning = learning; }
			void setParameters(const FieldCouplingParameters& fcp);

			const std::vector<std::vector<double>>& getWeights() const;
			FieldCouplingParameters getParameters() const;

			void readWeights();
			void writeWeights() const;
			void fillWeightsRandomly();
		private:
			void updateOutput();
			void updateInputFieldDimensions();
			void updateWeights();
		};
	}
}
