// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "timer.h"


Timer::Timer(std::string functionName)
    : startTimepoint(std::chrono::high_resolution_clock::now()), functionName(std::move(functionName))
{
}

Timer::~Timer()
{
	stop();
}

void Timer::stop() const
{
    auto endTimepoint = std::chrono::high_resolution_clock::now();
    const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch();
    const auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch();
    const auto duration = end - start;
    std::cout << "Name: " << functionName << " Duration: " << duration << std::endl;
}