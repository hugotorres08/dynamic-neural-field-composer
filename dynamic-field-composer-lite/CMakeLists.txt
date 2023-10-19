cmake_minimum_required (VERSION 3.12)

project (dynamic-field-composer-lite
    VERSION 1.0.0 
    DESCRIPTION "A lighter version of a C++ application to develop Dynamic Neural Field Architectures")

# Add the source files
add_library(dynamic-field-composer-lite
    "src/simulation/simulation.cpp"
    "src/simulation/visualization.cpp" 
    
    "src/application/application.cpp"
    
    "src/elements/element.cpp"
    "src/elements/field_coupling.cpp"
    "src/elements/neural_field.cpp" 
    "src/elements/gauss_stimulus.cpp"  
    "src/elements/mexican_hat_kernel.cpp"
    "src/elements/gauss_kernel.cpp"
    "src/elements/normal_noise.cpp"
    
    "src/mathtools/mathtools.cpp"

    "src/exceptions/exception.cpp"
    
    "src/user_interface/user_interface.cpp"
    "src/user_interface/user_interface_window.cpp"
    "src/user_interface/plot_window.cpp"
    "src/user_interface/simulation_window.cpp" 
) 

# Set the desired output directory relative to the project
set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/data")

# Pass the OUTPUT_DIRECTORY as a preprocessor definition
add_compile_definitions(OUTPUT_DIRECTORY="${OUTPUT_DIRECTORY}")

# Set C++ standard and compile options
# Set the minimum required C++ version to C++11
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(dynamic-field-composer-lite PRIVATE cxx_std_20)

# Specify include directories
target_include_directories(dynamic-field-composer-lite PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Main executable
add_executable(launch-dynamic-field-composer-lite
    "src/dynamic-field-composer-lite.cpp"
)

add_subdirectory(lib)

target_link_libraries(launch-dynamic-field-composer-lite PUBLIC imgui)
target_link_libraries(launch-dynamic-field-composer-lite PUBLIC implot)
target_link_libraries(launch-dynamic-field-composer-lite PRIVATE dynamic-field-composer-lite)

# Set C++ standard and compile options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(launch-dynamic-field-composer-lite PRIVATE cxx_std_20)

# Add Test library
add_subdirectory(tests)
