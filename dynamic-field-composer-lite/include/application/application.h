#pragma once

#include "exceptions/exception.h"
#include "simulation/simulation.h"
#include "simulation/visualization.h"
#include "user_interface/user_interface.h"

class Application
{
private:
	std::shared_ptr<Simulation> simulation;
	std::vector<std::shared_ptr<Visualization>> visualizations;
	std::shared_ptr<UserInterface> userInterface;
	bool activateUserInterface;
public:
	Application(const std::shared_ptr<Simulation>& simulation, bool activateUserInterface = true);
	Application(const Application&) = delete;             
	Application& operator=(const Application&) = delete;  
	Application(Application&&) = delete;                  
	Application& operator=(Application&&) = delete;       

	void init() const;
	void step() const;
	void close() const;

	void activateUserInterfaceWindow(const std::shared_ptr<UserInterfaceWindow>& window) const;
	void setActivateUserInterfaceAs(bool activateUI);
	bool getCloseUI() const;

	~Application() = default;
private:
	void addVisualization();
};
