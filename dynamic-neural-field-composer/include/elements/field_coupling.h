#pragma once

#include <set>

#include "tools/math.h"
#include "element.h"
#include "tools/utils.h"


namespace dnf_composer
{
	enum class LearningRule
	{
		HEBBIAN = 0,
		OJA,
		DELTA_WIDROW_HOFF,
		DELTA_KROGH_HERTZ
	};

	namespace element
	{
		struct FieldCouplingParameters : ElementSpecificParameters
		{
			int inputFieldSize;
			double scalar;
			double learningRate;
			LearningRule learningRule;

			FieldCouplingParameters(int inputFieldSize = 100, double scalar = 1.0, double learningRate = 0.1,
				LearningRule learningRule = LearningRule::HEBBIAN)
			: inputFieldSize(inputFieldSize), scalar(scalar), learningRate(learningRate),
				learningRule(learningRule)
			{}

			std::string toString() const override
			{
				std::string result = "Field coupling parameters\n";
				result += "Input field size: " + std::to_string(inputFieldSize) + "\n";
				result += "Scalar: " + std::to_string(scalar) + "\n";
				result += "Learning rate: " + std::to_string(learningRate) + "\n";
				return result;
			}
		};

		class FieldCoupling : public Element
		{
		protected:
			FieldCouplingParameters parameters;
			std::vector<std::vector<double>> weights;
			bool trained;
			bool updateAllWeights;
			std::string weightsFilePath;
		public:
			FieldCoupling(const ElementCommonParameters& elementCommonParameters, 
				const FieldCouplingParameters& fc_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			// close() used to be a pure virtual function in Element
			// and was implemented in FieldCoupling.cpp
			// resetWeights() used to be called here
			std::shared_ptr<Element> clone() const override;


			void setWeightsFilePath(const std::string& filePath);
			bool readWeights();
			void resetWeights();
			void saveWeights() const;
			virtual void updateWeights(const std::vector<double>& input, const std::vector<double>& output);

			void setLearningRate(double learningRate);
			void setUpdateAllWeights(bool updateAllWeights);
			void setParameters(const FieldCouplingParameters& fcp);

			const std::vector<std::vector<double>>& getWeights() const;
			FieldCouplingParameters getParameters() const;
		protected:
			void getInputFunction();
			void computeOutput();
			void scaleOutput();
			void writeWeights() const;
			void fillWeightsRandomly();
		};
	}
}
