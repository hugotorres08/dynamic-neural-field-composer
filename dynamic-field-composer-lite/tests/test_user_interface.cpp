#include <catch2/catch_test_macros.hpp>

#include "user_interface/user_interface.h"
#include "user_interface/logger_window.h"

TEST_CASE("UserInterface", "[UserInterface]")
{
    SECTION("Initialization")
    {
        dnf_composer::user_interface::UserInterface userInterface;
        userInterface.init();
        REQUIRE(userInterface.getCloseUI() == false); // Ensure closeUI is initially false
        userInterface.close(); 
    }

    SECTION("Activate Window")
    {
        dnf_composer::user_interface::UserInterface userInterface;
        const auto window = std::make_shared<dnf_composer::user_interface::LoggerWindow>();
        userInterface.activateWindow(window);
        userInterface.init();
        userInterface.close();

    }
}