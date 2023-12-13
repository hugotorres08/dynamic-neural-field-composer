#pragma once

#include "element.h"
#include "activation_function.h"

namespace dnf_composer
{
	namespace element
	{
		struct NeuralFieldParameters
		{
		    double tau;
		    double startingRestingLevel;
			std::unique_ptr<ActivationFunction> activationFunction;

			NeuralFieldParameters(const NeuralFieldParameters& other)
			{
				tau = other.tau;
				startingRestingLevel = other.startingRestingLevel;
				activationFunction = other.activationFunction->clone();
			}
		};

		class NeuralField : public Element
		{
		protected:
			NeuralFieldParameters parameters;
		    double centroid;
		public:
			NeuralField(const ElementCommonParameters& commonParameters, const NeuralFieldParameters& parameters);

			void init() override;
			void step(double t, double deltaT) override;
			void close() override;

			void printParameters() override;

			void setParameters(const NeuralFieldParameters& parameters);
			NeuralFieldParameters getParameters() const;
		    double getCentroid() const;

			~NeuralField() override = default;

		protected:
			void calculateActivation(double t, double deltaT);
			void calculateOutput();
			void calculateCentroid();
		};
	}
}
