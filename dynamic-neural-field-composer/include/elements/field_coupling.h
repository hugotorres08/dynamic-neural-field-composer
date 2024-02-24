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
		DELTA_KROGH_HERTZ,
	};

	namespace element
	{
		struct FieldCouplingParameters : ElementSpecificParameters
		{
			int inputFieldSize;
			double scalar;
			double learningRate;
			LearningRule learningRule;

			FieldCouplingParameters(int inputFieldSize, double scalar, double learningRate, LearningRule learningRule)
			: inputFieldSize(inputFieldSize), scalar(scalar), learningRate(learningRate), learningRule(learningRule)
			{}
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
			FieldCoupling(const ElementCommonParameters& elementCommonParameters, const FieldCouplingParameters& fc_parameters);

			void init() override;
			void step(double t, double deltaT) override;
			void close() override;
			void printParameters() override;
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

			~FieldCoupling() override = default;

		protected:
			void getInputFunction();
			void computeOutput();
			void scaleOutput();
			void writeWeights() const;
			void fillWeightsRandomly();
		};
	}
}
