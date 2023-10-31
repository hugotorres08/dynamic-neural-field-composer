#include "elements/neural_field.h"


NeuralField::NeuralField(const std::string& id, const int& size,
	const NeuralFieldParameters& parameters)
	: parameters(parameters)
{
	// Assert that the size is positive
	assert(size > 0);

	this->label = ElementLabel::NEURAL_FIELD;
	this->uniqueIdentifier = id;
	this->size = size;

	centroid = -1.0;

	components["input"] = std::vector<double>(size);
	components["activation"] = std::vector<double>(size);
	components["output"] = std::vector<double>(size);
	components["resting level"] = std::vector<double>(size);
}

void NeuralField::init()
{
	std::ranges::fill(components["activation"], parameters.startingRestingLevel);
	std::ranges::fill(components["input"], 0.0);
	std::ranges::fill(components["resting level"], parameters.startingRestingLevel);

	calculateOutput();
}

void NeuralField::step(const double& t, const double& deltaT)
{
	updateInput();
	calculateActivation(t, deltaT);
	calculateOutput();
	calculateCentroid();
}

void NeuralField::close()
{
}

void NeuralField::setParameters(const NeuralFieldParameters& parameters)
{
	this->parameters = parameters;
	init();
}

NeuralFieldParameters NeuralField::getParameters() const
{
	return parameters;
}

double NeuralField::getCentroid() const
{
	return centroid;
}

void NeuralField::calculateActivation(const double& t, const double& deltaT)
{
	for (int i = 0; i < size; i++)
	{
		components["activation"][i] = components["activation"][i] + deltaT / parameters.tau *
			(-components["activation"][i] + components["resting level"][i] + components["input"][i]);
	}
}

void NeuralField::calculateOutput()
{
	switch (parameters.activationFunctionParameters.type)
	{
		case ActivationFunctionType::Sigmoid:
			components["output"] = mathtools::sigmoid(components["activation"], 
				parameters.activationFunctionParameters.steepness, static_cast<double>(parameters.activationFunctionParameters.xShift));
			break;
		case ActivationFunctionType::Heaviside:
			components["output"] = mathtools::heaviside(components["activation"], parameters.activationFunctionParameters.xShift);
			break;
	}
}

void NeuralField::calculateCentroid()
{
	const std::vector<double> f_output = mathtools::heaviside(components["activation"], 0.1);

	//if (*std::max_element(f_output.begin(), f_output.end()) > 0)
	if (*std::ranges::max_element(f_output) > 0) 
	{
		const bool isAtLimits = (f_output[0] > 0) || (f_output[size - 1] > 0);

		double sumActivation = 0.0;
		double sumWeightedPositions = 0.0;

		for (int i = 0; i < size; i++)
		{
			const double activation = f_output[i];

			sumActivation += activation;

			// Calculate the circular distance from the midpoint to the position
			double distance = 0.0;
			if (isAtLimits)
				distance = fmod(static_cast<double>(i) - static_cast<double>(size) * 0.5 + static_cast<double>(size) * 10, static_cast<double>(size));
			else
				distance = fmod(static_cast<double>(i) - static_cast<double>(size) * 0.5, static_cast<double>(size));
			sumWeightedPositions += distance * activation;
		}

		if (sumActivation != 0.0)
		{
			// Shift the centroid back to the circular field
			centroid = fmod(static_cast<double>(size) * 0.5 + sumWeightedPositions / sumActivation, static_cast<double>(size));
			if (isAtLimits)
				centroid = (centroid >= 0 ? centroid : centroid + static_cast<double>(size));
		}
	}
	else
		centroid = -1.0;
}