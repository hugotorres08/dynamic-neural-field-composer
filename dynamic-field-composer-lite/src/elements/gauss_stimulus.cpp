#include "elements/gauss_stimulus.h"

GaussStimulus::GaussStimulus(const std::string& id, const int& size, const GaussStimulusParameters& parameters)
	: parameters(parameters)
{
	// Assert that the size is positive
	assert(size > 0);

	if (parameters.position < 0 || parameters.position >= size)
		throw Exception(ErrorCode::GAUSS_STIMULUS_POSITION_OUT_OF_RANGE, id);

	this->label = ElementLabel::GAUSS_STIMULUS;
	this->uniqueName = id;
	this->size = size;
	components["output"] = std::vector<double>(size);
	this->parameters.circular = true;
	this->parameters.normalized = false;
}

void GaussStimulus::init()
{
	std::vector<double> g(size);

	if (parameters.circular)
		g = mathtools::circularGauss(size, parameters.sigma, parameters.position);
	else
		std::cout << "Not implemented yet" << std::endl;

	if(!parameters.normalized)
		for (int i = 0; i < size; i++)
			components["output"][i] = parameters.amplitude * g[i];
	else
		std::cout << "Not implemented yet" << std::endl;

	// if stimulus is being used as a sum of stimulus
	components["input"] = std::vector<double>(size);
	updateInput();
	for (int i = 0; i < size; i++)
		components["output"][i] += components["input"][i];
}

void GaussStimulus::step(const double& t, const double& deltaT)
{
}

void GaussStimulus::close()
{
}

void GaussStimulus::setParameters(const GaussStimulusParameters& parameters)
{
	this->parameters = parameters;
	init();
}

GaussStimulusParameters GaussStimulus::getParameters() const
{
	return parameters;
}
