#pragma once

#include "element.h"
#include "activation_function.h"

#include "simulation/simulation.h"
#include "elements/kernel.h"

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

			NeuralFieldParameters()
				:tau(0), startingRestingLevel(0), activationFunction(nullptr)
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
		};

		struct NeuralFieldBump
		{
			double centroid;
			double startPosition;
			double endPosition;
			double amplitude;
			double width;
			double speed;
			double decay;

			NeuralFieldBump(double centroid = 0.0,
				double startPosition = 0.0,
				double endPosition = 0.0,
				double amplitude = 0.0, 
				double width = 0.0, 
				double speed = 0.0, 
				double decay = 0.0)
				: centroid(centroid),
			startPosition(startPosition),
			endPosition(endPosition),
			amplitude(amplitude),
			width(width), speed(speed), decay(decay)
			{}
		};

		struct NeuralFieldState
		{
			double centroid;
			double prevActivationSum;
			double prevActivationAvg;
			double prevActivationNorm;
			bool stable;
			std::vector<NeuralFieldBump> bumps;
			bool selfStabilized;
			bool selfSustained;
			double lowestActivation;
			double highestActivation;

			NeuralFieldState()
				:centroid(0.0), prevActivationSum(0.0), prevActivationAvg(0.0),
					prevActivationNorm(0.0), stable(false), bumps({}),
					selfStabilized(false), selfSustained(false),
					lowestActivation(0.0), highestActivation(0.0)
			{}
		};

		class NeuralField : public Element
		{
		protected:
			NeuralFieldParameters parameters;
			NeuralFieldState state;
		public:
			NeuralField(const ElementCommonParameters& elementCommonParameters, const NeuralFieldParameters& parameters);

			void init() override;
			void step(double t, double deltaT) override;
			void printParameters() override;
			std::shared_ptr<Element> clone() const override;


			void setParameters(const NeuralFieldParameters& parameters);
			NeuralFieldParameters getParameters() const;
		    double getCentroid() const;
			bool isStable() const;
			double getLowestActivation() const { return state.lowestActivation; }
			double getHighestActivation() const { return state.highestActivation; }
			std::vector<NeuralFieldBump> getBumps() const { return state.bumps; }
			std::shared_ptr<Kernel> getSelfExcitationKernel() const;
		protected:
			void calculateActivation(double t, double deltaT);
			void calculateOutput();
			void calculateCentroid();
			void updateState();
			void checkStability();
			void updateParameters();
			void updateMinMaxActivation();
			void updateBumps();
		};
	}
}
