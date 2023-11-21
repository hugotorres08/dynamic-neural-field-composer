cmake_minimum_required(VERSION 3.15)

project (dynamic-neural-field-composer
    DESCRIPTION "A lighter version of a C++ application to develop Dynamic Neural Field Architectures"
	LANGUAGES CXX
)

# Define library version
set(DNF_COMPOSER_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(DNF_COMPOSER_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(DNF_COMPOSER_VERSION ${DNF_COMPOSER_VERSION_MAJOR}.${DNF_COMPOSER_VERSION_MINOR} CACHE STRING "version" FORCE)

set(DNF_COMPOSER_INC_INSTALL_DIR "include")
set(DNF_COMPOSER_RUNTIME_INSTALL_DIR "bin")
set(DNF_COMPOSER_LIBRARY_INSTALL_DIR "bin")
set(DNF_COMPOSER_ARCHIVE_INSTALL_DIR "lib")
set(DNF_COMPOSER_FRAMEWORK_INSTALL_DIR "bin")
set(DNF_COMPOSER_CMAKE_CONFIG_INSTALL_DIR "share/${CMAKE_PROJECT_NAME}/cmake")
set(DNF_COMPOSER_ADDITIONAL_FILES_INSTALL_DIR "share/${CMAKE_PROJECT_NAME}")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check whether VCPKG is set up in your system
if(NOT DEFINED ENV{VCPKG_ROOT})
  message(FATAL_ERROR "ERROR: This project requires VCPKG.\n")
endif()

# Set VCPKG root directory
set(VCPKG_ROOT $ENV{VCPKG_ROOT})

# Include VCPKG toolchain
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# Set the desired output directory relative to the project
set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
# Pass the OUTPUT_DIRECTORY as a preprocessor definition
add_compile_definitions(OUTPUT_DIRECTORY="${OUTPUT_DIRECTORY}")


# Set header files
set(header
    "include/simulation/simulation.h"
    "include/simulation/visualization.h" 
     
    "include/application/application.h"
     
    "include/elements/element_factory.h"
    "include/elements/element.h"
    "include/elements/field_coupling.h"
    "include/elements/neural_field.h" 
    "include/elements/gauss_stimulus.h"  
    "include/elements/mexican_hat_kernel.h"
    "include/elements/gauss_kernel.h"
    "include/elements/normal_noise.h"
    "include/elements/gauss_field_coupling.h" 
    "include/elements/kernel.h"
     
    "include/mathtools/mathtools.h"
     
    "include/exceptions/exception.h"
     
    "include/user_interface/user_interface.h"
    "include/user_interface/user_interface_window.h"
    "include/user_interface/plot_window.h"
    "include/user_interface/simulation_window.h"
     
    "include/architecture/architecture-builder.h"
    "include/architecture/architecture-file-handler.h"
)

# Set source files
set(src 
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

# Define library target
add_library(${CMAKE_PROJECT_NAME} ${header} ${src})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> 
    $<INSTALL_INTERFACE:${DNF_COMPOSER_INC_INSTALL_DIR}> 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Setup imgui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui "d3d12.lib" "dxgi.lib" "d3dcompiler.lib")

# Setup implot
find_package(implot CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE implot::implot)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
                            DNF_COMPOSER=1
                            DNF_COMPOSER_VERSION_MAJOR=${DNF_COMPOSER_VERSION_MAJOR}
                            DNF_COMPOSER_VERSION_MINOR=${DNF_COMPOSER_VERSION_MINOR}
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
                                       OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${DNF_COMPOSER_VERSION}"
                                       POSITION_INDEPENDENT_CODE ON
)

install(TARGETS ${CMAKE_PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
        RUNTIME       DESTINATION ${DNF_COMPOSER_RUNTIME_INSTALL_DIR}
        LIBRARY       DESTINATION ${DNF_COMPOSER_LIBRARY_INSTALL_DIR}
        ARCHIVE       DESTINATION ${DNF_COMPOSER_ARCHIVE_INSTALL_DIR}
        FRAMEWORK     DESTINATION ${DNF_COMPOSER_FRAMEWORK_INSTALL_DIR}
)

include(CMakePackageConfigHelpers)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-config-version.cmake")
write_basic_package_version_file(${version_file}
                                 VERSION ${DNF_COMPOSER_VERSION}
                                 COMPATIBILITY AnyNewerVersion
)

install(FILES ${header} DESTINATION ${DNF_COMPOSER_INC_INSTALL_DIR})

install(EXPORT ${CMAKE_PROJECT_NAME}Targets DESTINATION ${DNF_COMPOSER_CMAKE_CONFIG_INSTALL_DIR}
                                       FILE ${CMAKE_PROJECT_NAME}-config.cmake
)

# Add executable project
set(EXE_PROJECT ${CMAKE_PROJECT_NAME}-exe)
add_executable(${EXE_PROJECT} "src/dynamic-field-composer-lite.cpp")
target_include_directories(${EXE_PROJECT} PRIVATE include)
target_link_libraries(${EXE_PROJECT} PRIVATE imgui::imgui ${CMAKE_PROJECT_NAME})

# Setup Catch2
enable_testing()
find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)

# Add test project
set(TEST_PROJECT ${CMAKE_PROJECT_NAME}-test)
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
target_link_libraries(${TEST_PROJECT} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain ${CMAKE_PROJECT_NAME})
# Automatically discover and add tests
catch_discover_tests(${TEST_PROJECT})
