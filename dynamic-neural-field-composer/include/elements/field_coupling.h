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
			ElementDimensions inputFieldDimensions;
			LearningRule learningRule;
			double scalar;
			double learningRate;
			bool isLearningActive;

			FieldCouplingParameters(const ElementDimensions& inputFieldDimensions = ElementDimensions{},
				LearningRule learningRule = LearningRule::HEBB,
				double scalar = 1.0, double learningRate = 0.01)
					: inputFieldDimensions(inputFieldDimensions),
				learningRule(learningRule), scalar(scalar),
				learningRate(learningRate), isLearningActive(false)
			{}

			std::string toString() const override
			{
				std::string result = "Field coupling parameters\n";
				result += "Input field dimensions: " + inputFieldDimensions.toString() + "\n";
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
			std::shared_ptr<Element> input;
			std::shared_ptr<Element> output;
		public:
			FieldCoupling(const ElementCommonParameters& elementCommonParameters, 
				const FieldCouplingParameters& fc_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			void setLearningRate(double learningRate);
			void setLearning(bool learning);
			void setParameters(const FieldCouplingParameters& fcp);
			FieldCouplingParameters getParameters() const;

			void readWeights();
			void writeWeights() const;
			void clearWeights();
		private:
			void updateOutput();
			void updateInputField();
			void updateOutputField();
			void updateWeights();
			bool checkValidConnections();
		};
	}
}
