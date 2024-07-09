#pragma once

#include "element.h"
#include "activation_function.h"
#include "simulation/simulation.h"
#include "elements/kernel.h"
#include "tools/logger.h"

namespace dnf_composer
{
	namespace element
	{
		struct NeuralFieldParameters : ElementSpecificParameters
		{
			double tau;
			double startingRestingLevel;
			std::unique_ptr<ActivationFunction> activationFunction;

			NeuralFieldParameters& operator=(const NeuralFieldParameters& other)
			{
				if (this != &other)
				{
					tau = other.tau;
					startingRestingLevel = other.startingRestingLevel;
					if (other.activationFunction)
						activationFunction = other.activationFunction->clone();
					else
						activationFunction.reset();
				}
				return *this;
			}

			bool operator==(const NeuralFieldParameters& other) const
			{
				constexpr double epsilon = 1e-6;
				return std::abs(tau - other.tau) < epsilon &&
					std::abs(startingRestingLevel - other.startingRestingLevel) < epsilon &&
					activationFunction == other.activationFunction;
			}

			NeuralFieldParameters()
				:tau(0.0), startingRestingLevel(0.0), activationFunction(nullptr)
			{}

			NeuralFieldParameters(double tau, double restingLevel,
				const ActivationFunction& activationFunction)
				: tau(tau), startingRestingLevel(restingLevel),
				activationFunction(activationFunction.clone())
			{ }

			NeuralFieldParameters(const NeuralFieldParameters& other)
			{
				tau = other.tau;
				startingRestingLevel = other.startingRestingLevel;
				if (other.activationFunction == nullptr)
					activationFunction = std::make_unique<HeavisideFunction>(0);
				else
					activationFunction = other.activationFunction->clone();
			}

			std::string toString() const override
			{
				std::string result = "Neural field parameters\n";
				result += "Tau: " + std::to_string(tau) + "\n";
				result += "Resting level: " + std::to_string(startingRestingLevel) + "\n";
				result += "Activation function: " + activationFunction->toString() + "\n";
				return result;
			}

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
				double width = 0.0)
				: centroid(centroid),
				startPosition(startPosition),
				endPosition(endPosition),
				amplitude(amplitude),
				width(width)
			{}

			std::string toString() const
			{
				std::string str = "Bump\n";
				str += "Centroid: " + std::to_string(centroid) + "\n";
				str += "Amplitude: " + std::to_string(amplitude) + "\n";
				str += "Start position: " + std::to_string(startPosition) + "\n";
				str += "End position: " + std::to_string(endPosition) + "\n";
				str += "Width: " + std::to_string(width) + "\n";
				return str;
			}

			void print() const;
			
		};

		struct NeuralFieldState
		{
			std::vector<NeuralFieldBump> bumps;
			bool stable;
			double lowestActivation;
			double highestActivation;
			double thresholdForStability;
			double previousActivationSum = 0.0;
			double previousActivationAvg = 0.0;
			double previousActivationNorm = 0.0;

			NeuralFieldState()
				:bumps({}), stable(false), lowestActivation(0.0),
				highestActivation(0.0), thresholdForStability(0.035)
			{}

			std::string toString() const
			{
				std::string str = "Neural field state\n";
				str += "Stable: " + std::to_string(stable) + "\n";
				str += "Lowest activation: " + std::to_string(lowestActivation) + "\n";
				str += "Highest activation: " + std::to_string(highestActivation) + "\n";
				str += "Threshold for stability: " + std::to_string(thresholdForStability) + "\n";
				str += "Bumps:\n";
				for (const auto& bump : bumps)
					str += bump.toString();
				return str;
			}

			void print() const;

		};

		class NeuralField : public Element
		{
		protected:
			NeuralFieldParameters parameters;
			NeuralFieldState state;
		public:
			NeuralField(const ElementCommonParameters& elementCommonParameters,
				const NeuralFieldParameters& parameters);

			void init() override;
			void step(double t, double deltaT) override;
			std::string toString() const override;
			std::shared_ptr<Element> clone() const override;

			void setThresholdForStability(double threshold) { state.thresholdForStability = threshold; }
			void setParameters(const NeuralFieldParameters& parameters);
			NeuralFieldParameters getParameters() const;
			//double getCentroid() const;
			bool isStable() const;
			double getLowestActivation() const { return state.lowestActivation; }
			double getHighestActivation() const { return state.highestActivation; }
			std::vector<NeuralFieldBump> getBumps() const { return state.bumps; }
			std::shared_ptr<Kernel> getSelfExcitationKernel() const;
		protected:
			void calculateActivation(double t, double deltaT);
			void calculateOutput();
			//void calculateCentroid();
			void updateState();
			void checkStability();
			void updateMinMaxActivation();
			void updateBumps();
		};
	}
}