// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "simulation/simulation.h"


Simulation::Simulation(double deltaT, double tZero, double t)
	: deltaT(deltaT), tZero(tZero), t(t)
{
	initialized = false;
	elements = {};
}

void Simulation::init() {
	t = tZero;
	for (const auto& element : elements)
		element->init();

	initialized = true;
}

void Simulation::step() {
	t += deltaT;
	for (const auto& element : elements)
		element->step(t, deltaT);
}

void Simulation::close() {
	for (const auto& element : elements)
		element->close();
	
	initialized = false;
}

void Simulation::run(double runTime)
{
	if (runTime <= 0)
		throw Exception(ErrorCode::SIM_RUNTIME_LESS_THAN_ZERO, static_cast<int>(runTime));

	const double simTime = t + runTime;

	if (!initialized)
		init();

	while (t < simTime)
		step();

	close();
}

void Simulation::addElement(const std::shared_ptr<Element>& element)
{
	// Check if an element with the same id already exists
	const int newElementId = element->getUniqueIdentifier();
	//for (int i = 0; i < elements.size(); i++) {
	//	if (elements[i]->getUniqueIdentifier() == newElementId) {
	//		throw Exception(ErrorCode::SIM_ELEM_ALREADY_EXISTS, newElementId);
	//		// Element with the same id already exists, throw an exception or handle the error
	//		//throw Exception(ErrorCode::SIM_ELEM_ALREADY_EXISTS, newElementId);
	//		// Alternatively, you can handle the error in any other way that makes sense for your application
	//		// such as logging an error message, returning an error code, etc.
	//		// ...
	//	}
	//}
	for (const auto& existingElement : elements) {
		if (existingElement->getUniqueIdentifier() == newElementId) {
			throw Exception(ErrorCode::SIM_ELEM_ALREADY_EXISTS, newElementId);
			// Handle the error here, such as logging an error message, returning an error code, etc.
			// ...
		}
	}


	elements.push_back(element);
	element->init(); 
}

void Simulation::removeElement(const std::string& elementId)
{
	for (const auto& element : elements)
		element->removeInput(elementId);

	for (int i = 0; i < static_cast<int>(elements.size()); i++)
	{
		// Remove the element from the simulation
		if (elements[i]->getUniqueName() == elementId)
		{
			elements.erase(elements.begin() + i);
			return;
		}
	}
	throw Exception(ErrorCode::SIM_ELEM_NOT_FOUND, elementId);
}

void Simulation::resetElement(const std::string& idOfElementToReset, const std::shared_ptr<Element>& newElement)
{
	bool elementFound = false;

	for (auto& element : elements) 
	{
		if (element->getUniqueName() == idOfElementToReset) 
		{
			element = newElement;
			element->init();
			elementFound = true;
			break;
		}
	}

	if (!elementFound)
		throw Exception(ErrorCode::SIM_ELEM_NOT_FOUND, idOfElementToReset);

}

void Simulation::createInteraction(const std::string& stimulusElementId, 
	const std::string& stimulusComponent, const std::string& receivingElementId) const
{
	const std::shared_ptr<Element> stimulusElement = getElement(stimulusElementId);
	const std::shared_ptr<Element> receivingElement = getElement(receivingElementId);
	receivingElement->addInput(stimulusElement, stimulusComponent);
}

std::shared_ptr<Element> Simulation::getElement(const std::string& id) const
{
	for (const auto& element : elements)
		if (element->getUniqueName() == id)
			return element;

	throw Exception(ErrorCode::SIM_ELEM_NOT_FOUND, id);
}

std::shared_ptr<Element> Simulation::getElement(const int index) const 
{
	if (index < static_cast<int>(elements.size()))
		return elements[index];

	throw Exception(ErrorCode::SIM_ELEM_INDEX, index);
}

std::vector<double> Simulation::getComponent(const std::string& id, const std::string& componentName) const
{

	const std::shared_ptr<Element> foundElement = getElement(id);
	return foundElement->getComponent(componentName);
}

std::vector<double>* Simulation::getComponentPtr(const std::string& id, const std::string& componentName) const
{
	const std::shared_ptr<Element> foundElement = getElement(id);
	return foundElement->getComponentPtr(componentName);
}

int Simulation::getNumberOfElements() const
{
	return static_cast<int>(elements.size());
}

std::vector<std::shared_ptr<Element>> Simulation::getElementsThatHaveSpecifiedElementAsInput(const std::string& specifiedElement, const std::string& inputComponent) const
{
	std::vector<std::shared_ptr<Element>> elementsThatHaveSpecifiedElementAsInput;
	for (const auto& element : elements) {
		if (element->hasInput(specifiedElement, inputComponent)) {
			elementsThatHaveSpecifiedElementAsInput.push_back(element);
		}
	}
	return elementsThatHaveSpecifiedElementAsInput;
}

bool Simulation::isInitialized() const
{
	return initialized;
}