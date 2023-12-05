// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "dynamic-neural-field-composer.h"

// This .cpp file is an example of how you can use the library to create your own DNF simulation.
// This setup runs the application with a GUI.
// This .exe reads the elements for the simulation from a .txt file named "example-1".

int main(int argc, char* argv[])
{
    // After defining the simulation, we can create the application.
    auto simulation = std::make_shared<dnf_composer::Simulation>(5, 0, 0);
    // You can run the application without the user interface by setting the second parameter to false.
    constexpr bool activateUserInterface = true;
    const dnf_composer::Application app{ simulation, activateUserInterface };

    // After creating the application, we can add the windows we want to display.
    app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::SimulationWindow>(simulation));
    dnf_composer::user_interface::PlotDimensions pd = { 0, 100, -30, 40 };
    app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::PlotWindow>(simulation, pd));
    app.activateUserInterfaceWindow(std::make_shared<dnf_composer::user_interface::LoggerWindow>());

    dnf_composer::ArchitectureBuilder architecture{ "example-1", simulation };

    architecture.readArchitecture();
    architecture.saveArchitecture();

    try {
        app.init();

        bool userRequestClose = false;
        while (!userRequestClose)
        {
            app.step();
            userRequestClose = app.getCloseUI();
        }
        app.close();
        return 0;
    }
    catch (const dnf_composer::Exception& ex) {
        std::cerr << "Exception: " << ex.what() << " ErrorCode: " << static_cast<int>(ex.getErrorCode()) << std::endl;
        return static_cast<int>(ex.getErrorCode());
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception occurred." << std::endl;
        return 1;
    }
}