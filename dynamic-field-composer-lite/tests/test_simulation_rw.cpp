#include <catch2/catch_test_macros.hpp>

#include "dynamic-neural-field-composer.h"

using namespace dnf_composer;

TEST_CASE("Simulation Read and Write", "[SimulationFileManager]")
{
	const std::shared_ptr<Simulation> sim = std::make_shared<Simulation>("name", 10, 0, 0);
	const element::GaussKernelParameters gkp1{ 5, 5 };
	const std::shared_ptr<element::GaussKernel> k_in_in(new element::GaussKernel({ "in - in", {100, 1.0} }, gkp1));
	sim->addElement(k_in_in);

	SECTION("save() and read()")
	{
		const int initialNumElements = sim->getNumberOfElements();
		sim->save();

		const std::shared_ptr<Simulation> newSim = std::make_shared<Simulation>("name", 10, 0, 0);
		newSim->read();
		const int newNumElements = newSim->getNumberOfElements();

		REQUIRE(initialNumElements == newNumElements);
	}
}