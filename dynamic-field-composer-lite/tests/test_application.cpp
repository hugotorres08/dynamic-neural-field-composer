#include <catch2/catch_test_macros.hpp>

#include "application/application.h"
#include "simulation/simulation.h"

TEST_CASE("Application", "[Application]")
{
    SECTION("Initialization")
    {
        using namespace dnf_composer;
        // Test if the application initializes the simulation and user interface (if activated)
        const std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>();
        const Application application(simulation, true);

        REQUIRE_NOTHROW(application.init());
        // Additional checks for the initialized state of the simulation and user interface
    }

    SECTION("Step")
    {
        using namespace dnf_composer;

        // Test if the application steps the simulation and user interface (if activated)
        const std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>();
        const Application application(simulation, true);

        REQUIRE_NOTHROW(application.step());
        // Additional checks for the stepped state of the simulation and user interface
    }

    SECTION("Close")
    {
        using namespace dnf_composer;

        // Test if the application closes the simulation and user interface (if activated)
        const std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>();
        const Application application(simulation, true);

        REQUIRE_NOTHROW(application.close());
        // Additional checks for the closed state of the simulation and user interface
    }

    SECTION("Activate/Deactivate User Interface")
    {
        using namespace dnf_composer;

        // Test if the application correctly activates and deactivates the user interface
        std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>();
        Application application(simulation, false); // Deactivate user interface initially

        REQUIRE(application.getCloseUI() == false); // User interface should be deactivated initially
        REQUIRE(application.getActivateUserInterface() == false); 

        application.setActivateUserInterfaceAs(true);
        REQUIRE(application.getCloseUI() == false); // User interface is activated, but not yet closed
        REQUIRE(application.getActivateUserInterface() == true); 

        application.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::LoggerWindow>());
        // Additional checks for the activated state of the user interface window

        // Test deactivation
        application.setActivateUserInterfaceAs(false);
        REQUIRE(application.getCloseUI() == false); // User interface is deactivated, but not yet closed
        REQUIRE(application.getActivateUserInterface() == false); 

    }
}