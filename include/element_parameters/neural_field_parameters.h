#pragma once

#include "element_parameters.h"
#include "elements/activation_function.h"


namespace dnf_composer
{
	namespace element
	{
		struct NeuralFieldParameters : ElementSpecificParameters
		{
			double tau;
			double startingRestingLevel;
			std::unique_ptr<ActivationFunction> activationFunction;

			NeuralFieldParameters();
			NeuralFieldParameters(double tau, double restingLevel, const ActivationFunction& activationFunction);
			NeuralFieldParameters(const NeuralFieldParameters& other);
			NeuralFieldParameters& operator=(const NeuralFieldParameters& other);
			bool operator==(const NeuralFieldParameters& other) const;
			std::string toString() const override;

		};

		struct NeuralFieldBump
		{
			double centroid;
			double startPosition;
			double endPosition;
			double amplitude;
			double width;

			NeuralFieldBump(double centroid = 0.0,
				double startPosition = 0.0,
				double endPosition = 0.0,
				double amplitude = 0.0,
				double width = 0.0);
			bool operator==(const NeuralFieldBump& other) const;
			std::string toString() const;
			void print() const;
		};

		struct NeuralFieldState
		{
			std::vector<NeuralFieldBump> bumps;
			bool stable;
			double lowestActivation;
			double highestActivation;
			double thresholdForStability;

			NeuralFieldState();
			std::string toString() const;
			void print() const;
		};
