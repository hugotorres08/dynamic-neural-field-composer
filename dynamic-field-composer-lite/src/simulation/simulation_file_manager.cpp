#include "./simulation/simulation_file_manager.h"



SimulationFileManager::SimulationFileManager()
{
	using json = nlohmann::json;

	std::cout << json::meta() << std::endl;
}
