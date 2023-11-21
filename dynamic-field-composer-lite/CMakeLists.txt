cmake_minimum_required(VERSION 3.15)

project (dynamic-field-composer-lite
    VERSION 1.0.0 
    DESCRIPTION "A lighter version of a C++ application to develop Dynamic Neural Field Architectures"
	LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check whether VCPKG is set up in your system
if(NOT DEFINED ENV{VCPKG_ROOT})
  message(FATAL_ERROR "ERROR: This project requires VCPKG.\n")
endif()


find_package(MyCppProject REQUIRED)

# Set VCPKG root directory
set(VCPKG_ROOT $ENV{VCPKG_ROOT})

# Include VCPKG toolchain
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# Set the desired output directory relative to the project
set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
# Pass the OUTPUT_DIRECTORY as a preprocessor definition
add_compile_definitions(OUTPUT_DIRECTORY="${OUTPUT_DIRECTORY}")

# Add library project
set(LIB_PROJECT ${CMAKE_PROJECT_NAME}_LIB)
add_library(${LIB_PROJECT}
    "src/simulation/simulation.cpp"
    "src/simulation/visualization.cpp" 
    
    "src/application/application.cpp"
    
    "src/elements/element_factory.cpp"
    "src/elements/element.cpp"
    "src/elements/field_coupling.cpp"
    "src/elements/neural_field.cpp" 
    "src/elements/gauss_stimulus.cpp"  
    "src/elements/mexican_hat_kernel.cpp"
    "src/elements/gauss_kernel.cpp"
    "src/elements/normal_noise.cpp"
    "src/elements/gauss_field_coupling.cpp" 
    "src/elements/kernel.cpp"
    
    "src/mathtools/mathtools.cpp"

    "src/exceptions/exception.cpp"
    
    "src/user_interface/user_interface.cpp"
    "src/user_interface/user_interface_window.cpp"
    "src/user_interface/plot_window.cpp"
    "src/user_interface/simulation_window.cpp"

    "src/architecture/architecture-builder.cpp"
    "src/architecture/architecture-file-handler.cpp"
)
target_include_directories(${LIB_PROJECT} PUBLIC ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})

# Setup imgui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${LIB_PROJECT} PRIVATE imgui::imgui "d3d12.lib" "dxgi.lib" "d3dcompiler.lib")

# Setup implot
find_package(implot CONFIG REQUIRED)
target_link_libraries(${LIB_PROJECT} PRIVATE implot::implot)

# Add executable project
add_executable(${CMAKE_PROJECT_NAME} "src/dynamic-field-composer-lite.cpp")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui ${LIB_PROJECT})


# Setup Catch2
enable_testing()
find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)

# Add test project
set(TEST_PROJECT ${CMAKE_PROJECT_NAME}_TEST)
add_executable(${TEST_PROJECT} 
    tests/test.cpp 
    tests/test_application.cpp
    tests/test_element.cpp 
    tests/test_gauss_kernel.cpp 
    tests/test_gauss_stimulus.cpp 
    tests/test_mexican_hat_kernel.cpp 
    tests/test_neural_field.cpp 
    tests/test_normal_noise.cpp
    tests/test_simulation.cpp 
    tests/test_user_interface.cpp 
    tests/test_visualization.cpp 
)
target_include_directories(${TEST_PROJECT} PRIVATE include)
target_link_libraries(${TEST_PROJECT} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain ${LIB_PROJECT})
# Automatically discover and add tests
catch_discover_tests(${TEST_PROJECT})
