cmake_minimum_required (VERSION 3.8)

set(TIMER_HEADERS
    ./include/timer.h
)

set(TIMER_SOURCES
    ./src/timer.cpp
)

add_library(profiling-tools ${TIMER_HEADERS} ${TIMER_SOURCES})


target_compile_definitions(profiling-tools PUBLIC)
set_property(TARGET profiling-tools PROPERTY CXX_STANDARD 20)

include_directories(./ ./include ./src)

